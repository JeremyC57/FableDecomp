// CGameScriptThing::MsgWhoKilledMe() const -- the script thing of whoever originated the most recent "killed" message (type 9) aimed at this thing, when that originator is still a live thing; otherwise the null script thing
// Leaf: MsgWhoKilledMe
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 5 (2026-11-05), CScriptThing wave: the local CScriptThing stand-in replaced by the shared engine/CScriptThing.h under FABLE_RETAIL_NSCRIPT (RTTI CScriptThing : CBaseClass, vtable 0x01238C8C, PImp +0x04; ~CScriptThing 0x004AA840 and the copy 0x004ABE90 declared out of line). Also: the engine/CCountedPointer.h include moved up to the header include (the header needs it first).
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Naming wave 18: the global stand-in g_pQuestManager (0x013B89FC) is the private static CCountedPointer<CQuestManager>
//   CTBaseSingletonNoDefaultAlloc<CQuestManager>::PClass, read through the inline static accessor Get() (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) @ 0x008D0610, 161 bytes, `ret 4` (the hidden CScriptThing result).
// Callees: CMessageEventManager::FindMostRecentMessage<CFilter_Bind2<IsOfType, IsTargettedAtThing>> @ 0x008D46D0 (its
//   label names the CanBeSeenOrHeard instantiation), CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (x3),
//   CThing::GetScriptThing @ 0x004C7E50 (result pointer passed straight through), CGameScriptThingManager::
//   GetNullScriptThing @ 0x00704730 (landed) through CTBaseSingletonNoDefaultAlloc<CQuestManager>::PClass (0x013B89FC) ->PGameScriptThingManager (+4).
// Layout: engine/CGameScriptThing.h (ThingUID +0x18, World +0x44), engine/CWorld.h (PMessageEventManager_Data +0x60);
//   CMessageEvent (Ego_r.pdb, 0x58 bytes) POriginator +0x48; CThing retail ThingTypeFlags +0x6c and the Dead bit at
//   +0x91 (landed CThingSearchTools models).
// Shape: the 0x14 frame holds the IsOfType(9) filter, the IsTargettedAtThing(ThingUID) filter and the Bind2 pointer pair
//   (operator&&); the originator test is `GetPItem() && IsThingOfType<CThing>(converted temporary) && alive`: the
//   CThing type mask is 0 (copied into a local, so VC keeps the dead `xor al,al ; jne`) and the converted pointer
//   bound to the const reference is stored into the dead filter slot; the live test re-fetches the item. The message
//   manager is a local read before the filters are built; the quest manager's thing manager comes through an inline
//   accessor (loaded straight into ecx).
#include "engine/CGameScriptThing.h"
#include "engine/CWorld.h"

class CBaseObject;
#define FABLE_RETAIL_NSCRIPT
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
struct CThingRetail {                              // retail CThing (0x10 below Ego_r.pdb from ThingTypeFlags on)
    enum { ThingTypeMask = 0 };                    // every thing matches the CThing type mask
    unsigned char _pad00[0x6c];
    unsigned long ThingTypeFlags;                  // +0x6c
    unsigned char _pad70[0x91 - 0x70];
    unsigned char Dead : 1;                        // +0x91 bit 0
    unsigned char _bits91 : 7;
    CScriptThing GetScriptThing() const;           // 0x004C7E50
};
template <class T> inline bool IsThingOfType(const CThingRetail* const& thing)
{
    unsigned long mask = T::ThingTypeMask;
    return (thing->ThingTypeFlags & mask) == mask;
}
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
class CMessageEvent { public:                             // 0x58 bytes
    unsigned char           _pad00[0x48];
    CBaseIntelligentPointer POriginator;           // +0x48 CIntelligentPointer<CThing>
    CBaseIntelligentPointer PTarget;               // +0x50
};
class CMessageEventFilter_IsOfType { public:
    long Type;
    CMessageEventFilter_IsOfType(long type) : Type(type) { }
};
class CMessageEventFilter_IsTargettedAtThing { public:
    unsigned __int64 UID;
    CMessageEventFilter_IsTargettedAtThing(unsigned __int64 uid) : UID(uid) { }
};
template <class A, class B> class CFilter_Bind2 { public: // the bound filter holds POINTERS to its operands
    const A* m_pFirst;
    const B* m_pSecond;
    CFilter_Bind2(const A& a, const B& b) : m_pFirst(&a), m_pSecond(&b) { }
};
template <class A, class B> inline CFilter_Bind2<A, B> operator&&(const A& a, const B& b) { return CFilter_Bind2<A, B>(a, b); }
typedef CFilter_Bind2<CMessageEventFilter_IsOfType, CMessageEventFilter_IsTargettedAtThing> CKilledMeFilter;
class CMessageEventManager { public:
    template <class TFilter> const CMessageEvent* FindMostRecentMessage(const TFilter& filter) const;   // 0x008D46D0 ??$FindMostRecentMessage@V?$CFilter_Bind2@VCMessageEventFilter_IsOfType@@VCMessageEventFilter_IsTargettedAtThing@@@@@CMessageEventManager@@QBEPBVCMessageEvent@@...
};

class CGameScriptThingManager { public:
    CScriptThing GetNullScriptThing();             // 0x00704730
};
template <class T> class CTBaseSingletonNoDefaultAlloc {              // the CRTP singleton base of CQuestManager
    static CCountedPointer<T> PClass;                                // 0x013B89FC ?PClass@?$CTBaseSingletonNoDefaultAlloc@VCQuestManager@@@@0V?$CCountedPointer@VCQuestManager@@@@A (private static)
public:
    static T& __fastcall Get() { return *PClass; }                   // Ego_r / FableWin ?Get@?$CTBaseSingletonNoDefaultAlloc@VCQuestManager@@@@SIAAVCQuestManager@@XZ (inline)
};
class CQuestManager { public:
    void*                    _vftable;
    CGameScriptThingManager* PGameScriptThingManager;   // +0x04 CCountedPointer<CGameScriptThingManager>::Data
    CGameScriptThingManager* GetGameScriptThingManager() const { return PGameScriptThingManager; }
};

struct CGameScriptThingMethods : CGameScriptThing {
    CScriptThing MsgWhoKilledMe() const;
};

CScriptThing CGameScriptThingMethods::MsgWhoKilledMe() const
{
    CMessageEventManager* messages = World->PMessageEventManager_Data;
    const CMessageEvent* message = messages->FindMostRecentMessage(
        CMessageEventFilter_IsOfType(9) && CMessageEventFilter_IsTargettedAtThing(ThingUID));
    if (message) {
        if (message->POriginator.GetPItem() &&
            IsThingOfType<CThingRetail>((CThingRetail*)message->POriginator.GetPItem())) {
            CThingRetail* killer = (CThingRetail*)message->POriginator.GetPItem();
            if (killer && !killer->Dead)
                return killer->GetScriptThing();
        }
    }
    return CTBaseSingletonNoDefaultAlloc<CQuestManager>::Get().GetGameScriptThingManager()->GetNullScriptThing();
}