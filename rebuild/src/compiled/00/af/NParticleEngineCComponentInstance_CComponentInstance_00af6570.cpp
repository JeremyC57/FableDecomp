// NParticleEngine::CComponentInstance::~CComponentInstance() -- implicit destructor: release the counted Component (+0x04), then CBaseClass::~CBaseClass by tail jump
// Leaf: ??1CComponentInstance@NParticleEngine@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AF6570, 60 bytes, thiscall, `jmp` tail; no call or address reference (an unreferenced
//   COMDAT kept by the linker, no /OPT:REF).
// Identity (proven): it sits right after the landed CComponentInstance::CComponentInstance 0x00AF6550 (vtable 0x0129F478,
//   RTTI .?AVCComponentInstance@NParticleEngine@@ : CBaseClass) and before the landed ~pair<EPSCType, CComponentInstance>
//   0x00AF65B0, which inlines this same sequence at +4; the vtable's slot 0 ??_G 0x00AF6B90 inlines it too (release +0x04,
//   Data = Info = 0, call ~CBaseClass 0x0099A2E0). FableWin.pdb names ??1CComponentInstance@NParticleEngine@@UAE@XZ. The
//   ??_G stores no vtable: the class declares no destructor. Ego_r.pdb layout (20 bytes): vptr, Component
//   (CCountedPointer<NParticleEngine::CPSCBase>) +0x04, Enabled +0x0C, InstanceName +0x10. The manifest label
//   (_Dest_val<..pair<CCharString const,CCountedPointer<C3DSkeletalMorphTarget>>..>) is propagated and wrong.
// Callees: CCPPointerInfo::DeleteFunc through the block (Data in ecx), operator delete 0x00BFE9BC (the block),
//   CBaseClass::~CBaseClass 0x0099A2E0 (??1CBaseClass@@UAE@XZ, tail).
// Layout: no engine/CComponentInstance.h; the class is the Ego_r.pdb layout over engine/CBaseClass.h and
//   engine/CCountedPointer.h.
#include "engine/CCountedPointer.h"
#include "engine/CBaseClass.h"

namespace NParticleEngine {

class CPSCBase;

class CComponentInstance : public CBaseClass {              // Ego_r.pdb, 20 bytes
public:
    CCountedPointer<CPSCBase> Component;                    // +0x04
    bool                      Enabled;                      // +0x0C
    unsigned long             InstanceName;                 // +0x10
};

}

void EmitComponentInstanceDestructor(NParticleEngine::CComponentInstance* instance)   // emits the implicit ??1; not a retail function
{
    instance->NParticleEngine::CComponentInstance::~CComponentInstance();
}