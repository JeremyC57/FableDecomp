// CAIStateGroup_MinionSitAtSchool::InitCreaturePosition() -- during school time, place the creature near its home building (radius 5).
// Leaf: ?InitCreaturePosition@CAIStateGroup_MinionSitAtSchool@@UAE_NXZ
// Plan phase 2 (2026-11-04), CVectorMap wave (stand-in map): the local CInterfaceMap stand-in (and its same-TU LowerBound body) replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..> > derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D; the same-TU noinline LowerBound is the header's); entries read as pair first/second, the map end through end(). Spellings unified: 0040f020: ?LowerBound@CInterfaceMap@@QAEPAUSEntry@1@PBJ@Z -> ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z
// Grade: RELOCATION_MATCH -- same-TU LowerBound body; the && chain inside `if (HasTC(0x23)) { return ...; } return false;` (trailing byte false block, as retail); the creature passed to an inline PlaceNearHome(creature) helper so it is loaded once AFTER the school-time call (a named local loads it before)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0090FC50, length 127 bytes (+ int3 padding). Vtable slot 3.
// Callees: CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface, type 0x23 = CTCVillager, bit 3 of the bitset
// dword at +0x24), GFGetPVillageInterface @ 0x007EE540 (__fastcall on the villager component), the village's
// school-time test @ 0x00658A35 (manifest "CTCVillage::IsCookTime" is a propagated label on this body),
// CBaseIntelligentPointer::GetPItem @ 0x00A01B50 on the creature's PHome at +0x170, CThingCreatureBase::
// AutoPlaceNearBuilding(CThingBuilding*, float) @ 0x00666EF0 with 5.0f.
// Shape: a trailing `return false` for the interface miss (byte `xor al,al`; HasTC on the masked word), then a bool
// `&&` chain returned through the full register (`mov eax,1` / `xor eax,eax`) whose first call takes the
// pointer-returning FindTC result straight into ecx; the creature is the argument of an inline helper for the home
// lookup and the call (esi). Layout local (the engine header flattens the base).
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
class CTCBase;
class CThing;
class CWorld;
class CThingAICreature;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with CVectorMap::LowerBound @ 0x0040F020 -- the inlined
// GetInterface<T>() shape of the landed AI bodies (hit path: lookup result; miss path: the never-written slot).
#define FABLE_RETAIL_CTHING
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

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

// IS_THING_ALIVE(ptr): the pointer is set and the thing's dead bit (+0x91 bit 0) is clear (inlined).
inline bool IS_THING_ALIVE(const CIntelligentPointer<CThing>& p)
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
    CWorld* GetPWorld();                        // 0x008FCFC0 (PBrain->+0x1c->+0x24; manifest "CPlayer::GetCurrentMode" is propagated)
    unsigned long* GetPWorldSeed();             // 0x008FCFD0 (world random seed)
};

long __fastcall GFGetCurrentFrame();            // 0x0049D870 (manifest "GFGetPreMainMemoryUsed" is propagated)
unsigned long __fastcall GFROR13(unsigned long value);   // 0x00497890

struct CTCVillager;
class CTCVillage { public:
    bool IsSchoolTime();                        // 0x00658A35
};
class CTCVillageMember { public: CTCVillage* GetPVillage() const; };   // 0x007EE540 (Ego_r.pdb CTCVillageMember::GetPVillage, const)

class CThingBuilding;
struct CThingCreatureHome {
    unsigned char _pad[0x170];
    CIntelligentPointer<CThing> PHome;              // +0x170 CIntelligentPointer<CThingBuilding>
    bool AutoPlaceNearBuilding(CThingBuilding* building, float radius);   // 0x00666EF0
};

struct CAIStateGroup_MinionSitAtSchool : CAIStateGroupBase {
    virtual bool InitCreaturePosition();
};

__forceinline bool PlaceNearHome(CThingCreatureHome* creature)
{
    return creature->AutoPlaceNearBuilding((CThingBuilding*)creature->PHome.GetPItem(), 5.0f);
}

bool CAIStateGroup_MinionSitAtSchool::InitCreaturePosition()
{
    if (PBrain->PCreature->HasTC(TCI_VILLAGE_MEMBER)) {
        return ((CTCVillageMember*)(((CTCVillager*)&PBrain->PCreature->GetTC(TCI_VILLAGE_MEMBER))))->GetPVillage()->IsSchoolTime()
            && PlaceNearHome((CThingCreatureHome*)PBrain->PCreature);
    }
    return false;
}