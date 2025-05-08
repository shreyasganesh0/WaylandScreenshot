#include "frame_handler.h"

static const struct zwlr_screencopy_frame_v1_listener frame_listener = {
    .buffer_done = frame_handle_buffer_done,
    .flags       = frame_handle_flags,
    .ready       = frame_handle_ready,
    .failed      = frame_handle_failed,
    .format      = frame_handle_format,
    .buffer      = frame_handle_buffer
};

    
static void frame_handle_buffer_done(void *data, struct zwlr_screencopy_frame_v1 *frame) {

    zwlr_screencopy_frame_v1_copy(frame);
}

static void frame_handle_flags(void *data, struct zwlr_screencopy_frame_v1 *frame, uint32_t flags) {}

static void frame_handle_ready(void *data, struct zwlr_screencopy_frame_v1 *frame, uint32_t tv_sec_hi, uint32_t tv_sec_lo, uint32_t tv_nsec) {

    FILE *fo = fopen(file_path, "wb");
    if (!fo) {
        perror("fopen");
        exit(1);
    }
    fprintf(f, "
}

static void frame_handle_failed(void *data, struct zwlr_screencopy_frame_v1 *frame) {

    printf("Frame capture failed\n");
    exit(1);
}

static void frame_handle_format(void *data, struct zwlr_screencopy_frame_v1 *frame, uint32_t format) {

    frame_cap.format = format;
}

struct wl_buffer_listener buffer_listener = {

    .release = buffer_release
};

static void buffer_release(void *data, wl_buffer *wl_buffer) {

    wl_buffer_destroy(wl_buffer);
}

static void frame_handle_buffer(void *data, struct zwlr_screencopy_frame_v1 *frame, uint32_t format, uint32_t height, uint32_t width, uin32_t stride) {

    frame_cap.width = width;
    frame_cap.height = height;
    frame_cap.stride = stride;

    size_t size = stride * height;
    char template[] = "~/Screenshots/screenshot-XXXXXX";
    int frame_cap.fd = mkstemp(template);
    if (frame_cap.fd == -1) {

        printf("Failed to create file at path %s\n", file_path);
        return -1;
    }

    ftruncate(frame_cap.fd, size);
    frame_cap.data = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, cap.fd, 0);
    if (frame_cap.data == NULL) {

        printf("Failed to allocated memory\n");
        return  -1;
    }
    unlink(template);

    struct wl_shm_pool *pool = wl_shm_pool_create(shm, frame_cap.fd, size);
    frame_cap.wl_buffer - wl_shm_pool_create_buffer(pool, 0 , width, height, stride, WL_SHMFORMAT_ARGB888);
    wl_shm_pool_destroy(pool);
    wl_buffer_add_listener(frame_cap.wl_buffer, &buffer_listener, NULL);

}
