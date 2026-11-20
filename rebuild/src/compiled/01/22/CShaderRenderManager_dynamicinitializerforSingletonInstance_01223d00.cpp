// CShaderRenderManager::`dynamic initializer for 'SingletonInstance'' -- default-construct the shader render manager singleton at 0x013BC470 out of line, then register its atexit destructor 0x0122A4C0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x01223D00, oracle length 22 bytes.
// Leaf: _$E1
// Retail bytes:  mov ecx, 0x13bc470 ; call 0x98ab00 ; push 0x122a4c0 ; call 0x4012bc ; pop ecx ; ret
// Identity (NF151, q4669): the static is the private ?SingletonInstance@CShaderRenderManager@@0V1@A (Ego_d.pdb PUB32; Ego_r.pdb 0x0101A770 /
//   FableWin.pdb 0x0400AE30 `dynamic initializer for 'CShaderRenderManager::SingletonInstance'' and their `dynamic atexit destructor`
//   0x0101DF60 / 0x04012790 = retail 0x0122A4C0, the landed NF52 row; ?Get@CShaderRenderManager@@SIAAV1@XZ returns it). 41 landed rows already
//   bind that spelling. Was the descriptive global `g_CShaderRenderManager_013bc470` on a local CShaderRenderManager model; now the static is
//   defined on the shared engine/CShaderRenderManager.h (constructor 0x0098AB00 ??0CShaderRenderManager@@QAE@XZ, _atexit 0x004012BC), the
//   compiler emitting the initializer and its atexit destructor (the NF92 0x0121A440 form).
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CVERTEXSHADERCONSTANTLAYOUT
#include "engine/CShaderRenderManager.h"

CShaderRenderManager CShaderRenderManager::SingletonInstance;   // 0x013BC470
