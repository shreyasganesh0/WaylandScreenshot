static struct wl_display         *display = NULL;
static struct wl_shm             *shm = NULL;
static struct zwlr_screencopy_manager_v1 *screencopy = NULL;
static struct wl_output          *output = NULL;

capture_t frame_cap = {0};
