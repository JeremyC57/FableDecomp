// CAIStateGroup_MinionGuardBribed::IsRunnable() -- runnable while the villager's village is still inside its
// Leaf: ?IsRunnable@CAIStateGroup_MinionGuardBribed@@UAE_NXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// "bribed" frame window.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00929AA0, length 112 bytes. Vtable slot 11 of
// CAIStateGroup_MinionGuardBribed (0x0128AE74).
// Callees: CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface, type 0x23 = CTCVillager, bit 3 of the
// bitset dword at +0x24), GFGetPVillageInterface @ 0x007EE540 (__fastcall on the villager component; the
// manifest's IS_THING_ALIVE label is propagated) twice, CAIStateGroupBase::GetPWorld @ 0x008FCFC0 (called
// for nothing: its result is dropped, as in the landed siblings), CWorld::GetFrame @ 0x0049D870.
// Shape: a bool `&&` chain (`mov eax,1` / `xor eax,eax` through the full register): the interface must be
// present, the village must exist, and the current frame must still be below the village's bribe-end frame
// (+0xA8), the village being re-fetched for the read. The villager pointer is an uninitialised local filled
// by the inlined lookup (the miss path jumps straight to the false exit).
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
    CWorld& GetWorld() const;                        // 0x008FCFC0 (PBrain->+0x1c->+0x24; manifest "CPlayer::GetCurrentMode" is propagated) ?GetWorld@CAIStateGroupBase@@IBEAAVCWorld@@XZ (Ego_r/FableWin, protected; agentNF16: was GetPWorld)
public:
    unsigned long* GetPWorldSeed();             // 0x008FCFD0 (world random seed)
};

class CWorld { public: static long __fastcall GetFrame(); };            // 0x0049D870 (manifest "GFGetPreMainMemoryUsed" is propagated)
unsigned long __fastcall GFROR13(unsigned long value);   // 0x00497890

struct CTCVillager;
class CTCVillage { public:
    unsigned char _pad[0xa8];
    long m_frameBribeEnds;                      // +0xa8
};
class CTCVillageMember { public: CTCVillage* GetPVillage() const; };   // 0x007EE540 (Ego_r.pdb CTCVillageMember::GetPVillage, const)

struct CAIStateGroup_MinionGuardBribed : CAIStateGroupBase {
    virtual bool IsRunnable();
    bool IsStillBribed(CTCVillager* villager)
    {
        (&GetWorld());
        return CWorld::GetFrame() < ((CTCVillageMember*)villager)->GetPVillage()->m_frameBribeEnds;
    }
};

bool CAIStateGroup_MinionGuardBribed::IsRunnable()
{
    CTCVillager* villager;
    return PBrain->PCreature->GetTC(TCI_VILLAGE_MEMBER, &villager)
        && ((CTCVillageMember*)villager)->GetPVillage() != 0
        && IsStillBribed(villager);
}