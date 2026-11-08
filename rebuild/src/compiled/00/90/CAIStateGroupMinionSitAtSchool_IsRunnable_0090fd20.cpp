// CAIStateGroup_MinionSitAtSchool::IsRunnable() -- runnable when the villager's village says it is time to go to
// Leaf: ?IsRunnable@CAIStateGroup_MinionSitAtSchool@@UAE_NXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// school and the seat base agrees.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0090FD20, length 132 bytes (+ int3 padding). Vtable slot 11.
// Callees: CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface, type 0x23 = CTCVillager, bit 3 of the bitset
// dword at +0x24), GFGetPVillageInterface @ 0x007EE540 (twice: null test, then the call object), the second world
// accessor @ 0x008FCFA0, CTCVillage::IsTimeToGoToSchool(CThingAICreature*, CWorld*, bool) @ 0x0065A8EE,
// CAIStateGroup_MinionSitOnSeatBase::IsRunnable @ 0x0090F9C0.
// Shape: one bool `&&` chain returned through the full register (`mov eax,1` / `xor eax,eax`): interface present,
// village exists, IsTimeToGoToSchool(creature, world, true), base IsRunnable. The villager pointer stays in edi
// across the chain, `this` in ebx; the creature is a named local loaded before the argument pushes (`push 1`
// first: right-to-left) so it rides in esi across the world call (three callee-saved registers).
// Layout local (the engine header flattens the base).
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
#include "fable_compiler.h"
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
    unsigned long* GetPWorldSeed();             // 0x008FCFD0 (world random seed)
};

long __fastcall GFGetCurrentFrame();            // 0x0049D870 (manifest "GFGetPreMainMemoryUsed" is propagated)
unsigned long __fastcall GFROR13(unsigned long value);   // 0x00497890

struct CTCVillager;
class CTCVillage { public:
    bool IsTimeToGoToSchool(CThing* creature, CWorld* world, bool flag);   // 0x0065A8EE
};
class CTCVillageMember { public: CTCVillage* GetPVillage() const; };   // 0x007EE540 (Ego_r.pdb CTCVillageMember::GetPVillage, const)

struct CAIStateGroupBaseW : CAIStateGroupBase {
};

struct CAIStateGroup_MinionSitOnSeatBase : CAIStateGroupBaseW {
    unsigned char _seat[0x28];                  // +0x14..+0x3b UsesSeat members + POwner
    virtual bool IsRunnable();                  // 0x0090F9C0
};

struct CAIStateGroup_MinionSitAtSchool : CAIStateGroup_MinionSitOnSeatBase {
    virtual bool IsRunnable();
    // the creature is a NAMED local taken before the argument pushes (it rides in esi across the world call)
    FABLE_FORCEINLINE bool IsTimeForSchool(CTCVillager* villager)
    {
        CThing* creature = PBrain->PCreature;
        return ((CTCVillageMember*)villager)->GetPVillage()->IsTimeToGoToSchool(creature, (CWorld*)&PeekWorld(), true);
    }
};

bool CAIStateGroup_MinionSitAtSchool::IsRunnable()
{
    CTCVillager* villager;
    return PBrain->PCreature->GetTC(TCI_VILLAGE_MEMBER, &villager)
        && ((CTCVillageMember*)villager)->GetPVillage() != 0
        && IsTimeForSchool(villager)
        && CAIStateGroup_MinionSitOnSeatBase::IsRunnable();
}