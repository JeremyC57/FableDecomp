// Android glue: the Vulkan driver choice, logging to the data directory, and the JNI entry
// the on-screen touch controls use. main_posix.cpp calls androidInit() first thing.
#include <windows.h>
#include <xinput.h>

#include <android/log.h>
#include <dlfcn.h>
#include <jni.h>
#include <unistd.h>

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <string>
#include <thread>

#include <SDL.h>
#include <adrenotools/driver.h>
#include <vulkan/vulkan.h>

#include <atomic>
#include <functional>

namespace w32 {
void setVirtualPad(bool active, const XINPUT_GAMEPAD& g);
void addSdlEventHook(std::function<void(const SDL_Event&)> fn);
}

namespace {

// stdout/stderr go nowhere on Android: copy them into logcat and FableRecomp.log.
void redirectOutput(const std::string& data) {
    static FILE* file = std::fopen((data + "/FableRecomp.log").c_str(), "w");
    int fds[2];
    if (pipe(fds) != 0) return;
    dup2(fds[1], 1);
    dup2(fds[1], 2);
    close(fds[1]);
    std::setvbuf(stdout, nullptr, _IOLBF, 0);
    std::setvbuf(stderr, nullptr, _IONBF, 0);
    std::thread([fd = fds[0]] {
        char buf[1024];
        std::string line;
        for (ssize_t n; (n = read(fd, buf, sizeof buf)) > 0;) {
            if (file) std::fwrite(buf, 1, static_cast<size_t>(n), file), std::fflush(file);
            line.append(buf, static_cast<size_t>(n));
            for (size_t nl; (nl = line.find('\n')) != std::string::npos; line.erase(0, nl + 1))
                __android_log_write(ANDROID_LOG_INFO, "FableRecomp", line.substr(0, nl).c_str());
        }
    }).detach();
}

// A custom driver (e.g. Mesa Turnip) imported by the launcher: loaded through adrenotools,
// which gets it past the linker namespace that hides vendor libraries from apps. DXVK then
// takes its vkGetInstanceProcAddr instead of opening the system libvulkan.so.
PFN_vkGetInstanceProcAddr loadVulkanDriver() {
    const char* dir = std::getenv("FABLE_VK_DRIVER_DIR");
    const char* lib = std::getenv("FABLE_VK_DRIVER_LIB");
    const char* hooks = std::getenv("FABLE_NATIVE_LIB_DIR");
    if (!dir || !*dir || !lib || !*lib || !hooks) {
        std::printf("Vulkan: system driver\n");
        return nullptr;
    }
    const char* tmp = std::getenv("FABLE_TMP_DIR");
    std::string driverDir = dir, hookDir = hooks;
    if (driverDir.back() != '/') driverDir += '/';
    if (hookDir.back() != '/') hookDir += '/';
    void* h = adrenotools_open_libvulkan(RTLD_NOW, ADRENOTOOLS_DRIVER_CUSTOM, tmp, hookDir.c_str(), driverDir.c_str(), lib,
                                         nullptr, nullptr);
    if (!h) {
        std::printf("Vulkan: custom driver %s%s failed to load (%s); using the system driver\n", driverDir.c_str(), lib,
                    dlerror());
        return nullptr;
    }
    void* gipa = dlsym(h, "vkGetInstanceProcAddr");
    if (!gipa) {
        std::printf("Vulkan: custom driver %s has no vkGetInstanceProcAddr; using the system driver\n", lib);
        return nullptr;
    }
    char hex[32];
    std::snprintf(hex, sizeof hex, "%llx", static_cast<unsigned long long>(reinterpret_cast<uintptr_t>(gipa)));
    setenv("DXVK_VK_GET_INSTANCE_PROC_ADDR", hex, 1);
    std::printf("Vulkan: custom driver %s%s\n", driverDir.c_str(), lib);
    return reinterpret_cast<PFN_vkGetInstanceProcAddr>(gipa);
}

// Whether the GPU has shaderInt64, which DXVK 3.x requires. Qualcomm's own Adreno driver
// lacks it; DXVK 2.6 (shipped as libdxvk_d3d9_v2.so) runs without it.
bool hasShaderInt64(PFN_vkGetInstanceProcAddr gipa) {
    if (!gipa) {
        void* vk = dlopen("libvulkan.so", RTLD_NOW | RTLD_LOCAL);
        if (vk) gipa = reinterpret_cast<PFN_vkGetInstanceProcAddr>(dlsym(vk, "vkGetInstanceProcAddr"));
        if (!gipa) return true;  // let DXVK report the problem
    }
    auto createInstance = reinterpret_cast<PFN_vkCreateInstance>(gipa(nullptr, "vkCreateInstance"));
    if (!createInstance) return true;
    VkApplicationInfo app{VK_STRUCTURE_TYPE_APPLICATION_INFO};
    app.apiVersion = VK_API_VERSION_1_1;
    VkInstanceCreateInfo ci{VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO};
    ci.pApplicationInfo = &app;
    VkInstance inst = VK_NULL_HANDLE;
    if (createInstance(&ci, nullptr, &inst) != VK_SUCCESS) return true;
    auto enumerate = reinterpret_cast<PFN_vkEnumeratePhysicalDevices>(gipa(inst, "vkEnumeratePhysicalDevices"));
    auto features = reinterpret_cast<PFN_vkGetPhysicalDeviceFeatures>(gipa(inst, "vkGetPhysicalDeviceFeatures"));
    auto destroy = reinterpret_cast<PFN_vkDestroyInstance>(gipa(inst, "vkDestroyInstance"));
    bool any = false;
    uint32_t n = 0;
    if (enumerate && features && enumerate(inst, &n, nullptr) == VK_SUCCESS && n) {
        std::vector<VkPhysicalDevice> devs(n);
        enumerate(inst, &n, devs.data());
        for (VkPhysicalDevice d : devs) {
            VkPhysicalDeviceFeatures f{};
            features(d, &f);
            any |= f.shaderInt64 == VK_TRUE;
        }
    } else {
        any = true;
    }
    if (destroy) destroy(inst, nullptr);
    return any;
}

}  // namespace

