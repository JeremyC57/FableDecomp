// CMessageEvent::GetPTarget<CThingCreatureBase>() const -- the message's target thing when it is alive in the pointer and is a creature (all four creature type-flag bits set); else null
// Leaf: ??$GetPTarget@VCThingCreatureBase@@@CMessageEvent@@QBEPAVCThingCreatureBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006AF520, 48 bytes, thiscall, `ret`.
// Identity (probable): Ego_r.pdb CMessageEvent has POriginator (CIntelligentPointer<CThing>) +0x48 and PTarget +0x50 and
//   the getter templates GetPOriginator<CThing / CThingAICreature / CThingCreatureBase> and GetPTarget<CThing /
//   CThingCreatureBase>; this body reads PTarget (+0x50) and requires a non-empty type mask (0x0F), so it is the
//   CThingCreatureBase instantiation (the CThing one needs no type test: see 0x006AF4E0; the +0x48 twin with mask 0x1F
//   is landed as 0x0090DF30). Its caller CTCPerceiveAlert::FindAlertEvent 0x00744D00 binds this address as an invented
//   "CGameScriptThing::GetPThing" (label correction; CGameScriptThing is 0x48 bytes, PThing at +0x0C).
// Callees: CBaseIntelligentPointer::GetPItem() const 0x00A01B50 (three times; the last a tail jump).
// Layout: CThing::ThingTypeFlags retail +0x6C (Ego_r +0x7C less the 0x10 the STLport containers save before it).
// Shape: the landed 0x0090DF30 body: `T* result = 0` in edi, the mask test `not eax ; test al,0Fh`.
class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

#define FABLE_RETAIL_CTHING
#include "engine/CThing.h"
class CThingCreatureBase : public CThing { };

class CMessageEvent {
public:
    unsigned char               _pad00[0x48];
    CIntelligentPointer<CThing> POriginator;             // +0x48
    CIntelligentPointer<CThing> PTarget;                 // +0x50
    template <class T> T* GetPTarget() const;
};

template <class T> T* CMessageEvent::GetPTarget() const
{
    T* result = 0;
    if (PTarget.Get()) {
        if (!(~PTarget.Get()->GetTypeFlags() & 0x0F))
            result = (T*)PTarget.Get();
    }
    return result;
}
template CThingCreatureBase* CMessageEvent::GetPTarget<CThingCreatureBase>() const;