// std::pair<NParticleEngine::EPSCType,NParticleEngine::CComponentInstance>::~pair() -- destroy a particle-system component slot: release the instance's counted component, then the CBaseClass root of the instance
// Leaf: ??1?$pair@W4EPSCType@NParticleEngine@@VCComponentInstance@2@@std@@QAE@XZ
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AF65B0, length 61 bytes, `jmp` tail. Callers: 0x00AF768D / 0x00AF784A in
// the vector insert helpers 0x00AF7570 / 0x00AF7730 of the CParticleSystem Components vector (the landed resize(n)
// 0x00AF7A10 names the element: pair<EPSCType, CComponentInstance>, vtable 0x0129F478, base CBaseClass). The manifest label
// (_Dest_val<allocator<pair<CCharString const,CCountedPointer<C3DSkeletalMorphTarget>>>>) is propagated and wrong.
// Layout: pair.first (EPSCType) +0; CComponentInstance +4 = vptr +0, Component (CCountedPointer<CPSCBase>) +4/+8,
// Enabled +0xc, InstanceName +0x10 (the landed 0x00AF7A10 model).
// Callees: CCPPointerInfo::DeleteFunc through the block (Data in ecx), operator delete @ 0x00BFE9BC (the block),
// CBaseClass::~CBaseClass @ 0x0099A2E0 (tail).
// Shape: the enum key needs no destruction, so Info is read straight off ecx and `&second` goes to esi; CComponentInstance
// declares no destructor (no vtable store), its counted pointer re-reads Info, the out-of-line root dtor is the tail jmp.
void __cdecl operator delete(void* p);                       // 0x00BFE9BC

#include "engine/CCountedPointer.h"

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0

namespace NParticleEngine {
enum EPSCType { PSC_NONE = 0 };
class CPSCBase;
class CComponentInstance : public CBaseClass {               // vtable 0x0129F478, 0x14 bytes
public:
    CCountedPointer<CPSCBase> Component;                     // +0x04
    bool                      Enabled;                       // +0x0c
    unsigned long             InstanceName;                  // +0x10
};
}

namespace std {
template<class T1, class T2> struct pair {
    T1 first;
    T2 second;
};
}

typedef std::pair<NParticleEngine::EPSCType, NParticleEngine::CComponentInstance> TComponentSlot;

void DestroyComponentSlot(TComponentSlot* slot)              // emits the implicit destructor; not part of the retail body
{
    slot->TComponentSlot::~TComponentSlot();
}