// The on-screen keyboard is shown and hidden on the SDL thread, through a user event.
std::atomic<Uint32> g_keyboardEvent{0};

void androidInit(const std::string& data) {
    redirectOutput(data);
    const Uint32 ev = SDL_RegisterEvents(1);
    if (ev != static_cast<Uint32>(-1)) {
        g_keyboardEvent = ev;
        w32::addSdlEventHook([ev](const SDL_Event& e) {
            if (e.type != ev) return;
            if (SDL_IsTextInputActive()) SDL_StopTextInput();
            else SDL_StartTextInput();
        });
    }
    setenv("DXVK_WSI_DRIVER", "SDL2", 0);
    setenv("DXVK_LOG_PATH", data.c_str(), 0);
    PFN_vkGetInstanceProcAddr gipa = loadVulkanDriver();
    // FABLE_DXVK (launcher setting): "3" or "2" forces a DXVK build, anything else picks one.
    const char* pick = std::getenv("FABLE_DXVK");
    const bool v2 = pick && !std::strcmp(pick, "2") ? true
                  : pick && !std::strcmp(pick, "3") ? false
                  : !hasShaderInt64(gipa);
    setenv("FABLE_D3D9_LIBRARY", v2 ? "libdxvk_d3d9_v2.so" : "libdxvk_d3d9.so", 1);
    std::printf("Direct3D 9: DXVK %s\n", v2 ? "2.6 (driver without shaderInt64)" : "3.x");
}

// org.fablerecomp.TouchControls.nativeSetPad: the overlay's state, sent on every change.
extern "C" JNIEXPORT void JNICALL Java_org_fablerecomp_TouchControls_nativeSetPad(
    JNIEnv*, jclass, jboolean active, jint buttons, jfloat lx, jfloat ly, jfloat rx, jfloat ry, jfloat lt, jfloat rt) {
    auto axis = [](float v) { return static_cast<SHORT>(std::lround(std::clamp(v, -1.0f, 1.0f) * 32767.0f)); };
    auto trig = [](float v) { return static_cast<BYTE>(std::lround(std::clamp(v, 0.0f, 1.0f) * 255.0f)); };
    XINPUT_GAMEPAD g{};
    g.wButtons = static_cast<WORD>(buttons);
    g.sThumbLX = axis(lx);
    g.sThumbLY = axis(ly);
    g.sThumbRX = axis(rx);
    g.sThumbRY = axis(ry);
    g.bLeftTrigger = trig(lt);
    g.bRightTrigger = trig(rt);
    w32::setVirtualPad(active, g);
}

namespace host::pad {
void touchMotion(int dx, int dy);
void touchButton(int button, bool down);
}  // namespace host::pad

// org.fablerecomp.TouchControls touchpad: drags move the mouse (camera / menu cursor), taps click.
extern "C" JNIEXPORT void JNICALL Java_org_fablerecomp_TouchControls_nativeMouseMotion(JNIEnv*, jclass, jint dx, jint dy) {
    host::pad::touchMotion(dx, dy);
}
extern "C" JNIEXPORT void JNICALL Java_org_fablerecomp_TouchControls_nativeMouseButton(JNIEnv*, jclass, jint button, jboolean down) {
    host::pad::touchButton(button, down);
}

// org.fablerecomp.TouchControls keyboard button: show or hide the on-screen keyboard.
extern "C" JNIEXPORT void JNICALL Java_org_fablerecomp_TouchControls_nativeToggleKeyboard(JNIEnv*, jclass) {
    if (const Uint32 ev = g_keyboardEvent.load()) {
        SDL_Event e{};
        e.type = ev;
        SDL_PushEvent(&e);
    }
}
