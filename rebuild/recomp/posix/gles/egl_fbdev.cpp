// See egl_fbdev.hpp. EGL and GLES are loaded with dlopen so the binary carries no dependency on
// them (the package ships none; the OS's GPU driver provides them). Several candidate
// libraries and surface kinds are tried in turn, since what a handheld's OS provides varies:
//   1. a window surface on Arm's fbdev native window (the fbdev Mali blob) — presented by
//      eglSwapBuffers;
//   2. a pbuffer surface — presented by reading the frame back and copying it into /dev/fb0
//      (any EGL that initialises: a GBM/surfaceless Mali variant, Mesa).
#include "egl_fbdev.hpp"

#include "host.hpp"

#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES3/gl3.h>
#include <dlfcn.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

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
bool g_copy = false;  // present by copying the frame into the framebuffer
bool g_surfaceIsWindow = false;
int g_w = 0, g_h = 0;

// the framebuffer, for the copy presenter
int g_fbFd = -1;
unsigned char* g_fbMem = nullptr;
size_t g_fbLen = 0;
fb_var_screeninfo g_var{};
fb_fix_screeninfo g_fix{};
std::vector<unsigned char> g_readback;

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
PFNEGLGETPLATFORMDISPLAYEXTPROC p_eglGetPlatformDisplayEXT;
PFNGLREADPIXELSPROC p_glReadPixels;
PFNGLBINDFRAMEBUFFERPROC p_glBindFramebuffer;
PFNGLFINISHPROC p_glFinish;

template <class F> bool load(F& f, const char* name, bool required = true) {
    f = reinterpret_cast<F>(dlsym(g_egl, name));
    if (!f && required) log("gles: %s lacks %s", "libEGL", name);
    return f != nullptr;
}

bool loadEgl() {
    return load(p_eglGetDisplay, "eglGetDisplay") && load(p_eglInitialize, "eglInitialize") && load(p_eglBindAPI, "eglBindAPI") &&
           load(p_eglChooseConfig, "eglChooseConfig") && load(p_eglGetConfigAttrib, "eglGetConfigAttrib") &&
           load(p_eglCreateWindowSurface, "eglCreateWindowSurface") && load(p_eglCreatePbufferSurface, "eglCreatePbufferSurface") &&
           load(p_eglCreateContext, "eglCreateContext") && load(p_eglMakeCurrent, "eglMakeCurrent") &&
           load(p_eglSwapBuffers, "eglSwapBuffers") && load(p_eglSwapInterval, "eglSwapInterval") &&
           load(p_eglGetProcAddress, "eglGetProcAddress") && load(p_eglGetError, "eglGetError") &&
           load(p_eglDestroySurface, "eglDestroySurface") && load(p_eglDestroyContext, "eglDestroyContext") &&
           load(p_eglTerminate, "eglTerminate") && load(p_eglQueryString, "eglQueryString") &&
           (load(p_eglGetPlatformDisplayEXT, "eglGetPlatformDisplayEXT", false) ||
            (p_eglGetPlatformDisplayEXT = reinterpret_cast<PFNEGLGETPLATFORMDISPLAYEXTPROC>(p_eglGetProcAddress("eglGetPlatformDisplayEXT")), true));
}

bool openFb() {
    if (g_fbFd >= 0) return true;
    const char* dev = std::getenv("FABLE_FBDEV");
    g_fbFd = ::open(dev ? dev : "/dev/fb0", O_RDWR);
    if (g_fbFd < 0) return false;
    if (ioctl(g_fbFd, FBIOGET_VSCREENINFO, &g_var) != 0 || ioctl(g_fbFd, FBIOGET_FSCREENINFO, &g_fix) != 0 || !g_var.xres || !g_var.yres) {
        ::close(g_fbFd);
        g_fbFd = -1;
        return false;
    }
    return true;
}

