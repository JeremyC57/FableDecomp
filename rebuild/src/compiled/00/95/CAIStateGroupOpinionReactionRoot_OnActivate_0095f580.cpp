// CAIStateGroup_OpinionReactionRoot::OnActivate() -- cut short the creature's current action when more than the
// Leaf: ?OnActivate@CAIStateGroup_OpinionReactionRoot@@UAEXXZ
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Naming wave 18: the global stand-in g_framesPerSecond (0x01375550) is the protected static long CMainGameComponent::ConstantFPS, read through the
//   inline static accessor GetConstantFPS() as retail code outside the class does (name-only: the bytes are unchanged).
// allowed seconds of it remain (and it is not a looping one); log the current reaction against the thing reacted to
// and take that reaction's "allow speech on non-pure-AI speaker" flag; cache the creature's CTCTalk.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0095F580, length 213 bytes (+ int3 padding).
// Callees: CCreatureActionBase::GetFramesRemaining @ 0x006925D0 (on the creature's current action at +0x100),
// CThingCreatureBase::FinishCurrentAction @ 0x00662650, CBaseIntelligentPointer::GetPItem @ 0x00A01B50,
// CAIStateGroupBase::GetWorld @ 0x008FCFC0 (+0x74 = COpinionReactionManager), COpinionReactionManager::LogReaction
// @ 0x00700940, CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface into PTCTalk, type 0xF = CTCTalk, bit 15 of
// the dword at +0x20); frames-per-second long @ 0x01375550, the seconds float global @ 0x0122DEDC.
// Shape: the frame rate is read into a local before the GetFramesRemaining call (both fild from the frame), the
// reaction manager pointer is a named local; the per-reaction flag table is a byte array at manager+0x45f.
// Engine header names; ThingReactingTo is the unaligned CIntelligentPointer at +0x6b (PDB, packed).
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
protected:
    CWorld& GetWorld() const;   // 0x008FCFC0 (Ego_r.pdb ?GetWorld@CAIStateGroupBase@@IBEAAVCWorld@@XZ)
};

class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

// `PThing = thing` is the converting-temporary + inline `SetPItem(other.GetPItem())` assignment.

class CMainGameComponent {
protected:
    static long ConstantFPS;                             // 0x01375550 ?ConstantFPS@CMainGameComponent@@1JA (protected static)
public:
    static long __fastcall GetConstantFPS() { return ConstantFPS; }   // Ego_r / FableWin ?GetConstantFPS@CMainGameComponent@@SIJXZ (inline)
};

class CCreatureActionBase { public:
    unsigned char _pad[0x20];
    long LoopCount;                             // +0x20
    long GetFramesRemaining();                  // 0x006925D0
};
class CThingCreatureBase { public:
    unsigned char _pad[0x100];
    CCreatureActionBase* PCurrentAction;        // +0x100
    void FinishCurrentAction();                 // 0x00662650
};

class COpinionReactionManager { public:
    unsigned char _pad[0x45f];
    bool AllowSpeechOnNonPureAISpeaker[1];      // +0x45f, indexed by reaction type
    void LogReaction(long reactionType, CThing* creature, bool flag);   // 0x00700940
};
class CWorld { public:
    unsigned char _pad[0x74];
    COpinionReactionManager* POpinionReactionManager;   // +0x74
};

class CTCTalk;

#pragma pack(push, 1)
class CAIStateGroup_OpinionReactionRoot : public CAIStateGroupBase { public:
    CTCTalk* PTCTalk;                           // +0x14
    long CurrentReactionType;                   // +0x18 EOpinionReactionType
    bool ReactionFlags[79];                     // +0x1c
    CBaseIntelligentPointer ThingReactingTo;    // +0x6b CIntelligentPointer<CThing>
    unsigned char _pad_0x73[0x5];               // +0x73
    unsigned char CurrentUpdateFunc[0x10];      // +0x78
    bool LastRunWasTryAndLook;                  // +0x88
    bool ReactingToPlayer;                      // +0x89
    bool AllowSpeechOnNonPureAISpeaker;         // +0x8a
    virtual void OnActivate();
};
#pragma pack(pop)

void CAIStateGroup_OpinionReactionRoot::OnActivate()
{
    CCreatureActionBase* action = ((CThingCreatureBase*)PBrain->PCreature)->PCurrentAction;
    if (action != 0) {
        long framesPerSecond = CMainGameComponent::GetConstantFPS();
        if (action->GetFramesRemaining() > framesPerSecond * 0.0f && action->LoopCount <= 0)
            ((CThingCreatureBase*)PBrain->PCreature)->FinishCurrentAction();
    }
    AllowSpeechOnNonPureAISpeaker = false;
    if (CurrentReactionType != 0 && ThingReactingTo.GetPItem() != 0) {
        COpinionReactionManager* manager = GetWorld().POpinionReactionManager;
        manager->LogReaction(CurrentReactionType, PBrain->PCreature, true);
        AllowSpeechOnNonPureAISpeaker = manager->AllowSpeechOnNonPureAISpeaker[CurrentReactionType];
    }
    PBrain->PCreature->GetTC(TCI_TALK, &PTCTalk);
}