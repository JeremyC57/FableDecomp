// A census of the game's Direct3D 9 use (what a renderer of our own has to provide):
// FABLE_D3D_CENSUS=<dir> wraps the device, counts calls, records distinct states and
// formats and dumps every shader's bytecode to <dir>. Report on exit and every 2000 frames.
#pragma once
#include "host.hpp"

#include <d3d9.h>

namespace host::census {
IDirect3DDevice9* wrap(IDirect3DDevice9* inner);  // returns inner when the census is off
}
