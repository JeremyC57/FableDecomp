// CGameScriptThing::MsgWhoKilledMe() const -- the script thing of whoever originated the most recent "killed" message (type 9) aimed at this thing, when that originator is still a live thing; otherwise the null script thing
// Leaf: MsgWhoKilledMe
// Retail: Fable.exe (TLC, VC7.1) @ 0x008D0610, 161 bytes, `ret 4` (the hidden CScriptThing result).
// Callees: CMessageEventManager::FindMostRecentMessage<CFilter_Bind2<IsOfType, IsTargettedAtThing>> @ 0x008D46D0 (its
//   label names the CanBeSeenOrHeard instantiation), CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (x3),
//   CThing::GetScriptThing @ 0x004C7E50 (result pointer passed straight through), CGameScriptThingManager::
//   GetNullScriptThing @ 0x00704730 (landed) through g_pQuestManager (0x013B89FC) ->PGameScriptThingManager (+4).
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

struct CBaseObject;
struct CScriptThing {                              // 12 bytes (vptr + PImp {Data, Info}), returned by hidden pointer
    void* __vftable;
    void* PImp_Data;
    void* PImp_Info;
    CScriptThing(const CScriptThing& other);
};
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
struct CBaseIntelligentPointer {
    void* __vftable;
    void* Info;
    CBaseObject* GetPItem() const;                 // 0x00A01B50
};
struct CMessageEvent {                             // 0x58 bytes
    unsigned char           _pad00[0x48];
    CBaseIntelligentPointer POriginator;           // +0x48 CIntelligentPointer<CThing>
    CBaseIntelligentPointer PTarget;               // +0x50
};
struct CMessageEventFilter_IsOfType {
    long Type;
    CMessageEventFilter_IsOfType(long type) : Type(type) { }
};
struct CMessageEventFilter_IsTargettedAtThing {
    unsigned __int64 UID;
    CMessageEventFilter_IsTargettedAtThing(unsigned __int64 uid) : UID(uid) { }
};
template <class A, class B> struct CFilter_Bind2 { // the bound filter holds POINTERS to its operands
    const A* m_pFirst;
    const B* m_pSecond;
    CFilter_Bind2(const A& a, const B& b) : m_pFirst(&a), m_pSecond(&b) { }
};
template <class A, class B> inline CFilter_Bind2<A, B> operator&&(const A& a, const B& b) { return CFilter_Bind2<A, B>(a, b); }
typedef CFilter_Bind2<CMessageEventFilter_IsOfType, CMessageEventFilter_IsTargettedAtThing> CKilledMeFilter;
struct CMessageEventManager {
    CMessageEvent* FindMostRecentMessage(const CKilledMeFilter& filter);   // 0x008D46D0
};

struct CGameScriptThingManager {
    CScriptThing GetNullScriptThing();             // 0x00704730
};
struct CQuestManager {
    void*                    _vftable;
    CGameScriptThingManager* PGameScriptThingManager;   // +0x04 CCountedPointer<CGameScriptThingManager>::Data
    CGameScriptThingManager* GetGameScriptThingManager() const { return PGameScriptThingManager; }
};
extern CQuestManager* g_pQuestManager;             // 0x013B89FC

struct CGameScriptThingMethods : CGameScriptThing {
    CScriptThing MsgWhoKilledMe() const;
};

CScriptThing CGameScriptThingMethods::MsgWhoKilledMe() const
{
    CMessageEventManager* messages = World->PMessageEventManager_Data;
    CMessageEvent* message = messages->FindMostRecentMessage(
        CMessageEventFilter_IsOfType(9) && CMessageEventFilter_IsTargettedAtThing(ThingUID));
    if (message) {
        if (message->POriginator.GetPItem() &&
            IsThingOfType<CThingRetail>((CThingRetail*)message->POriginator.GetPItem())) {
            CThingRetail* killer = (CThingRetail*)message->POriginator.GetPItem();
            if (killer && !killer->Dead)
                return killer->GetScriptThing();
        }
    }
    return g_pQuestManager->GetGameScriptThingManager()->GetNullScriptThing();
}