// The EGL display: the default one, then the platforms the driver advertises.
bool initDisplay() {
    const char* client = p_eglQueryString(EGL_NO_DISPLAY, EGL_EXTENSIONS);
    log("gles: EGL client extensions: %s", client ? client : "(none)");
    EGLint major = 0, minor = 0;
    g_display = p_eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (g_display != EGL_NO_DISPLAY && p_eglInitialize(g_display, &major, &minor)) {
        log("gles: EGL %d.%d on the default display", major, minor);
        return true;
    }
    log("gles: eglInitialize(default display) failed (0x%X)", p_eglGetError());
    if (p_eglGetPlatformDisplayEXT && client) {
        struct { const char* ext; EGLenum platform; const char* name; } platforms[] = {
            {"EGL_MESA_platform_surfaceless", 0x31DD /* EGL_PLATFORM_SURFACELESS_MESA */, "surfaceless"},
            {"EGL_EXT_platform_device", 0x313F /* EGL_PLATFORM_DEVICE_EXT */, "device"},
        };
        for (const auto& p : platforms) {
            if (!std::strstr(client, p.ext)) continue;
            g_display = p_eglGetPlatformDisplayEXT(p.platform, EGL_DEFAULT_DISPLAY, nullptr);
            if (g_display != EGL_NO_DISPLAY && p_eglInitialize(g_display, &major, &minor)) {
                log("gles: EGL %d.%d on the %s platform", major, minor, p.name);
                return true;
            }
            log("gles: eglInitialize(%s platform) failed (0x%X)", p.name, p_eglGetError());
        }
    }
    g_display = EGL_NO_DISPLAY;
    return false;
}

EGLConfig chooseConfig(EGLint surfaceType) {
    EGLConfig configs[32];
    EGLint n = 0;
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
    }
    if (n == 0) return nullptr;
    EGLConfig config = configs[0];
    for (EGLint i = 0; i < n; ++i) {  // prefer 8 bits per channel (Mesa lists 10-bit first)
        EGLint red = 0;
        p_eglGetConfigAttrib(g_display, configs[i], EGL_RED_SIZE, &red);
        if (red == 8) { config = configs[i]; break; }
    }
    EGLint r = 0, g = 0, b = 0, a = 0;
    p_eglGetConfigAttrib(g_display, config, EGL_RED_SIZE, &r), p_eglGetConfigAttrib(g_display, config, EGL_GREEN_SIZE, &g);
    p_eglGetConfigAttrib(g_display, config, EGL_BLUE_SIZE, &b), p_eglGetConfigAttrib(g_display, config, EGL_ALPHA_SIZE, &a);
    log("gles: EGL config R%dG%dB%dA%d for a %s surface", r, g, b, a, surfaceType == EGL_WINDOW_BIT ? "window" : "pbuffer");
    return config;
}

bool makeContext(EGLConfig config) {
    const EGLint ctx[] = {EGL_CONTEXT_CLIENT_VERSION, 3, EGL_NONE};
    g_context = p_eglCreateContext(g_display, config, EGL_NO_CONTEXT, ctx);
    if (g_context == EGL_NO_CONTEXT) { log("gles: eglCreateContext (ES 3) failed (0x%X)", p_eglGetError()); return false; }
    if (!p_eglMakeCurrent(g_display, g_surface, g_surface, g_context)) {
        log("gles: eglMakeCurrent failed (0x%X)", p_eglGetError());
        p_eglDestroyContext(g_display, g_context);
        g_context = EGL_NO_CONTEXT;
        return false;
    }
    return true;
}

