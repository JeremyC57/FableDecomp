// NParticleEngine::Uninitialise() -- release the particle engine's singletons (emitter manager, emitter database, particle-system-component manager): drop each global counted pointer's reference, destroying the object on the last one, and null the pointer
// Leaf: ?Uninitialise@NParticleEngine@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AEBEA0, 163 bytes, __fastcall, no arguments. Caller 0x0041769F (engine shut-down).
// Identity: FableWin.pdb `?Uninitialise@NParticleEngine@@YIXXZ` (the sibling Initialise 0x00AEBE20); globals PEmitterManager
//   0x013D2E1C, PEmitterDatabase 0x013D2E14, PPSCManager 0x013D2E24 (FableWin.pdb dynamic initializers).
// Callees: each count block's per-type destroy function (`call [block+4]`, object in ecx), operator delete 0x00BFE9BC.
// Layout: CCountedPointer = {T* Data, count block*}; count block = {RefCount, __fastcall destroy function, object}.
// Shape: three inline CCountedPointer::ReleaseData() calls (`if (Block) { --Block->RefCount; if (Block->RefCount == 0) {
//   Block->DeleteData(Block->Data); delete Block; } } Block = 0; Data = 0;`) -- the global's block re-read after every
//   store through it; one zero register (esi) serves all tests and stores.
struct CCountBlock {
    long RefCount;                                          // +0x0
    void (__fastcall* DeleteData)(void* data);              // +0x4
    void* Data;                                             // +0x8
};

template <class T> class CCountedPointer {
public:
    T*           Data;                                      // +0x0
    CCountBlock* Block;                                     // +0x4
    void ReleaseData()
    {
        if (Block) {
            --Block->RefCount;
            if (Block->RefCount == 0) {
                Block->DeleteData(Block->Data);
                delete Block;
            }
        }
        Block = 0;
        Data = 0;
    }
};

namespace NParticleEngine {
class CParticleEmitterDatabase;
class CParticleEmitterManager;
class CPSCManager;

extern CCountedPointer<CParticleEmitterDatabase> PEmitterDatabase;   // 0x013D2E14
extern CCountedPointer<CParticleEmitterManager>  PEmitterManager;    // 0x013D2E1C
extern CCountedPointer<CPSCManager>              PPSCManager;        // 0x013D2E24

void __fastcall Uninitialise()
{
    PEmitterManager.ReleaseData();
    PEmitterDatabase.ReleaseData();
    PPSCManager.ReleaseData();
}
}