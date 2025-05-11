static struct wl_display         *display = NULL;
static struct wl_shm             *shm = NULL;
static struct zwlr_screencopy_manager_v1 *screencopy = NULL;
static struct wl_output          *output = NULL;

capture_t frame_cap = {0};

struct wl_buffer_listener buffer_listener = {

    .release = buffer_release
};

struct zwlr_screencopy_frame_v1_listener frame_listener = {
    .flags       = frame_handle_flags,
    .ready       = frame_handle_ready,
    .failed      = frame_handle_failed,
    .buffer      = frame_handle_buffer
};

struct wl_registry_listener registry_listener = {

    .global =  registry_global_handler, 
    .global_remove = registry_global_handler_remove,
};
