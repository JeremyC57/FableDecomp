// _global::AtexitDtor_CShaderRenderManager_at_013bc470_0122a4c0() -- atexit destructor thunk of CShaderRenderManager::SingletonInstance
// NF52 (round 7): the object is CShaderRenderManager::SingletonInstance (?SingletonInstance@CShaderRenderManager@@0V1@A, 0x013BC470,
// engine/CShaderRenderManager.h Get()) and the jump target 0x0098ABF0 is ??1CShaderRenderManager@@UAE@XZ (catalog row, the
// header's virtual destructor; a qualified call is direct). Retail: mov ecx,0x013BC470 ; jmp 0x0098ABF0 -- the atexit destructor
// the 0x01223D00 initializer registers. Was the made-up AutoTinyThunkTarget g_AutoTinyThunkObject_013bc470 / Invoke_0098abf0.
// Leaf: ?GlobalMethodTailThunk_0122a4c0@@YIXXZ
#include "engine/CShaderRenderManager.h"
void __fastcall GlobalMethodTailThunk_0122a4c0()
{
    CShaderRenderManager::Get().CShaderRenderManager::~CShaderRenderManager();
}
