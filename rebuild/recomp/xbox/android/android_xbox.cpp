// Android glue for the Xbox build: logging into the data directory (and logcat), the custom
// Vulkan driver (adrenotools), and the JNI entries of the on-screen controls.
// The launcher (Java) passes its choices as environment variables before the game starts.
#include "../input.hpp"
#include "../settings.hpp"
#include "../xhost.hpp"

#include <android/log.h>
#include <dlfcn.h>
#include <jni.h>
#include <SDL.h>
#include <SDL_system.h>
#include <unistd.h>

#include <cstdio>
#include <cstdlib>
#include <string>
#include <thread>

#include <adrenotools/driver.h>

namespace xb {

// The game rebooted itself with launch data (loading a save from the pause menu): the activity
// arranges a new game process (GameActivity.relaunch), this one then exits.
void androidRelaunch() {
    JNIEnv* env = static_cast<JNIEnv*>(SDL_AndroidGetJNIEnv());
    jobject activity = static_cast<jobject>(SDL_AndroidGetActivity());
    if (!env || !activity) return;
    jclass cls = env->GetObjectClass(activity);
    if (jmethodID m = env->GetMethodID(cls, "relaunch", "()V")) env->CallVoidMethod(activity, m);
    if (env->ExceptionCheck()) env->ExceptionClear();
    env->DeleteLocalRef(cls);
    env->DeleteLocalRef(activity);
}

// stdout/stderr go nowhere on Android: copy them into logcat and <data>/FableXbox.log.
void androidInit() {
    const char* data = std::getenv("FABLE_XBOX_DATA");
    static FILE* file = data ? std::fopen((std::string(data) + "/FableXbox.log").c_str(), "w") : nullptr;
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
                __android_log_write(ANDROID_LOG_INFO, "FableXbox", line.substr(0, nl).c_str());
        }
    }).detach();
}

// A custom driver (e.g. Mesa Turnip) imported by the launcher, loaded through adrenotools,
// which gets it past the linker namespace that hides vendor libraries from apps. Returns the
// library handle for vk::Settings::driverHandle, or null for the system driver.
void* androidVulkanDriver() {
    const char* dir = std::getenv("FABLE_VK_DRIVER_DIR");
    const char* lib = std::getenv("FABLE_VK_DRIVER_LIB");
    const char* hooks = std::getenv("FABLE_NATIVE_LIB_DIR");
    if (!dir || !*dir || !lib || !*lib || !hooks) return nullptr;
    std::string driverDir = dir, hookDir = hooks;
    if (driverDir.back() != '/') driverDir += '/';
    if (hookDir.back() != '/') hookDir += '/';
    void* h = adrenotools_open_libvulkan(RTLD_NOW, ADRENOTOOLS_DRIVER_CUSTOM, std::getenv("FABLE_TMP_DIR"), hookDir.c_str(),
                                         driverDir.c_str(), lib, nullptr, nullptr);
    std::printf(h ? "Vulkan: custom driver %s%s\n" : "Vulkan: custom driver %s%s failed to load; using the system driver\n",
                driverDir.c_str(), lib);
    return h;
}

}  // namespace xb

extern "C" JNIEXPORT void JNICALL Java_org_fablexbox_TouchControls_nativeSetPad(JNIEnv*, jclass, jboolean active, jint buttons, jfloat lx,
                                                                                 jfloat ly, jfloat rx, jfloat ry, jfloat lt, jfloat rt) {
    xb::input::setVirtualPad(active, static_cast<uint32_t>(buttons), lx, ly, rx, ry, lt, rt);
}
extern "C" JNIEXPORT void JNICALL Java_org_fablexbox_TouchControls_nativeMouseMotion(JNIEnv*, jclass, jint dx, jint dy) {
    xb::input::touchLook(dx, dy);
}
extern "C" JNIEXPORT void JNICALL Java_org_fablexbox_TouchControls_nativeMouseButton(JNIEnv*, jclass, jint, jboolean) {}
extern "C" JNIEXPORT void JNICALL Java_org_fablexbox_TouchControls_nativeToggleKeyboard(JNIEnv*, jclass) {}

// Quick menu (GameActivity): frame statistics, options that apply while the game runs, and an
// on-demand profile (<data>/profile.txt).
extern "C" JNIEXPORT jfloatArray JNICALL Java_org_fablexbox_GameActivity_nativeStats(JNIEnv* env, jclass) {
    float v[4];
    xb::perfStatsEx(v);
    jfloatArray a = env->NewFloatArray(4);
    if (a) env->SetFloatArrayRegion(a, 0, 4, v);
    return a;
}
extern "C" JNIEXPORT void JNICALL Java_org_fablexbox_GameActivity_nativeSetOption(JNIEnv* env, jclass, jstring name, jboolean on) {
    const char* n = env->GetStringUTFChars(name, nullptr);
    const std::string key = n ? n : "";
    if (n) env->ReleaseStringUTFChars(name, n);
    if (key == "vibration") xb::settings().vibration = on;
    else if (key == "hud_corners") xb::settings().hudCorners = on;
    else if (key == "text_sharpen") xb::settings().textSharpen = on;
    else if (key == "occlusion") xb::settings().occlusion = on;
    else if (key == "shadow_bias_fix") xb::settings().shadowBiasFix = on;
}
extern "C" JNIEXPORT jstring JNICALL Java_org_fablexbox_GameActivity_nativeProfile(JNIEnv* env, jclass, jint seconds) {
    const char* data = std::getenv("FABLE_XBOX_DATA");
    const std::string path = std::string(data ? data : ".") + "/profile.txt";
    return xb::profilerCapture(0.0, seconds, path) ? env->NewStringUTF(path.c_str()) : nullptr;
}
