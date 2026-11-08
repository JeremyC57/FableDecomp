// CAIStateGroup_MinionGossip::IsStillValid() -- once relying on the gossipee, still valid only while the gossip partner
// Leaf: ?IsStillValid@CAIStateGroup_MinionGossip@@UAE_NXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// (our CTCGossip's target) is alive and the partner's CTCGossip targets us back.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0090CB80, length 185 bytes (+ int3 padding).
// Callees: CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface, type 0xC1 = CTCGossip, bit 1 of the bitset
// dword at +0x38), CBaseIntelligentPointer::GetPItem @ 0x00A01B50 on CTCGossip's +0x0c target pointer (twice).
// Shape: GetInterface writes straight into the members PTCGossip (+0x1c) / PTCGossipOther (+0x20), which are then
// reloaded (`mov [edi+1ch],edx ; mov ecx,[edi+1ch]`); the presence-bit miss path just falls through to the reload.
// Engine header names (PDB); the header flattens the base, so the class is modelled locally.
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
#include "fable_compiler.h"
class CTCBase;
class CThing;
class CWorld;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with CVectorMap::LowerBound @ 0x004365B0.
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
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

class CTCGossip { public:
    unsigned char _pad[0xc];
    CBaseIntelligentPointerCtor PGossipTarget;  // +0x0c
};

struct CAIStateGroup_MinionGossip : CAIStateGroupBase {
    long FrameOfLastGossip;                     // +0x14
    bool Critical;                              // +0x18
    bool NowRelyingOnGossipee;                  // +0x19
    CTCGossip* PTCGossip;                       // +0x1c
    CTCGossip* PTCGossipOther;                  // +0x20
    virtual bool IsStillValid();
};

bool CAIStateGroup_MinionGossip::IsStillValid()
{
    if (NowRelyingOnGossipee) {
        PBrain->PCreature->GetTC(TCI_GOSSIP, &PTCGossip);
        CThing* other = PTCGossip->PGossipTarget.GetPItem();
        if (other != 0 && !other->IsDead()) {
            other->GetTC(TCI_GOSSIP, &PTCGossipOther);
            CThing* self = PBrain->PCreature;
            if (PTCGossipOther->PGossipTarget.GetPItem() == self)
                return true;
        }
        return false;
    }
    return true;
}