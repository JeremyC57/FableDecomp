// CAIStateGroup_MinionSitOutside::IsStillValid() -- not during the village curfew, and only until FrameToStopSitting.
// Leaf: ?IsStillValid@CAIStateGroup_MinionSitOutside@@UAEEXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00939060, length 114 bytes (+ int3 padding). Vtable slot 5.
// Callees: CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface, type 0x23 = CTCVillager, bit 3 of the bitset
// dword at +0x24), GFGetPVillageInterface @ 0x007EE540 (twice: null test, then the call object), CTCVillage::
// IsCurfewTime @ 0x00658DB3, CAIStateGroupBase::GetPWorld @ 0x008FCFC0 (called for nothing: the result is dropped,
// as in the landed siblings), CWorld::GetFrame @ 0x0049D870.
// Shape: an early `return false` (its own byte `xor al,al` epilogue) from inside the nested village test, then
// `(&GetWorld()); return GFGetCurrentFrame() <= FrameToStopSitting;` (`cmp eax,[edi+40h] ; setle al` with no
// `xor` zero-extension: the result type is an unsigned char, not bool).
// FrameToStopSitting is +0x40 (engine header name; the header flattens the base so the class is modelled locally).
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
    virtual void InitStateGroup();              // slot 13 (+0x34)
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
    bool IsCurfewTime_00658db3();                        // 0x00658DB3
};
class CTCVillageMember { public: CTCVillage* GetPVillage() const; };   // 0x007EE540 (Ego_r.pdb CTCVillageMember::GetPVillage, const)

struct CAIStateGroup_MinionSitOutside : CAIStateGroupBase {
    unsigned char _seat[0x2c];                  // +0x14..+0x3f UsesSeat / SitOnSeatBase / SitOnSeat members
    long FrameToStopSitting;                    // +0x40
    virtual unsigned char IsStillValid();       // `setle al` with no zero-extension: an unsigned char result
};

unsigned char CAIStateGroup_MinionSitOutside::IsStillValid()
{
    CTCVillager* villager;
    if (PBrain->PCreature->GetTC(TCI_VILLAGE_MEMBER, &villager)) {
        if (((CTCVillageMember*)villager)->GetPVillage() != 0) {
            if (((CTCVillageMember*)villager)->GetPVillage()->IsCurfewTime_00658db3())
                return false;
        }
    }
    (&GetWorld());
    return CWorld::GetFrame() <= FrameToStopSitting;
}