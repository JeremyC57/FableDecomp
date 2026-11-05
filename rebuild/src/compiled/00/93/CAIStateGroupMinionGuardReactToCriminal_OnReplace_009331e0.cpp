// CAIStateGroup_MinionGuardReactToCriminal::OnReplace() -- when this guard is the village's assigned criminal
// Leaf: ?OnReplace@CAIStateGroup_MinionGuardReactToCriminal@@UAEXXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// handler, clear the assignment.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009331E0, length 102 bytes (+ int3 padding). Vtable slot 10.
// Callees: CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface, type 0x23 = CTCVillager, bit 3 of the
// bitset dword at +0x24), GFGetPVillageInterface @ 0x007EE540 (__fastcall on the villager component),
// CBaseIntelligentPointer::GetPItem @ 0x00A01B50 on the village's intelligent pointer at +0xDC, then the
// village's setter @ 0x00933280 (`ret 4`, landed as "AssignTarget") with a null argument.
// Shape: nested ifs, no return value; the brain's creature is read into edi (the `this` register, dead after
// the last use) for the compare against the pointer item. The villager pointer is an uninitialised local
// filled by the inlined lookup (the miss path skips everything, `push esi` sits inside the hit branch).
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
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

class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"


// IS_THING_ALIVE(ptr): the pointer is set and the thing's dead bit (+0x91 bit 0) is clear (inlined).
inline bool IS_THING_ALIVE(const CIntelligentPointer<CThing>& p)
{
    if (p.Get() != 0) {
        if (!p.Get()->IsDead())
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
    unsigned char _pad[0xdc];
    CIntelligentPointer<CThing> PCriminalHandler;   // +0xdc the guard assigned to the current criminal
    void SetCriminalHandler(CThing* guard);     // 0x00933280
};
class CTCVillageMember { public: CTCVillage* GetPVillage() const; };   // 0x007EE540 (Ego_r.pdb CTCVillageMember::GetPVillage, const)

struct CAIStateGroup_MinionGuardReactToCriminal : CAIStateGroupBase {
    CIntelligentPointer<CThing> PHero;              // +0x14
    virtual void OnReplace();
};

void CAIStateGroup_MinionGuardReactToCriminal::OnReplace()
{
    CTCVillager* villager;
    if (PBrain->PCreature->GetTC(TCI_VILLAGE_MEMBER, &villager)) {
        CTCVillage* village = ((CTCVillageMember*)villager)->GetPVillage();
        if (village != 0) {
            CThing* creature = PBrain->PCreature;
            if (village->PCriminalHandler.Get() == creature)
                village->SetCriminalHandler(0);
        }
    }
}