// CAIStateGroup_MeleeCombatNearbyBase::ShouldGoStraightIntoStrafe() -- in combat mode, with a live enemy and the
// Leaf: ?ShouldGoStraightIntoStrafe@CAIStateGroup_MeleeCombatNearbyBase@@QAE_NXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// group running: strafe straight away when the creature is nearer the enemy (XY plane) than its preferred combat
// position is.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0093D180, length 236 bytes (+ int3 padding).
// Callees: CThingCreatureBase::IsInCombatMode @ 0x006621A0, CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PEnemy
// +0x14), IsStopping @ 0x008FD010, CThingCreatureBase::GetCombatManager @ 0x00661ED0, CCombatManager::
// GetPreferredPos (C3DVector by hidden pointer) @ 0x006EA770, CThing::GetPosition @ 0x004C73D0.
// Shape: the preferred position is a named 12-byte local built in place (its arguments pushed before the manager
// call); two XY deltas from the enemy position: the first a 12-byte local whose z is never written (retail keeps
// the dead slot: a C2DVector gives a 4-byte smaller frame) stored across the calls, the second on the
// FPU), compared by squared length (`fcompp ; test ah,41h`).
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
class CTCBase;
class CThing;
class CWorld;
class CThingAICreature;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with CVectorMap::LowerBound @ 0x0040F020 -- the inlined
// GetInterface<T>() shape of the landed AI bodies (hit path: lookup result; miss path: the never-written slot).
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class C3DVector;

class CBaseObject;
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseObject* GetPItem() const;                   // 0x00A01B50
    CBaseIntelligentPointer();                  // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
    void SetPItem(CThing* item);                // 0x00A01B90
};

struct CIntelligentPointerThing : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01244344
    virtual void v00();
    CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { SetPItem(((CThing*)other.GetPItem())); return *this; }
};

// IS_THING_ALIVE(ptr): the pointer is set and the thing's dead bit (+0x91 bit 0) is clear (inlined).
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (((CThing*)p.GetPItem()) != 0) {
        if (!((CThing*)p.GetPItem())->IsDead())
            return true;
    }
    return false;
}

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

class CAIStateGroupBase { public:
protected:
    const CWorld& PeekWorld() const;    // 0x008FCFA0
public:
    virtual ~CAIStateGroupBase();               // slot 0
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08();
    virtual void Cleanup();                     // slot 9  (+0x24)
    virtual void OnReplace();                   // slot 10 (+0x28)
    virtual bool IsRunnable();                  // slot 11 (+0x2c)
    virtual void Update();                      // slot 12 (+0x30)
    virtual void v13();
    virtual long GetTimeToInitiate();           // slot 14 (+0x38)
    virtual void v15(); virtual void v16(); virtual void v17();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
protected:
    CWorld& GetWorld() const;    // 0x008FCFC0 (PBrain->+0x1c->+0x24; manifest "CPlayer::GetCurrentMode" is propagated)
public:
protected:
    bool IsStopping() const;   // 0x008FD010 (Ego_r.pdb protected const)
};

// Retail builds message filters as an expression chain: `IsOfType(n) && HasExtraData() && IsForThing(t)`.
// Each `operator&&` returns a CFilter_Bind2/CFilter_Bind3 holding POINTERS to its operands (the filter
// temporaries live in the caller's frame until the call returns; the empty one-byte filter has a user
// constructor so it takes no store and sits in the high byte of a slot), and FindMostRecentMessage takes the
// bound filter by const reference (one `lea ; push`) -- the landed 0x00910D50 / 0x00939D70 model.
class CMessageEventFilter_IsOfType { public:
    long m_type;
    CMessageEventFilter_IsOfType(long type) : m_type(type) { }
};

class CMessageEventFilter_HasExtraData { public:
    CMessageEventFilter_HasExtraData() { }
};

struct CMessageEventFilter_IsForThing {
    CThing* m_pThing;
    CMessageEventFilter_IsForThing(CThing* thing) : m_pThing(thing) { }
};

