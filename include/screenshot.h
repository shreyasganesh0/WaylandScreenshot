#ifndef SCREENSHOT_H
#define SCREENSHOT_H

static void registry_global_handler_remove(void *data, struct wl_registry *registry, uint32_t name);
static void registry_global_handler(void *data, struct wl_registry *registry, uint32_t name, const char *interface, uint32_t version);

#endif
