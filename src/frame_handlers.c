static void frame_handle_buffer_done(void *data, struct zwlr_screencopy_frame_v1 *frame) {

    zwlr_screencopy_frame_v1_copy(frame, frame_cap.wl_buffer);
}

static char* convert_to_rgb() {

    uint8_t *ptr = malloc(frame_cap.width * frame_cap.height * 3);
    for (int i = 0; i < frame_cap.width * frame_cap.height; i++) {

        uint32_t px = ((uint32_t*)frame_cap.data)[i];
        ptr[3*i + 0] = (px >> 16) & 0XFF;
        ptr[3*i + 1] = (px >> 8) & 0XFF;
        ptr[3*i + 2] = (px) & 0XFF;
    }

    return ptr;
}

static void frame_handle_flags(void *data, struct zwlr_screencopy_frame_v1 *frame, uint32_t flags) {}

static void frame_handle_ready(void *data, struct zwlr_screencopy_frame_v1 *frame, uint32_t tv_sec_hi, uint32_t tv_sec_lo, uint32_t tv_nsec) {


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
    sprintf(file_path, "Screenshots/screenshot-%s.png", buf);

    char *rgb = convert_to_rgb();
    int quality = 90; // 1 - 100
    if(!stbi_write_jpg(file_path, frame_cap.width, frame_cap.height, 3, rgb, quality)) {

        fprintf(stderr, "Failed to write png\n");
        return;
    }

}

static void frame_handle_failed(void *data, struct zwlr_screencopy_frame_v1 *frame) {

    printf("Frame capture failed\n");
    exit(1);
}

/*static void frame_handle_format(void *data, struct zwlr_screencopy_frame_v1 *frame, uint32_t format) {

    frame_cap.format = format;
}*/


static void buffer_release(void *data, struct wl_buffer *wl_buffer) {

    wl_buffer_destroy(wl_buffer);
}

static void frame_handle_buffer(void *data, struct zwlr_screencopy_frame_v1 *frame, uint32_t format, uint32_t height, uint32_t width, uint32_t stride) {

    frame_cap.width = width;
    frame_cap.height = height;
    frame_cap.stride = stride;

    size_t size = stride * height;
    char template[] = "~/Screenshots/screenshot-XXXXXX";
    frame_cap.fd = mkstemp(template);
    if (frame_cap.fd == -1) {

        printf("Failed to create file at path %s\n", template);
        return;
    }

    ftruncate(frame_cap.fd, size);
    frame_cap.data = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, frame_cap.fd, 0);
    if (frame_cap.data == NULL) {

        printf("Failed to allocated memory\n");
        return;
    }
    unlink(template);

    struct wl_shm_pool *pool = wl_shm_create_pool(shm, frame_cap.fd, size);
    frame_cap.wl_buffer = wl_shm_pool_create_buffer(pool, 0 , width, height, stride, WL_SHM_FORMAT_ARGB8888);
    wl_shm_pool_destroy(pool);
    struct wl_buffer_listener buffer_listener = {

        .release = buffer_release
    };
    wl_buffer_add_listener(frame_cap.wl_buffer, &buffer_listener, NULL);

}