class CMessageEventFilter_IsOriginatedByThing { public:
    unsigned __int64 UID;                       // the originator's 64-bit UID (PDB)
    CMessageEventFilter_IsOriginatedByThing(const CThing* thing);    // 0x006AF2B0 (0x006AF2B0 / 0x006AF290 twins, out of line, `ret 4`)
};

template<class A, class B> class CFilter_Bind2 { public:
    const A* m_pFirst;
    const B* m_pSecond;
    CFilter_Bind2(const A& a, const B& b) : m_pFirst(&a), m_pSecond(&b) { }
};

template<class A, class B, class C> class CFilter_Bind3 { public:
    const A* m_pFirst;
    const B* m_pSecond;
    const C* m_pThird;
    CFilter_Bind3(const CFilter_Bind2<A, B>& ab, const C& c) : m_pFirst(ab.m_pFirst), m_pSecond(ab.m_pSecond), m_pThird(&c) { }
};

template<class A, class B> inline CFilter_Bind2<A, B> operator&&(const A& a, const B& b) { return CFilter_Bind2<A, B>(a, b); }
template<class A, class B, class C> inline CFilter_Bind3<A, B, C> operator&&(const CFilter_Bind2<A, B>& ab, const C& c) { return CFilter_Bind3<A, B, C>(ab, c); }

class CMessageEvent;
class CMessageEventManager;
class CWorld { public:
    unsigned char _pad[0x60];
    CMessageEventManager* m_pMessageManager;    // +0x60
    CMessageEventManager* GetPMessageManager() { return m_pMessageManager; }
};

struct CAIStateGroupBaseW : CAIStateGroupBase {
};

class C3DVector { public: float x, y, z; };
class C2DVector { public:
    float x, y;
    float LengthSquared() const { return x * x + y * y; }
};
// a 12-byte delta whose z is never written (retail's frame keeps the unused slot)
struct CXYDelta {
    float x, y, z;
    float LengthSquaredXY() const { return x * x + y * y; }
};

class CCombatManager;
class CThingCreatureBase : public CThing { public:
    bool IsInCombatMode();                      // 0x006621A0
    CCombatManager& GetCombatManager();         // 0x00661ED0
};

class CCombatManager { public:
    C3DVector GetPreferredPos(CThingCreatureBase* creature);   // 0x006EA770
};

struct CAIStateGroup_MeleeCombatNearbyBase : CAIStateGroupBase {
    CBaseIntelligentPointer PEnemy;             // +0x14 CIntelligentPointer<CThingCreatureBase>
    bool Archer;                                // +0x1c
    CThingCreatureBase* GetPEnemy() const { return (CThingCreatureBase*)PEnemy.GetPItem(); }
    bool ShouldGoStraightIntoStrafe();
};

bool CAIStateGroup_MeleeCombatNearbyBase::ShouldGoStraightIntoStrafe()
{
    if (((CThingCreatureBase*)PBrain->PCreature)->IsInCombatMode() && IS_THING_ALIVE(PEnemy) && !IsStopping()) {
        CThingCreatureBase* self = (CThingCreatureBase*)PBrain->PCreature;
        C3DVector preferred = self->GetCombatManager().GetPreferredPos(self);
        CThingCreatureBase* creature = (CThingCreatureBase*)PBrain->PCreature;
        const C3DVector& enemyPos = GetPEnemy()->GetPos();
        const C3DVector& creaturePos = creature->GetPos();
        CXYDelta toCreature;
        toCreature.x = creaturePos.x - enemyPos.x;
        toCreature.y = creaturePos.y - enemyPos.y;
        const C3DVector& enemyPos2 = GetPEnemy()->GetPos();
        C2DVector toPreferred = { preferred.x - enemyPos2.x, preferred.y - enemyPos2.y };
        if (toCreature.LengthSquaredXY() < toPreferred.LengthSquared())
            return true;
    }
    return false;
}