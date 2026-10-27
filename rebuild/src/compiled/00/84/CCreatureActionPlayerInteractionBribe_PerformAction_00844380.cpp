// CCreatureAction_PlayerInteractionBribe::PerformAction() -- when both the creature and the target are alive, post message 0x14 (flags 0x10, last argument 3) at the creature's position from the creature to the target
// Leaf: ?PerformAction@CCreatureAction_PlayerInteractionBribe@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00844380, 136 bytes, thiscall, `ret`.
// Identity (proven): vtable 0x01259664 (RTTI .?AVCCreatureAction_PlayerInteractionBribe@@) slot 3, the slot holding the landed CCreatureAction_PlayerInteractionGreet::PerformAction 0x00845900 in Greet's vtable 0x012592D4; Ego_r.pdb ?PerformAction@CCreatureAction_PlayerInteractionBribe@@UAEXXZ.
// Callees: CBaseIntelligentPointer::GetPItem() const 0x00A01B50 (PCreature +0x08, PTarget +0xA8), CCreatureActionBase::GetWorld()
//   0x00692940 (Ego_r.pdb IAE, a CWorld&), CThing::GetPosition() const 0x006A4D00 (the second retail copy, catalog spelling
//   GetPosition_006a4d00 per GOTCHAS), CMessageEventManager::AddMessage(EMessageEventType, unsigned long, const C3DVector&, CThing*,
//   CThing*, float, float, long) 0x00687540 (the world's PMessageEventManager +0x60).
// Shape (family L075): the landed Greet 0x00845900 message tail -- the creature fetched twice into named locals, the manager into a
//   third before the arguments; IS_THING_ALIVE (non-null, Dead bit 0 of +0x91 clear) on both pointers.
// Header debt: engine/CCreatureAction_PlayerInteractionBribe.h and engine/CCreatureActionBase.h are flat byte layouts without bases or methods, and there is
//   no engine/CThing.h, so the action, its base and CThing are modelled locally (retail offsets); CWorld is the shared header.
#include "engine/CWorld.h"
class C3DVector;
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // Ego_r.pdb: vptr + PData, 8 bytes
public:
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};
class CThing {
public:
    unsigned char _pad00[0x91];                          // (members not touched here)
    unsigned char Flags91;                               // +0x91 bit 0: dead
    const C3DVector& GetPosition_006a4d00() const;       // 0x006A4D00
};
inline bool IS_THING_ALIVE(const CIntelligentPointer<CThing>& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->Flags91 & 1))
            return true;
    }
    return false;
}
enum EMessageEventType { EMessageEventType_FIRST };
class CMessageEvent;
class CMessageEventManager {
public:
    CMessageEvent* AddMessage(EMessageEventType type, unsigned long flags, const C3DVector& position, CThing* from, CThing* to,
                              float a, float b, long priority);   // 0x00687540
};
class CCreatureActionBase {                            // retail sizeof 0xA8
public:
    virtual ~CCreatureActionBase();
    unsigned char               _pad04[4];             // +0x04
    CIntelligentPointer<CThing> PCreature;             // +0x08
    unsigned char               _pad10[0xA8 - 0x10];   // +0x10
protected:
    CWorld& GetWorld();                                // 0x00692940
};
class CCreatureAction_PlayerInteractionBribe : public CCreatureActionBase {
public:
    CIntelligentPointer<CThing> PTarget;               // +0xA8
    virtual void PerformAction();
};
void CCreatureAction_PlayerInteractionBribe::PerformAction()
{
    if (IS_THING_ALIVE(PCreature) && IS_THING_ALIVE(PTarget)) {
        CThing* pCreatureThing = PCreature.GetPItem();
        CThing* pCreature = PCreature.GetPItem();
        CMessageEventManager* pManager = GetWorld().PMessageEventManager_Data;
        pManager->AddMessage((EMessageEventType)0x14, 0x10, pCreature->GetPosition_006a4d00(), pCreatureThing, PTarget.GetPItem(), 0.0f, 0.0f, 3);
    }
}