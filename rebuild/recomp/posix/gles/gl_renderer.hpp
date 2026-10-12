// Direct3D 9 on OpenGL ES 3.0: the renderer for devices without Vulkan (the RG34XX SP's
// Mali-G31). The COM interfaces the game sees are implemented here directly; the guest
// proxies and the Lock staging in win/d3d9.cpp work unchanged on top of them.
#pragma once
#include "host.hpp"

#include <d3d9.h>

namespace host::gles {
IDirect3D9* createD3D9();
}
namespace w32 {
bool glesRenderer();  // w32/user.cpp: FABLE_RENDERER=gles
}
