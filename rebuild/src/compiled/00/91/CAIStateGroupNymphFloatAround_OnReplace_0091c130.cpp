// CAIStateGroup_NymphFloatAround::OnReplace() -- kill the live float target (+0x1c) and clear it, and stop the nymph
// Leaf: ?OnReplace@CAIStateGroup_NymphFloatAround@@UAEXXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// component's flying-around; a default CBrainOutput local is built and released around it (unused).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0091C130, length 235 bytes (+ int3 padding).
// Callees: CBrainOutput::CBrainOutput @ 0x00835620, CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CThing::Kill(bool)
// @ 0x004C9B80, the converting CIntelligentPointer temporary from 0 (vtable 0x01238C6C): ctor @ 0x00A01B10, SetPItem
// @ 0x00A01B90 (twice), dtor @ 0x00A01C10; CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface, type 0xF5 =
// CTCNymph, bit 21 of the dword at +0x3c), CTCNymph::StopFlyingAround(long) @ 0x0080F870, then the implicit
// ~CBrainOutput inlined: ~CBaseIntelligentPointer @ 0x00A01C10 on +0x40, the CCountedPointer block release at +0x20
// (count, __fastcall destroy, operator delete @ 0x00BFE9BC).
// Shape: the landed AIAction_PumpCombatActions (0x0097D980) CBrainOutput model with its inline counted-pointer dtor.
// No engine header for this class; PFloatTarget is +0x1c.
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
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

class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#include "engine/CIntelligentPointer.h"

// `PThing = thing` is the converting-temporary + inline `SetPItem(other.GetPItem())` assignment.


struct CCountBlock {                            // CCountedPointer's shared block
    long Count;                                 // +0x00
    void (__fastcall* Destroy)(void* object);   // +0x04
    void* Object;                               // +0x08
};

struct CCountedPointer {                        // CCountedPointer<..> {object, block}
    void* Object;                               // +0x00
    CCountBlock* Block;                         // +0x04
    ~CCountedPointer()
    {
        if (Block) {
            --Block->Count;
            if (Block->Count == 0) {
                Block->Destroy(Block->Object);
                delete Block;
            }
        }
        Object = 0;
        Block = 0;
    }
};

class CBrainOutput { public:                           // 0x48 bytes
    unsigned long Flags;                        // +0x00
    unsigned char _pad04[0x20 - 0x04];
    CCountedPointer PAction;                    // +0x20
    unsigned char _pad28[0x40 - 0x28];
    CBaseIntelligentPointer PTarget;        // +0x40
    CBrainOutput();                             // 0x00835620
};

struct CThingKill {
};
class CTCNymph { public:
    void StopFlyingAround(long mode);           // 0x0080F870
};

inline bool IS_THING_ALIVE(const CIntelligentPointer<CThing>& p)
{
    if (p.Get() != 0) {
        if (!p.Get()->IsDead())
            return true;
    }
    return false;
}

struct CAIStateGroup_NymphFloatAround : CAIStateGroupBase {
    unsigned char _pad_0x14[0x8];               // +0x14
    CIntelligentPointer<CThing> PFloatTarget;      // +0x1c
    virtual void OnReplace();
};

void CAIStateGroup_NymphFloatAround::OnReplace()
{
    CBrainOutput output;
    if (IS_THING_ALIVE(PFloatTarget)) {
        ((CThing*)(((CThingKill*)PFloatTarget.Get())))->Kill(true);
        PFloatTarget = 0;
    }
    CTCNymph* nymph;
    if (PBrain->PCreature->GetTC(TCI_NYMPH, &nymph))
        nymph->StopFlyingAround(-1);
}