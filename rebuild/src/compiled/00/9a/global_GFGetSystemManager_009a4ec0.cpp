// GFGetSystemManager() -- returns the address of the engine's static system manager object GSystemManager (0x013CA618)
// Leaf: ?GFGetSystemManager@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009A4EC0, 6 bytes (`mov eax, offset GSystemManager; ret`).
// Identity: the PDB name sets hold ?GFGetSystemManager@@YIPAVCSystemManager@@XZ (__fastcall) and the
// `dynamic initializer` / `dynamic atexit destructor` for 'GSystemManager'; the object at 0x013CA618 is destroyed
// by the atexit thunk 0x0122A680 calling CSystemManager::~CSystemManager 0x009A5800.  The old source returned the
// constant address (no relocation) under a Leaf naming no symbol.
class CSystemManager;
extern CSystemManager GSystemManager;                   // 0x013CA618

CSystemManager* __fastcall GFGetSystemManager()
{
    return &GSystemManager;
}
