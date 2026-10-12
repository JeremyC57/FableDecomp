// See egl_fbdev.hpp. EGL and GLES are loaded with dlopen so the binary carries no dependency on
// them (the package ships none; the OS's GPU driver provides them).
#include "egl_fbdev.hpp"

#include "host.hpp"

#include <EGL/egl.h>
#include <dlfcn.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include <cstdlib>
#include <cstring>

namespace host::gles::fb {
namespace {

struct FbdevWindow {  // Arm's fbdev EGL native window
    unsigned short width, height;
};

void* g_egl = nullptr;
void* g_gles = nullptr;
EGLDisplay g_display = EGL_NO_DISPLAY;
EGLSurface g_surface = EGL_NO_SURFACE;
EGLContext g_context = EGL_NO_CONTEXT;
FbdevWindow g_window{};

decltype(&eglGetDisplay) p_eglGetDisplay;
decltype(&eglInitialize) p_eglInitialize;
decltype(&eglBindAPI) p_eglBindAPI;
decltype(&eglChooseConfig) p_eglChooseConfig;
decltype(&eglGetConfigAttrib) p_eglGetConfigAttrib;
decltype(&eglCreateWindowSurface) p_eglCreateWindowSurface;
decltype(&eglCreatePbufferSurface) p_eglCreatePbufferSurface;
decltype(&eglCreateContext) p_eglCreateContext;
decltype(&eglMakeCurrent) p_eglMakeCurrent;
decltype(&eglSwapBuffers) p_eglSwapBuffers;
decltype(&eglSwapInterval) p_eglSwapInterval;
decltype(&eglGetProcAddress) p_eglGetProcAddress;
decltype(&eglGetError) p_eglGetError;
decltype(&eglDestroySurface) p_eglDestroySurface;
decltype(&eglDestroyContext) p_eglDestroyContext;
decltype(&eglTerminate) p_eglTerminate;
decltype(&eglQueryString) p_eglQueryString;

void* openLib(const char* const* names) {
    for (; *names; ++names)
        if (void* h = dlopen(*names, RTLD_NOW | RTLD_GLOBAL)) { log("gles: loaded %s", *names); return h; }
    return nullptr;
}

template <class F> bool load(F& f, const char* name) {
    f = reinterpret_cast<F>(dlsym(g_egl, name));
    if (!f) log("gles: libEGL lacks %s", name);
    return f != nullptr;
}

bool fbSize(int& w, int& h) {
    const char* dev = std::getenv("FABLE_FBDEV");
    const int fd = ::open(dev ? dev : "/dev/fb0", O_RDWR);
    if (fd < 0) return false;
    fb_var_screeninfo v{};
    const bool ok = ioctl(fd, FBIOGET_VSCREENINFO, &v) == 0 && v.xres && v.yres;
    if (ok) w = static_cast<int>(v.xres), h = static_cast<int>(v.yres);
    ::close(fd);
    return ok;
}

}  // namespace

bool open(int& width, int& height) {
    const char* mode = std::getenv("FABLE_GL_WINDOW");
    const bool pbuffer = mode && std::strcmp(mode, "pbuffer") == 0;
    static const char* const eglNames[] = {"libEGL.so.1", "libEGL.so", "libmali.so", "libMali.so", nullptr};
    static const char* const glesNames[] = {"libGLESv2.so.2", "libGLESv2.so", "libmali.so", "libMali.so", nullptr};
    g_egl = openLib(eglNames);
    if (!g_egl) { log("gles: no libEGL on this system (%s)", dlerror()); return false; }
    g_gles = openLib(glesNames);
    if (!g_gles) { log("gles: no libGLESv2 on this system (%s)", dlerror()); return false; }
    if (!(load(p_eglGetDisplay, "eglGetDisplay") && load(p_eglInitialize, "eglInitialize") && load(p_eglBindAPI, "eglBindAPI") &&
          load(p_eglChooseConfig, "eglChooseConfig") && load(p_eglGetConfigAttrib, "eglGetConfigAttrib") &&
          load(p_eglCreateWindowSurface, "eglCreateWindowSurface") && load(p_eglCreatePbufferSurface, "eglCreatePbufferSurface") &&
          load(p_eglCreateContext, "eglCreateContext") && load(p_eglMakeCurrent, "eglMakeCurrent") &&
          load(p_eglSwapBuffers, "eglSwapBuffers") && load(p_eglSwapInterval, "eglSwapInterval") &&
          load(p_eglGetProcAddress, "eglGetProcAddress") && load(p_eglGetError, "eglGetError") &&
          load(p_eglDestroySurface, "eglDestroySurface") && load(p_eglDestroyContext, "eglDestroyContext") &&
          load(p_eglTerminate, "eglTerminate") && load(p_eglQueryString, "eglQueryString")))
        return false;

    int w = 720, h = 480;
    if (const char* s = std::getenv("FABLE_SCREEN")) std::sscanf(s, "%dx%d", &w, &h);
    else if (!fbSize(w, h) && !pbuffer) log("gles: /dev/fb0 size unknown, assuming %dx%d", w, h);
    width = w, height = h;

    g_display = p_eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (g_display == EGL_NO_DISPLAY) { log("gles: eglGetDisplay failed"); return false; }
    EGLint major = 0, minor = 0;
    if (!p_eglInitialize(g_display, &major, &minor)) { log("gles: eglInitialize failed (0x%X)", p_eglGetError()); return false; }
    log("gles: EGL %d.%d %s | %s", major, minor, p_eglQueryString(g_display, EGL_VENDOR), p_eglQueryString(g_display, EGL_VERSION));
    p_eglBindAPI(EGL_OPENGL_ES_API);

    EGLConfig configs[32];
    EGLConfig config = nullptr;
    EGLint n = 0;
    const EGLint surfaceType = pbuffer ? EGL_PBUFFER_BIT : EGL_WINDOW_BIT;
    for (int pass = 0; pass < 3 && n == 0; ++pass) {
        // 1: ES3 bit with 24-bit colour; 2: ES2 bit (Mali drivers predating the ES3 bit still
        // create ES 3.x contexts); 3: anything renderable
        const EGLint attribs[] = {
            EGL_SURFACE_TYPE, surfaceType,
            EGL_RENDERABLE_TYPE, pass == 0 ? EGL_OPENGL_ES3_BIT : EGL_OPENGL_ES2_BIT,
            EGL_RED_SIZE, pass < 2 ? 8 : 5, EGL_GREEN_SIZE, pass < 2 ? 8 : 6, EGL_BLUE_SIZE, pass < 2 ? 8 : 5,
            EGL_DEPTH_SIZE, 0, EGL_STENCIL_SIZE, 0,
            EGL_NONE};
        if (!p_eglChooseConfig(g_display, attribs, configs, 32, &n)) n = 0;
        if (n > 0) {  // prefer 8 bits per channel (Mesa lists 10-bit first)
            config = configs[0];
            for (EGLint i = 0; i < n; ++i) {
                EGLint red = 0;
                p_eglGetConfigAttrib(g_display, configs[i], EGL_RED_SIZE, &red);
                if (red == 8) { config = configs[i]; break; }
            }
        }
    }
    if (n == 0) { log("gles: no EGL config for a %s surface", pbuffer ? "pbuffer" : "window"); return false; }
    EGLint r = 0, g = 0, b = 0, a = 0;
    p_eglGetConfigAttrib(g_display, config, EGL_RED_SIZE, &r), p_eglGetConfigAttrib(g_display, config, EGL_GREEN_SIZE, &g);
    p_eglGetConfigAttrib(g_display, config, EGL_BLUE_SIZE, &b), p_eglGetConfigAttrib(g_display, config, EGL_ALPHA_SIZE, &a);
    log("gles: EGL config R%dG%dB%dA%d", r, g, b, a);

    if (pbuffer) {
        const EGLint pb[] = {EGL_WIDTH, w, EGL_HEIGHT, h, EGL_NONE};
        g_surface = p_eglCreatePbufferSurface(g_display, config, pb);
    } else {
        g_window.width = static_cast<unsigned short>(w), g_window.height = static_cast<unsigned short>(h);
        g_surface = p_eglCreateWindowSurface(g_display, config, reinterpret_cast<EGLNativeWindowType>(&g_window), nullptr);
    }
    if (g_surface == EGL_NO_SURFACE) { log("gles: eglCreate%sSurface failed (0x%X)", pbuffer ? "Pbuffer" : "Window", p_eglGetError()); return false; }
    for (EGLint version = 3; version >= 3 && g_context == EGL_NO_CONTEXT; --version) {
        const EGLint ctx[] = {EGL_CONTEXT_CLIENT_VERSION, version, EGL_NONE};
        g_context = p_eglCreateContext(g_display, config, EGL_NO_CONTEXT, ctx);
    }
    if (g_context == EGL_NO_CONTEXT) { log("gles: eglCreateContext (ES 3) failed (0x%X)", p_eglGetError()); return false; }
    if (!p_eglMakeCurrent(g_display, g_surface, g_surface, g_context)) { log("gles: eglMakeCurrent failed (0x%X)", p_eglGetError()); return false; }
    p_eglSwapInterval(g_display, 1);
    log("gles: EGL %s surface %dx%d", pbuffer ? "pbuffer" : "framebuffer", w, h);
    return true;
}

void* proc(const char* name) {
    void* p = g_gles ? dlsym(g_gles, name) : nullptr;
    if (!p && p_eglGetProcAddress) p = reinterpret_cast<void*>(p_eglGetProcAddress(name));
    return p;
}

void swap() {
    if (g_display != EGL_NO_DISPLAY && g_surface != EGL_NO_SURFACE) p_eglSwapBuffers(g_display, g_surface);
}

void setSwapInterval(int interval) {
    if (g_display != EGL_NO_DISPLAY) p_eglSwapInterval(g_display, interval);
}

void close() {
    if (g_display == EGL_NO_DISPLAY) return;
    p_eglMakeCurrent(g_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    if (g_context != EGL_NO_CONTEXT) p_eglDestroyContext(g_display, g_context);
    if (g_surface != EGL_NO_SURFACE) p_eglDestroySurface(g_display, g_surface);
    p_eglTerminate(g_display);
    g_display = EGL_NO_DISPLAY, g_surface = EGL_NO_SURFACE, g_context = EGL_NO_CONTEXT;
}

}  // namespace host::gles::fb
