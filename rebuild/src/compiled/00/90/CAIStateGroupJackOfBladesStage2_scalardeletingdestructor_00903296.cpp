// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
#pragma optimize("s", on)
class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
// Naming (NF423 gen, 2026-11-25): the callee 0x00903046 now binds its PDB spelling ??1CAIStateGroup_JackOfBladesBase@@UAE@XZ (the T1 tracker target; the self row's
//   Leaf / PDB name sets), called qualified on a declaration of the class (no body, no layout); was the free stand-in ?CAIStateGroup_JackOfBladesStage2_Destruct@@YIXPAX@Z.
class CAIStateGroup_JackOfBladesBase { public: virtual ~CAIStateGroup_JackOfBladesBase(); };
void* __fastcall CAIStateGroup_JackOfBladesStage2_CompositeScalarDeletingDestructor(void* stateGroup, int, unsigned int flags)
{
    ((CBaseIntelligentPointer*)(static_cast<unsigned char*>(stateGroup) + 76))->CBaseIntelligentPointer::~CBaseIntelligentPointer();
    reinterpret_cast<CAIStateGroup_JackOfBladesBase*>(stateGroup)->CAIStateGroup_JackOfBladesBase::~CAIStateGroup_JackOfBladesBase();
    if (flags & 1)
        operator delete(stateGroup);
    return stateGroup;
}