// CAIStateGroup_MinionReactToBurglary::IsRunnable() -- during the creature's sleep hours, runnable when the live
// Leaf: ?IsRunnable@CAIStateGroup_MinionReactToBurglary@@UAE_NXZ
// Plan phase 4 (2026-11-05), C3DVector wave (rows with a second value class, from their landed source): the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// player is visible to it and standing inside its home building; remember the player in PBurglar.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00957F20, length 210 bytes (+ int3 padding).
// Callees: CThingAICreature::GetSleepTimes @ 0x00833390 (two out longs), CAIStateGroupBase::GetWorld @ 0x008FCFC0
// (twice), the world clock's IsTimeOfDayInRange @ 0x006BB390 (world +0x1c), CBaseIntelligentPointer::GetPItem
// @ 0x00A01B50 on the creature's home-building pointer (+0x168), the world's player-manager getter @ 0x0049D9D0
// and its player getter @ 0x00487DD0 (manifest "_Cons_val<...>" labels are propagated and wrong),
// CanCreatureSeeCreature @ 0x008645E0, the thing position getter @ 0x004C73D0, CThingBuilding::IsPointInside
// @ 0x0082D000, CIntelligentPointer(CThing*) @ 0x0056C860, CIntelligentPointer::operator= @ 0x005ED300,
// ~CBaseIntelligentPointer @ 0x00A01C10.
// Shape: the home pointer read through an inline getter (`lea ecx,[eax+168h]`); the time test result and the home pointer are named locals computed before the combined test (bl / ebp);
// `PBurglar = CIntelligentPointer(player)` through a named out-of-line-constructed pointer + operator=; the clock is a named local (GetPWorld before the argument loads).
// No engine header for this class; PBurglar is +0x14.
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
#include "fable_compiler.h"
class CTCBase;
class CThing;
class CWorld;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with CVectorMap::LowerBound @ 0x004365B0.
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();               // slot 0
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual bool IsStillValid();                // slot 5  (+0x14)
    virtual void v06(); virtual void v07(); virtual void v08();
    virtual void Cleanup();                     // slot 9  (+0x24)
    virtual void OnReplace();                   // slot 10 (+0x28)
    virtual bool IsRunnable();                  // slot 11 (+0x2c)
    virtual void Update();                      // slot 12 (+0x30)
    virtual void InitStateGroup();              // slot 13 (+0x34)
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
protected:
    CWorld& GetWorld() const;   // 0x008FCFC0 (Ego_r.pdb ?GetWorld@CAIStateGroupBase@@IBEAAVCWorld@@XZ)
};

struct CBaseIntelligentPointerCtor {            // CBaseIntelligentPointer (8 bytes: vptr + item)
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointerCtor();              // 0x00A01B10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
    ~CBaseIntelligentPointerCtor();             // 0x00A01C10
};

// `PThing = thing` is the converting-temporary + inline `SetPItem(other.GetPItem())` assignment.
struct CIntelligentPointerThing : CBaseIntelligentPointerCtor {
    FABLE_FORCEINLINE CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointerCtor() { SetPItem(item); }
    FABLE_FORCEINLINE CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { SetPItem(other.GetPItem()); return *this; }
};

#include "engine/C3DVector.h"

class CThingAICreature { public:
    void GetSleepTimes(long* start, long* end); // 0x00833390
};
struct CThingPos {
    const C3DVector& GetPosition();             // 0x004C73D0
};
class CThingBuilding { public:
    bool IsPointInside(const C3DVector& point) const; // 0x0082D000
};
struct CHomeOwner {
    unsigned char _pad[0x168];
    CBaseIntelligentPointerCtor PHome;          // +0x168
    CThing* GetPHome() { return PHome.GetPItem(); }
};

struct CWorldClock {
    bool IsTimeOfDayInRange(long start, long end);  // 0x006BB390
};
class CPlayerManager { public:
    CThing* GetPlayerCharacter();               // 0x00487DD0
};
class CWorld { public:
    unsigned char _pad[0x1c];
    CWorldClock* PClock;                        // +0x1c
    CPlayerManager* GetPlayerManager();         // 0x0049D9D0
};

bool __fastcall CanCreatureSeeCreature(const CThing* viewer, const CThing* target, bool ignoreLight);   // 0x008645E0

struct CIntelligentPointerOut : CBaseIntelligentPointerCtor {
    CIntelligentPointerOut(CThing* item);       // 0x0056C860
    CIntelligentPointerOut& operator=(const CIntelligentPointerOut& other);   // 0x005ED300
};

struct CAIStateGroup_MinionReactToBurglary : CAIStateGroupBase {
    CIntelligentPointerOut PBurglar;            // +0x14
    virtual bool IsRunnable();
};

bool CAIStateGroup_MinionReactToBurglary::IsRunnable()
{
    long start;
    long end;
    ((CThingAICreature*)PBrain->PCreature)->GetSleepTimes(&start, &end);
    CWorldClock* clock = GetWorld().PClock;
    bool sleepTime = clock->IsTimeOfDayInRange(start, end);
    CThingBuilding* home = (CThingBuilding*)((CHomeOwner*)PBrain->PCreature)->GetPHome();
    if (sleepTime && home != 0) {
        CThing* player = GetWorld().GetPlayerManager()->GetPlayerCharacter();
        if (player != 0 && !player->IsDead() && CanCreatureSeeCreature(PBrain->PCreature, player, false) &&
            home->IsPointInside(((CThingPos*)player)->GetPosition())) {
            CIntelligentPointerOut burglar(player);
            PBurglar = burglar;
            return true;
        }
    }
    return false;
}