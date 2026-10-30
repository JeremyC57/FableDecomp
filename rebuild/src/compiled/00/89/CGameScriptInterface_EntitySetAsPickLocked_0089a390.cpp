// CGameScriptInterface::EntitySetAsPickLocked(const CScriptThing&) const -- script: mark a door as lock-picked: open and jam it, post a lock-pick message, and give the door an item-used event from the hero
// Leaf: ?EntitySetAsPickLocked@CGameScriptInterface@@UBEXABVCScriptThing@@@Z (Ego_r.pdb UBE @ 0x008D8FC0; virtual, modelled non-virtual)
// Retail: Fable.exe @ 0x0089A390, 248 bytes, `ret 4`.
// Body: if !thing.IsValid() (CScriptThing slot 75, +0x12c) return; OpenDoor(thing) (slot 425, +0x6a4); JamDoor(thing)
// (slot 429, +0x6b4); door = thing.GetThing() (slot 11, +0x2c); World->PMessageEventManager->AddMessage(0x46, 50,
// C3DVector(0, 0, 0), 0, 0, 0.0f, 0.0f, door) (0x00687540); hero = PlayerManager->GetMainPlayer() (0x00449970)
// .GetPControlledCreature() (0x00487DC0); if hero, hero alive (Dead +0x91 bit 0) and door->GetTC<CTCEntityEvents>(&events)
// (TC 0x42: UsingComponents bit test, Components lower_bound 0x0040F020): events->AddEvent(new
// NEntityEvents::CEventItemUsed(*hero)) (operator new 0x10, ctor 0x006A0750, AddEvent 0x008113C0; a null new passes 0).
// Names: OpenDoor / JamDoor are the RTTI vtable's PDB names for slots 425 / 429; AddMessage / CEventItemUsed(CThing&) /
// CTCEntityEvents::AddEvent are Ego_r.pdb symbols. The message type 0x46 is not named. Ego_r.pdb's AddMessage ends
// (CThing*, CThing*, float, float, long); retail passes the door as that last dword, so it is declared CThing* here.
// Layout: CGameScriptInterface World +0x04, PlayerManager +0x14; CWorld PMessageEventManager +0x60 (engine/CWorld.h);
// CThing retail UsingComponents +0x20, Components +0x44, Dead +0x91 bit 0 (as 0x00897AA0).
// Opt: /O2 (16-aligned entry).
class C3DVector {
public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                                    // CThing::Components, CVectorMap<ETCInterfaceType, CTCBase*> (retail 8 bytes)
public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);                // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long UsingComponents[9];                    // +0x20 bitset<274>
    CInterfaceMap Components;                            // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char Dead : 1;                              // +0x91 bit 0
    bool IsDead() const { return Dead; }
    bool HasTC(long type) const
    {
        if (UsingComponents[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    template <class T> __forceinline bool GetTC(T** out)
    {
        if (!HasTC(T::INTERFACE_TYPE))
            return false;
        ETCInterfaceType type = (ETCInterfaceType)(T::INTERFACE_TYPE);
        CInterfaceMap* map = &Components;
        CInterfaceMap::SEntry* it = map->LowerBound(type);
        if (it == map->last || T::INTERFACE_TYPE < it->first)
            it = map->last;
        *out = (T*)it->second;
        return true;
    }
};
class CThingPlayerCreature : public CThing { };
class CPlayer {
public:
    CThingPlayerCreature* GetPControlledCreature() const;   // 0x00487DC0
};
// CPlayerManager: the shared model engine/CPlayerManager.h (Ego_r.pdb CPlayerManager; the five accessors declared
//   there with their retail addresses: GetPlayer 0x004498C0 / 0x00449910, GetMainPlayer 0x00449970 / 0x00449960,
//   GetMainPlayerNumber 0x00449700).
#include "engine/CPlayerManager.h"
namespace NEntityEvents {
class CEventBase {
public:
    virtual ~CEventBase();
    long Data04;
};
class CEventItemUsed : public CEventBase {               // vtable 0x0125C820, 0x10 bytes
public:
    void* ItemUser[2];                                   // +0x08 (thing pointer, set by 0x00A01B10 / 0x00A01B90)
    CEventItemUsed(CThing& user);                        // 0x006A0750
};
}
class CTCEntityEvents {
public:
    enum { INTERFACE_TYPE = 0x42 };
    void AddEvent(NEntityEvents::CEventBase* event);     // 0x008113C0
};
enum EMessageEventType { MESSAGE_EVENT_0x46 = 0x46 };
class CMessageEventManager {
public:
    void* AddMessage(EMessageEventType type, unsigned long duration, const C3DVector& pos, CThing* from, CThing* to,
                     float a, float b, CThing* extra);   // 0x00687540
};
class CWorld {
public:
    unsigned char         _pad00[0x60];
    CMessageEventManager* PMessageEventManager;          // +0x60 (CCountedPointer data)
};
class CScriptThing {
public:
    virtual void _s000() const; virtual void _s001() const; virtual void _s002() const; virtual void _s003() const; virtual void _s004() const; virtual void _s005() const;
    virtual void _s006() const; virtual void _s007() const; virtual void _s008() const; virtual void _s009() const; virtual void _s010() const;
    virtual CThing* GetThing() const;                    // slot 11 (+0x2c)
    virtual void _s012() const; virtual void _s013() const; virtual void _s014() const; virtual void _s015() const; virtual void _s016() const; virtual void _s017() const;
    virtual void _s018() const; virtual void _s019() const; virtual void _s020() const; virtual void _s021() const; virtual void _s022() const; virtual void _s023() const;
    virtual void _s024() const; virtual void _s025() const; virtual void _s026() const; virtual void _s027() const; virtual void _s028() const; virtual void _s029() const;
    virtual void _s030() const; virtual void _s031() const; virtual void _s032() const; virtual void _s033() const; virtual void _s034() const; virtual void _s035() const;
    virtual void _s036() const; virtual void _s037() const; virtual void _s038() const; virtual void _s039() const; virtual void _s040() const; virtual void _s041() const;
    virtual void _s042() const; virtual void _s043() const; virtual void _s044() const; virtual void _s045() const; virtual void _s046() const; virtual void _s047() const;
    virtual void _s048() const; virtual void _s049() const; virtual void _s050() const; virtual void _s051() const; virtual void _s052() const; virtual void _s053() const;
    virtual void _s054() const; virtual void _s055() const; virtual void _s056() const; virtual void _s057() const; virtual void _s058() const; virtual void _s059() const;
    virtual void _s060() const; virtual void _s061() const; virtual void _s062() const; virtual void _s063() const; virtual void _s064() const; virtual void _s065() const;
    virtual void _s066() const; virtual void _s067() const; virtual void _s068() const; virtual void _s069() const; virtual void _s070() const; virtual void _s071() const;
    virtual void _s072() const; virtual void _s073() const; virtual void _s074() const;
    virtual bool IsValid() const;                        // slot 75 (+0x12c)
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
void CGameScriptInterface::EntitySetAsPickLocked(const CScriptThing& thing) const
{
    if (!thing.IsValid())
        return;
    OpenDoor(thing);
    JamDoor(thing);
    CThing* door = thing.GetThing();
    World->PMessageEventManager->AddMessage(MESSAGE_EVENT_0x46, 50, C3DVector(0.0f, 0.0f, 0.0f), 0, 0, 0.0f, 0.0f, door);
    CThingPlayerCreature* hero = PlayerManager->GetMainPlayer().GetPControlledCreature();
    CTCEntityEvents* events;
    if (hero && !hero->IsDead() && door->GetTC(&events))
        events->AddEvent(new NEntityEvents::CEventItemUsed(*hero));
}