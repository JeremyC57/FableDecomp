// NParticleEngine::Initialise() -- create the particle engine's singletons: the emitter database, the emitter manager and the particle-system-component manager, each adopted by its global counted pointer
// Leaf: ?Initialise@NParticleEngine@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AEBE20, 122 bytes, __fastcall, no arguments. Caller 0x004174FA (engine start-up).
// Identity: FableWin.pdb `?Initialise@NParticleEngine@@YIXXZ` (the sibling Uninitialise 0x00AEBEA0); globals PEmitterDatabase
//   0x013D2E14, PEmitterManager 0x013D2E1C, PPSCManager 0x013D2E24 (FableWin.pdb dynamic initializers, in address order).
// Callees: operator new 0x00BFEA1A (0x4c / 0x1018 / 0x10 bytes), CParticleEmitterDatabase() 0x00AE8BF0 (landed),
//   CParticleEmitterManager() 0x00AEBA90, CPSCManager() 0x00AF2970 (landed), and the three counted pointers' out-of-line
//   Reset(T*) 0x00AECCC0 / 0x00AECAC0 / 0x00AECC30 (landed).
// Shape: `P.Reset(new T);` three times -- each new-expression keeps its null test (`test eax,eax ; je ; ... xor eax,eax`);
//   VC7.1 tail-duplicates the last Reset call and `ret` into both arms of the final null test.
namespace NParticleEngine {
class CParticleEmitterDatabase {
public:
    unsigned char _body[0x4c];
    CParticleEmitterDatabase();                             // 0x00AE8BF0
};
class CParticleEmitterManager {
public:
    unsigned char _body[0x1018];
    CParticleEmitterManager();                              // 0x00AEBA90
};
class CPSCManager {
public:
    unsigned char _body[0x10];
    CPSCManager();                                          // 0x00AF2970
};
}

struct CCountBlock;

template <class T> class CCountedPointer {
public:
    T*           Data;                                      // +0x0
    CCountBlock* Block;                                     // +0x4
    void Reset(T* data);                                    // out of line per instantiation
};

namespace NParticleEngine {
extern CCountedPointer<CParticleEmitterDatabase> PEmitterDatabase;   // 0x013D2E14 (Reset 0x00AECCC0)
extern CCountedPointer<CParticleEmitterManager>  PEmitterManager;    // 0x013D2E1C (Reset 0x00AECAC0)
extern CCountedPointer<CPSCManager>              PPSCManager;        // 0x013D2E24 (Reset 0x00AECC30)

void __fastcall Initialise()
{
    PEmitterDatabase.Reset(new CParticleEmitterDatabase);
    PEmitterManager.Reset(new CParticleEmitterManager);
    PPSCManager.Reset(new CPSCManager);
}
}