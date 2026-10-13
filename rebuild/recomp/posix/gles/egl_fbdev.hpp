// An EGL window on the Linux framebuffer, for devices whose OS has no KMS/DRM (the Anbernic
// stock OS on the RG34XX SP: /dev/fb0 and Arm's fbdev Mali blob). SDL then runs on its dummy
// video driver for the window bookkeeping and input, and the GLES renderer presents here.
// The native window is the Mali fbdev convention: a struct with the surface's width and
// height. FABLE_GL_WINDOW=pbuffer makes it a pbuffer surface instead (tests on a PC without a
// framebuffer: nothing is shown, FABLE_GLES_DUMP captures the frames).
#pragma once

namespace host::gles::fb {
bool screenSize(int& width, int& height);  // /dev/fb0 (FABLE_SCREEN=WxH overrides)
bool open(int& width, int& height);  // libEGL + libGLESv2 from the OS, a current ES 3.x context
void* proc(const char* name);         // GL entry point
void swap();
void releaseCurrent();  // the creating thread gives the context up ...
bool makeCurrent();     // ... and the render thread takes it
void setSwapInterval(int interval);
void close();
int probe();  // --gl-probe: open, paint red/green/blue for a few seconds, report, exit code 0 on success
}