bool trySurfaces(bool wantWindow, bool wantPbuffer) {
    if (wantWindow) {
        if (EGLConfig c = chooseConfig(EGL_WINDOW_BIT)) {
            g_window.width = static_cast<unsigned short>(g_w), g_window.height = static_cast<unsigned short>(g_h);
            g_surface = p_eglCreateWindowSurface(g_display, c, reinterpret_cast<EGLNativeWindowType>(&g_window), nullptr);
            if (g_surface == EGL_NO_SURFACE) log("gles: eglCreateWindowSurface(fbdev window) failed (0x%X)", p_eglGetError());
            else if (makeContext(c)) { log("gles: framebuffer window surface %dx%d", g_w, g_h); g_surfaceIsWindow = true; return true; }
            else { p_eglDestroySurface(g_display, g_surface); g_surface = EGL_NO_SURFACE; }
        } else {
            log("gles: no EGL config for a window surface");
        }
    }
    if (wantPbuffer) {
        if (EGLConfig c = chooseConfig(EGL_PBUFFER_BIT)) {
            const EGLint pb[] = {EGL_WIDTH, g_w, EGL_HEIGHT, g_h, EGL_NONE};
            g_surface = p_eglCreatePbufferSurface(g_display, c, pb);
            if (g_surface == EGL_NO_SURFACE) log("gles: eglCreatePbufferSurface failed (0x%X)", p_eglGetError());
            else if (makeContext(c)) { log("gles: pbuffer surface %dx%d", g_w, g_h); g_surfaceIsWindow = false; return true; }
            else { p_eglDestroySurface(g_display, g_surface); g_surface = EGL_NO_SURFACE; }
        } else {
            log("gles: no EGL config for a pbuffer surface");
        }
    }
    return false;
}

bool mapFb() {
    if (!openFb()) { log("gles: cannot open the framebuffer for the copy presenter"); return false; }
    g_fbLen = g_fix.smem_len;
    g_fbMem = static_cast<unsigned char*>(mmap(nullptr, g_fbLen, PROT_READ | PROT_WRITE, MAP_SHARED, g_fbFd, 0));
    if (g_fbMem == MAP_FAILED) { g_fbMem = nullptr; log("gles: mmap(framebuffer) failed"); return false; }
    log("gles: framebuffer %ux%u (virtual %ux%u, offset %u,%u) %u bpp, line %u bytes: presenting by copy", g_var.xres, g_var.yres,
        g_var.xres_virtual, g_var.yres_virtual, g_var.xoffset, g_var.yoffset, g_var.bits_per_pixel, g_fix.line_length);
    return true;
}

}  // namespace

bool screenSize(int& w, int& h) {
    if (const char* s = std::getenv("FABLE_SCREEN")) return std::sscanf(s, "%dx%d", &w, &h) == 2;
    if (!openFb()) return false;
    w = static_cast<int>(g_var.xres), h = static_cast<int>(g_var.yres);
    return true;
}

