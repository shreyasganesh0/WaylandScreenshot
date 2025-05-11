
static char* convert_to_rgb() {

    uint8_t *ptr = malloc(frame_cap.width * frame_cap.height * 3);
    char *px = ((char*)frame_cap.data);
    for (int i = 0; i < frame_cap.width * frame_cap.height; i++) {

        ptr[3*i + 0] = px[3*i + 2];
        ptr[3*i + 1] = px[3*i + 1];
        ptr[3*i + 2] = px[3*i + 0];
    }

    return ptr;
}

static void frame_handle_flags(void *data, struct zwlr_screencopy_frame_v1 *frame, uint32_t flags) {}

static void frame_handle_ready(void *data, struct zwlr_screencopy_frame_v1 *frame, uint32_t tv_sec_hi, uint32_t tv_sec_lo, uint32_t tv_nsec) {

    fprintf(stderr, "Handling the screen buf\n");

    time_t now = time(NULL);
    if (now == (time_t)-1) {

        perror("time");
        return;
    }
    struct tm *t = localtime(&now);
    if (!t) {

        perror("localtime");
        return;
    }

    char buf[64];
    if (strftime(buf, sizeof(buf), "%Y-%m-%d--%H:%M:%S", t) == 0) {

        fprintf(stderr, "strftime error\n");
        return;
    }

    char file_path[128];
    sprintf(file_path, "/home/shreyas/Screenshots/screenshot-%s.jpg", buf);
    int fd = open(file_path, O_RDWR|O_CREAT|O_TRUNC, 0644);
    if (fd == -1) {

        printf("Failed to create ss file\n");
    }

    printf("Writing to file path %s\n", file_path);

    char *rgb = convert_to_rgb();
    int quality = 90; // 1 - 100
    if(!stbi_write_jpg(file_path, frame_cap.width, frame_cap.height, 3, rgb, quality)) {
        fprintf(stderr, "stbi_write_jpg failed: %s\n", strerror(errno));
        return;
    }
    zwlr_screencopy_frame_v1_destroy(frame);
    free(rgb);

}

static void frame_handle_failed(void *data, struct zwlr_screencopy_frame_v1 *frame) {

    printf("Frame capture failed\n");
    exit(1);
}

/*static void frame_handle_format(void *data, struct zwlr_screencopy_frame_v1 *frame, uint32_t format) {

    frame_cap.format = format;
}*/


static void buffer_release(void *data, struct wl_buffer *wl_buffer) {

    size_t size = (size_t)frame_cap.stride * frame_cap.height;
    munmap(frame_cap.data, size);
    close(frame_cap.fd);
    wl_buffer_destroy(wl_buffer);
}

static void frame_handle_buffer(void *data, struct zwlr_screencopy_frame_v1 *frame, uint32_t format, uint32_t width, uint32_t height, uint32_t stride) {

    frame_cap.format = format;
    frame_cap.width = width;
    frame_cap.height = height;
    frame_cap.stride = stride;
    fprintf(stderr,
            "→ BUFFER EVENT: fmt=0x%08x, width=%u, height=%u, stride=%u\n",
            format, width, height, stride);

    size_t size = (size_t)stride * height;
    char template[] = "/tmp/screenshot-XXXXXX";
    frame_cap.fd = mkstemp(template);
    if (frame_cap.fd == -1) {

        printf("Failed to create file at path %s\n", template);
        return;
    }

    ftruncate(frame_cap.fd, size);
    frame_cap.data = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, frame_cap.fd, 0);
    if (frame_cap.data == MAP_FAILED) {

        perror("mmap");
        close(frame_cap.fd);
        return;
    }
    unlink(template);

    struct wl_shm_pool *pool = wl_shm_create_pool(shm, frame_cap.fd, size);
    if (!pool) {

        printf("Failed to create pool\n");
    }
    frame_cap.wl_buffer = wl_shm_pool_create_buffer(pool, 0 , width, height, stride, format);
    if (!frame_cap.wl_buffer) {

        printf("Failed to create pool\n");
    }
    wl_shm_pool_destroy(pool);
    wl_buffer_add_listener(frame_cap.wl_buffer, &buffer_listener, NULL);
    fprintf(stderr, "copying\n");
    zwlr_screencopy_frame_v1_copy(frame, frame_cap.wl_buffer);

}
