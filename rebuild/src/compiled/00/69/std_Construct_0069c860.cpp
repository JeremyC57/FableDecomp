// std::_Construct(CGameCameraManager::CQuake*, const CGameCameraManager::CQuake&) -- placement copy of a camera quake: a trivially copyable 0x30-byte record (eleven dwords and the Global flag)
// Retail: Fable.exe @ 0x0069C860, 75 bytes; __fastcall-shaped STLport helper (ecx = element slot, edx = source element).
// Callees: none (inline trivial copy).
// Leaf: _Construct
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
#include "fable_compiler.h"
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (STLport <new>)
namespace N0069c860 {
#include "engine/C3DVector.h"
struct CQuake {                                  // CGameCameraManager::CQuake (PDB, sizeof 0x30)
    C3DVector Position;                          // +0x00
    C3DVector Axis;                              // +0x0C
    long      Frame;                             // +0x18
    float     Amplitude;                         // +0x1C
    float     Duration;                          // +0x20
    float     Frequency;                         // +0x24
    float     Phase;                             // +0x28
    bool      Global;                            // +0x2C
    FABLE_FORCEINLINE CQuake(const CQuake& o)        // the implicit memberwise copy, spelled out so VC7.1 inlines it into _Construct
        : Position(o.Position), Axis(o.Axis), Frame(o.Frame), Amplitude(o.Amplitude), Duration(o.Duration),
          Frequency(o.Frequency), Phase(o.Phase), Global(o.Global) { }
};

// STLport _Construct: placement-copy one element (null-checked placement new, then the implicit copy constructor)
void __fastcall _Construct(CQuake* p, const CQuake& value)
{
    if (p != 0)
        new (p) CQuake(value);
}
}