bool open(int& width, int& height) {
    const char* mode = std::getenv("FABLE_GL_WINDOW");  // window | pbuffer (no presentation) | copy
    const bool onlyPbuffer = mode && std::strcmp(mode, "pbuffer") == 0;
    const bool onlyCopy = mode && std::strcmp(mode, "copy") == 0;
    const bool onlyWindow = mode && std::strcmp(mode, "window") == 0;

    g_w = 720, g_h = 480;
    if (!screenSize(g_w, g_h) && !onlyPbuffer) log("gles: /dev/fb0 size unknown, assuming %dx%d", g_w, g_h);
    width = g_w, height = g_h;

    // candidate EGL/GLES libraries: the OS's by soname, then the places Mali blobs are put
    struct Candidate { std::string egl, gles; };
    std::vector<Candidate> candidates;
    if (const char* e = std::getenv("FABLE_EGL_LIB")) {
        std::string s = e;
        const size_t colon = s.find(':');
        candidates.push_back({s.substr(0, colon), colon == std::string::npos ? s : s.substr(colon + 1)});
    }
    for (const char* dir : {"", "/usr/lib/", "/usr/lib/aarch64-linux-gnu/", "/usr/lib64/", "/lib/", "/usr/lib/mali/", "/usr/lib/egl/",
                            "/usr/local/lib/", "/opt/mali/lib/", "/usr/lib/arm-linux-gnueabihf/"}) {
        const std::string d = dir;
        candidates.push_back({d + "libEGL.so.1", d + "libGLESv2.so.2"});
        candidates.push_back({d + "libmali.so", d + "libmali.so"});
        candidates.push_back({d + "libMali.so", d + "libMali.so"});
        candidates.push_back({d + "libEGL.so", d + "libGLESv2.so"});
    }
    for (const Candidate& c : candidates) {
        if (!c.egl.empty() && c.egl[0] == '/' && access(c.egl.c_str(), R_OK) != 0) continue;
        g_egl = dlopen(c.egl.c_str(), RTLD_NOW | RTLD_LOCAL);
        if (!g_egl) continue;
        g_gles = c.gles == c.egl ? g_egl : dlopen(c.gles.c_str(), RTLD_NOW | RTLD_LOCAL);
        if (!g_gles) { log("gles: %s loaded but not %s", c.egl.c_str(), c.gles.c_str()); dlclose(g_egl); g_egl = nullptr; continue; }
        log("gles: trying %s + %s", c.egl.c_str(), c.gles.c_str());
        if (loadEgl()) {
            p_eglBindAPI(EGL_OPENGL_ES_API);
            if (initDisplay()) {
                log("gles: EGL vendor %s | version %s", p_eglQueryString(g_display, EGL_VENDOR), p_eglQueryString(g_display, EGL_VERSION));
                const bool wantWindow = !onlyPbuffer && !onlyCopy;
                const bool wantPbuffer = !onlyWindow;
                if (trySurfaces(wantWindow, wantPbuffer)) {
                    // a pbuffer shows nothing by itself: present by copying into the framebuffer
                    g_copy = !g_surfaceIsWindow && !onlyPbuffer;
                    if (g_copy && !mapFb()) { log("gles: no framebuffer to present on"); close(); return false; }
                    if (g_copy) {
                        p_glReadPixels = reinterpret_cast<PFNGLREADPIXELSPROC>(proc("glReadPixels"));
                        p_glBindFramebuffer = reinterpret_cast<PFNGLBINDFRAMEBUFFERPROC>(proc("glBindFramebuffer"));
                        p_glFinish = reinterpret_cast<PFNGLFINISHPROC>(proc("glFinish"));
                        if (!p_glReadPixels || !p_glBindFramebuffer || !p_glFinish) { log("gles: no glReadPixels"); close(); return false; }
                        g_readback.resize(static_cast<size_t>(g_w) * g_h * 4);
                    }
                    return true;
                }
                p_eglTerminate(g_display);
                g_display = EGL_NO_DISPLAY;
            }
        }
        if (g_gles != g_egl) dlclose(g_gles);
        dlclose(g_egl);
        g_egl = g_gles = nullptr;
    }
    log("gles: no EGL library on this system produced a usable OpenGL ES 3 context (FABLE_EGL_LIB=<libEGL>:<libGLESv2> names one)");
    return false;
}

void* proc(const char* name) {
    void* p = g_gles ? dlsym(g_gles, name) : nullptr;
    if (!p && p_eglGetProcAddress) p = reinterpret_cast<void*>(p_eglGetProcAddress(name));
    return p;
}

