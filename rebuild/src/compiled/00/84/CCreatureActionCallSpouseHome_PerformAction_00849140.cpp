// CCreatureAction_CallSpouseHome::PerformAction() -- looks up the "STANDARD_MESSAGE_EVENT_CALL_SPOUSE_HOME" message event def through the creature's definition manager and, when found, posts it from the creature's position with the creature as the first thing; releases the def
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00849140, length 158 bytes (+ int3 padding).
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0, CCreatureActionBase::GetPDefinitionManager @ 0x00692910 (`PCreature.GetPItem()`
// then the creature's manager getter @ 0x004C7990; the manifest label is STL noise), CDefinitionManager::GetDef<T>(const CCharString&,
// CDefPointer<T>&) @ 0x007923C0 (bool), CCharString::~CCharString @ 0x0099EAE0, CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature
// at +8, three times), CCreatureActionBase::GetPWorld @ 0x00692940 (landed naming of 0x00842F70), CThing::GetPosition @ 0x006A4D00,
// CMessageEventManager::AddMessage(CMessageEventDef*, const C3DVector*, CThing*, CThing*, long) @ 0x00687730, then the def's release
// (`dec [p+4] ; jne ; call [vt+4]`) written by hand on a pointer local (GOTCHAS wave 25).
// agentNF164 (q5101 / q5102): the two CCreatureActionBase accessors take their PDB spellings (Ego_r.pdb + FableWin.pdb, as
//   engine/CCreatureActionBase.h FABLE_RETAIL_CCREATUREACTIONBASE_ACCESSORS): ?PeekDefinitionManager@CCreatureActionBase@@IBEABVCGameDefinitionManager@@XZ
//   (was GetPDefinitionManager QBE PAVCDefinitionManager) and ?GetWorld@CCreatureActionBase@@IAEAAVCWorld@@XZ (was GetPWorld QBE),
//   respelled in place on this row's local CCreatureActionBase; the manager is the shared engine/CGameDefinitionManager.h class.
// Shape: the name is a named local built first (`push -1 ; push offset` before the frame zero store); the def pointer is copied into
// ebp right after the lookup result is moved to bl; inside the `if` the creature is fetched twice into locals (the first unused), the
// manager into a third (edi, `this` being dead by then), then the five arguments right to left (0, 0, the creature, its position, the def).
// Layout: retail CCreatureActionBase is 0xa8 bytes with PCreature at +8; no engine header for this class, so `this` is modelled locally.
// NF49 (round 7): the string, the def handle, the message-event def and the manager come from the shared headers (were local
// CCharString / CMessageEventDef / CDefPointer / CDefinitionManager models): the lookup at 0x007923C0 is the by-name member template
// ??$GetDef@VCMessageEventDef@@@CDefinitionManager@@QBE_NABVCCharString@@PAV?$CDefPointer@$$CBVCMessageEventDef@@@@@Z (FableWin.pdb),
// with its same-TU body (FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_NAME_BODY: the template is instantiated in every TU that uses it,
// so VC7.1 sees the out-handle is only assigned through `out` and keeps the def in ebp); the hand-written release on a pointer
// local is the shared handle's scope-end destructor.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_NAME_BODY
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#include "fable_string.h"
#include "engine/CDefClassBase.h"
#include "engine/CDefPointer.h"
#include "engine/CDefinitionManager.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CMessageEventDef.h"
class C3DVector;
class CMessageEvent;


class CThingPhysical { public:  // retail: CThingPhysical derives from CThing; modelled as its base here (empty insertion, layout unchanged) so CThing-typed callers resolve the call
    const C3DVector& PeekPos() const;   // 0x006A4D00
};
class CThing : public CThingPhysical { public:
};

class CMessageEventManager { public:
    CMessageEvent* AddMessage(const CMessageEventDef& def, const C3DVector& position, CThing* thing1, CThing* thing2, long priority);   // 0x00687730  // NF35: Ego_r + FableWin ?AddMessage@CMessageEventManager@@QAEPAVCMessageEvent@@ABVCMessageEventDef@@ABVC3DVector@@PAVCThing@@2J@Z
};

class CWorld { public:
    unsigned char _pad[0x60];
    CMessageEventManager* m_pMessageEventManager;   // +0x60
};

class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

class CCreatureActionBase { public:                // retail sizeof 0xa8
    virtual void v00();
    unsigned char _pad04[4];                // +0x04
    CIntelligentPointer<CThing> PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0xa8 - 0x10];      // +0x10
protected:
    const CGameDefinitionManager& PeekDefinitionManager() const;   // 0x00692910 (Ego_r / FableWin protected const)
    CWorld& GetWorld();                     // 0x00692940 (Ego_r / FableWin protected)
};

class CCreatureAction_CallSpouseHome : public CCreatureActionBase { public:
    virtual void PerformAction();
};

void CCreatureAction_CallSpouseHome::PerformAction()
{
    CDefPointer<const CMessageEventDef> def;
    bool found;
    {
        CCharString name("STANDARD_MESSAGE_EVENT_CALL_SPOUSE_HOME", -1);
        found = PeekDefinitionManager().GetDef(name, &def);
    }
    if (found) {
        CThing* pCreatureThing = PCreature.GetPItem();   // fetched but unused: retail keeps the dead call
        CThing* pCreature = PCreature.GetPItem();
        CMessageEventManager* pManager = GetWorld().m_pMessageEventManager;   // named: resolved before the arguments
        pManager->AddMessage(*def, pCreature->PeekPos(), PCreature.GetPItem(), 0, 0);
    }
}
