#ifndef FRAME_HANDLER_H
#define FRAME_HANDLER_H


typedef struct {

    uint32_t height;
    uint32_t width;
    uint32_t stride;
    uint32_t format;
    void *data;
    int fd;
    struct wl_buffer *wl_buffer;
} capture_t;
    

static void buffer_release(void *data, struct wl_buffer *wl_buffer);
static void frame_handle_buffer_done(void *data, struct zwlr_screencopy_frame_v1 *frame);
static void frame_handle_flags(void *data, struct zwlr_screencopy_frame_v1 *frame, uint32_t flags);
static void frame_handle_ready(void *data, struct zwlr_screencopy_frame_v1 *frame, uint32_t tv_sec_hi, uint32_t tv_sec_lo, uint32_t tv_nsec);
static void frame_handle_failed(void *data, struct zwlr_screencopy_frame_v1 *frame);
/*static void frame_handle_format(void *data, struct zwlr_screencopy_frame_v1 *frame, uint32_t format);*/
static void frame_handle_buffer(void *data, struct zwlr_screencopy_frame_v1 *frame, uint32_t format, uint32_t height, uint32_t width, uint32_t stride);
static char* convert_to_rgb();

#endif
