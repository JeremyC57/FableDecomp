// CAIStateGroup_OpinionReactionRoot::Update() -- run the current update member function (marking the opinion
// Leaf: ?Update@CAIStateGroup_OpinionReactionRoot@@UAEXXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// component's reaction as done when it succeeds), then wait (yielding) while the creature is still talking; always
// finish the update; bail straight out when the state group is no longer valid.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0095F6F0, length 215 bytes (+ int3 padding).
// Callees: own virtual slot 5 (+0x14, IsStillValid), the CurrentUpdateFunc member-function pointer at +0x78 (fn, this
// adjustment at +0x7c), CVectorMap::LowerBound @ 0x0040F020 (non-const) / 0x004365B0 (const) (inlined GetInterface, type 0x4F = CTCCreatureOpinionOfHero,
// bit 15 of the dword at +0x28) and @ 0x004365B0 (type 0xF = CTCTalk, bit 15 of +0x20), CTCTalk::IsTalking
// @ 0x006E28A0, CAIStateGroupBase::IsStopping @ 0x008FD010, the brain's virtual slot 2 (+0x08, yield), the finish call
// @ 0x008FCF40.
// Shape: the PMF uses the multiple-inheritance representation (`#pragma pointers_to_members(full_generality, multiple_inheritance)`: fn +
// adjustment, `mov ecx,[edi+7Ch] ; add ecx,edi`; the PDB slot is 0x10 bytes); both lookups read the never-written out slot on a miss; the rotated yield loop.
// Engine header names; ThingReactingTo is the unaligned CIntelligentPointer at +0x6b (PDB, packed).
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
class CTCBase;
class CThing;
class CWorld;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with CVectorMap::LowerBound: the non-const
// instantiation @ 0x0040F020 and the const one @ 0x004365B0 (two byte-identical bodies).
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
public:
    void Terminate();   // 0x008FCF40
protected:
    bool IsStopping() const;   // 0x008FD010 (Ego_r.pdb protected const)
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
    __forceinline CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointerCtor() { SetPItem(item); }
    __forceinline CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { SetPItem(other.GetPItem()); return *this; }
};

class CTCCreatureOpinionOfHero { public:
    unsigned char _pad[0x340];
    bool ReactionHandled;                       // +0x340
};
class CTCTalk { public:
public:
    bool IsTalking() const;   // 0x006E28A0
};

struct CAIBrainV {
    virtual void v00(); virtual void v01();
    virtual void Yield();                       // slot 2 (+0x08)
};

#pragma pointers_to_members(full_generality, multiple_inheritance)

#pragma pack(push, 1)
class CAIStateGroup_OpinionReactionRoot : public CAIStateGroupBase { public:
    CTCTalk* PTCTalk;                           // +0x14
    long CurrentReactionType;                   // +0x18 EOpinionReactionType
    bool ReactionFlags[79];                     // +0x1c
    CBaseIntelligentPointerCtor ThingReactingTo;    // +0x6b CIntelligentPointer<CThing>
    unsigned char _pad_0x73[0x5];               // +0x73
    bool (CAIStateGroup_OpinionReactionRoot::*CurrentUpdateFunc)();    // +0x78 {fn, this adjustment}
    unsigned char _pmf_rest[0x8];               // +0x80 (the PDB member pointer is 0x10 bytes)
    bool LastRunWasTryAndLook;                  // +0x88
    bool ReactingToPlayer;                      // +0x89
    bool AllowSpeechOnNonPureAISpeaker;         // +0x8a
    virtual void Update();
};
#pragma pack(pop)

void CAIStateGroup_OpinionReactionRoot::Update()
{
    if (!IsStillValid()) {
        Terminate();
        return;
    }
    if (CurrentUpdateFunc != 0) {
        if (!(this->*CurrentUpdateFunc)()) {
            Terminate();
            return;
        }
        CTCCreatureOpinionOfHero* opinion;
        PBrain->PCreature->GetTC(TCI_CREATURE_OPINION_OF_HERO, &opinion);
        opinion->ReactionHandled = true;
    }
    const CTCTalk* talk;
    PBrain->PCreature->PeekTC(TCI_TALK, &talk);
    while (talk->IsTalking()) {
        if (IsStopping())
            break;
        ((CAIBrainV*)PBrain)->Yield();
    }
    Terminate();
}