void swap() {
    if (g_display == EGL_NO_DISPLAY || g_surface == EGL_NO_SURFACE) return;
    if (!g_copy) { p_eglSwapBuffers(g_display, g_surface); return; }
    // read the default framebuffer (bottom row first) and copy it into the visible framebuffer
    p_glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
    p_glReadPixels(0, 0, g_w, g_h, GL_RGBA, GL_UNSIGNED_BYTE, g_readback.data());
    const unsigned bpp = g_var.bits_per_pixel;
    const unsigned rows = std::min<unsigned>(static_cast<unsigned>(g_h), g_var.yres), cols = std::min<unsigned>(static_cast<unsigned>(g_w), g_var.xres);
    for (unsigned y = 0; y < rows; ++y) {
        const unsigned char* src = &g_readback[(static_cast<size_t>(g_h - 1 - y)) * g_w * 4];
        unsigned char* dst = g_fbMem + static_cast<size_t>(y + g_var.yoffset) * g_fix.line_length + static_cast<size_t>(g_var.xoffset) * (bpp / 8);
        if (dst + cols * (bpp / 8) > g_fbMem + g_fbLen) break;
        if (bpp == 32) {
            // the common layout: blue at offset 0, red at 16 (BGRX); honour the var info otherwise
            const bool bgrx = g_var.blue.offset == 0 && g_var.red.offset == 16;
            uint32_t* d = reinterpret_cast<uint32_t*>(dst);
            for (unsigned x = 0; x < cols; ++x, src += 4) {
                const uint32_t r = src[0], g = src[1], b = src[2];
                d[x] = bgrx ? (r << 16) | (g << 8) | b | 0xFF000000u : (r << g_var.red.offset) | (g << g_var.green.offset) | (b << g_var.blue.offset);
            }
        } else if (bpp == 16) {
            uint16_t* d = reinterpret_cast<uint16_t*>(dst);
            for (unsigned x = 0; x < cols; ++x, src += 4) d[x] = static_cast<uint16_t>(((src[0] >> 3) << 11) | ((src[1] >> 2) << 5) | (src[2] >> 3));
        } else if (bpp == 24) {
            for (unsigned x = 0; x < cols; ++x, src += 4) dst[x * 3] = src[2], dst[x * 3 + 1] = src[1], dst[x * 3 + 2] = src[0];
        }
    }
    // pan to the written area (no-op on single-buffered consoles)
    if (g_var.yres_virtual >= g_var.yres * 2 && std::getenv("FABLE_FB_PAN")) ioctl(g_fbFd, FBIOPAN_DISPLAY, &g_var);
}

void setSwapInterval(int interval) {
    if (g_display != EGL_NO_DISPLAY && !g_copy) p_eglSwapInterval(g_display, interval);
}

void close() {
    if (g_fbMem) { munmap(g_fbMem, g_fbLen); g_fbMem = nullptr; }
    if (g_fbFd >= 0) { ::close(g_fbFd); g_fbFd = -1; }
    if (g_display == EGL_NO_DISPLAY) return;
    p_eglMakeCurrent(g_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    if (g_context != EGL_NO_CONTEXT) p_eglDestroyContext(g_display, g_context);
    if (g_surface != EGL_NO_SURFACE) p_eglDestroySurface(g_display, g_surface);
    p_eglTerminate(g_display);
    g_display = EGL_NO_DISPLAY, g_surface = EGL_NO_SURFACE, g_context = EGL_NO_CONTEXT;
}

int probe() {
    std::fprintf(stderr, "gl probe: EGL on the framebuffer\n");
    int w = 0, h = 0;
    if (!open(w, h)) { std::fprintf(stderr, "gl probe: FAILED (the log above says where)\n"); return 1; }
    auto clearColor = reinterpret_cast<PFNGLCLEARCOLORPROC>(proc("glClearColor"));
    auto clear = reinterpret_cast<PFNGLCLEARPROC>(proc("glClear"));
    auto getString = reinterpret_cast<PFNGLGETSTRINGPROC>(proc("glGetString"));
    if (!clearColor || !clear || !getString) { std::fprintf(stderr, "gl probe: no GL entry points\n"); return 1; }
    std::fprintf(stderr, "gl probe: GL %s | %s | %s\n", getString(GL_VERSION), getString(GL_RENDERER), getString(GL_VENDOR));
    std::fprintf(stderr, "gl probe: %dx%d, presenting by %s; the screen should show red, green, blue for a second each\n", w, h, g_copy ? "framebuffer copy" : "eglSwapBuffers");
    const float colors[3][3] = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
    for (int c = 0; c < 3; ++c)
        for (int i = 0; i < 30; ++i) {
            clearColor(colors[c][0], colors[c][1], colors[c][2], 1);
            clear(GL_COLOR_BUFFER_BIT);
            swap();
            usleep(33000);
        }
    close();
    std::fprintf(stderr, "gl probe: done\n");
    return 0;
}

}  // namespace host::gles::fb
