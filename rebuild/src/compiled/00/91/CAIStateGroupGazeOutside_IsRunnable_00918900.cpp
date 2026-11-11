// CAIStateGroup_GazeOutside::IsRunnable() -- runnable when the idle scheduler (if any) prefers this group, the
// Leaf: ?IsRunnable@CAIStateGroup_GazeOutside@@UAE_NXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// creature's village reports the right time of day and a free gaze position exists.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00918900, length 186 bytes (+ int3 padding). Vtable slot 11.
// Callees: CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface: 0xd3 = bit 19 of +0x38, 0x23 = bit 3 of
// +0x24), CTCIdleScheduler::IsPreferredIdle(CAIStateGroupBase*) @ 0x00801BE0, GFGetPVillageInterface @ 0x007EE540
// (twice), CTCVillage::IsSociableTime @ 0x00658D5D, CAIStateGroup_Gaze::FindFreeGazePos @ 0x00918880.
// Shape: `if (has scheduler && !scheduler->IsPreferredIdle(this)) return false;` returns the tested al; the rest
// is one `&&` chain returned through the full register (`mov eax,1` / `xor eax,eax`), the pointer result
// tested `!= 0`.
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
// Name fix (agentNF23): 0x00658D5D is ?IsSociableTime@CTCVillage@@QBE_NXZ (Ego_r 0x0069A63D / FableWin name sets; donor order IsNightTime / IsSociableTime / IsCurfewTime = retail 0x00658D07 / 0x00658D5D / 0x00658DB3, and the body reads CVillageDef SociableStartTime +0xD4 / SociableFinishTime +0xD8, retail STLport offsets of Ego_r 244 / 248); was IsCurfewTime() (non-const; the curfew test is 0x00658DB3).
class CTCBase;
class CThing;
class CWorld;
class CThingAICreature;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with CVectorMap::LowerBound @ 0x0040F020 -- the inlined
// GetInterface<T>() shape of the landed AI bodies (hit path: lookup result; miss path: the never-written slot).
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};

// IS_THING_ALIVE(ptr): the pointer is set and the thing's dead bit (+0x91 bit 0) is clear (inlined).
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!p.GetPItem()->IsDead())
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

class CTCIdleScheduler { public:                       // interface 0xd3
    bool IsPreferredIdle(CAIStateGroupBase* group);   // 0x00801BE0
};

class CTCVillage { public:
    bool IsSociableTime() const;                // 0x00658D5D
};
struct CTCVillager;
class CTCVillageMember { public: CTCVillage* GetPVillage() const; };   // 0x007EE540 (Ego_r.pdb CTCVillageMember::GetPVillage, const)

class CThingGameObject;
struct CAIStateGroup_Gaze : CAIStateGroupBase {
    unsigned char GazeMarker[8];                // +0x14 CIntelligentPointer<CThingGameObject>
    float ChanceOfRunning;                      // +0x1c
    float MinJourneyForRunning;                 // +0x20
    float MaxDistanceForCreaturesToLookAt;      // +0x24
    long PrimaryMarkerDef;                      // +0x28
    long SecondaryMarkerDef;                    // +0x2c
    CThingGameObject* FindFreeGazePos();        // 0x00918880
};

struct CAIStateGroup_GazeOutside : CAIStateGroup_Gaze {
    virtual bool IsRunnable();
};

bool CAIStateGroup_GazeOutside::IsRunnable()
{
    CTCIdleScheduler* scheduler;
    if (PBrain->PCreature->GetTC(TCI_IDLE_SCHEDULER, &scheduler) && !scheduler->IsPreferredIdle(this))
        return false;
    CTCVillager* villager;
    return PBrain->PCreature->GetTC(TCI_VILLAGE_MEMBER, &villager)
        && ((CTCVillageMember*)villager)->GetPVillage() != 0
        && ((CTCVillageMember*)villager)->GetPVillage()->IsSociableTime()
        && FindFreeGazePos() != 0;
}