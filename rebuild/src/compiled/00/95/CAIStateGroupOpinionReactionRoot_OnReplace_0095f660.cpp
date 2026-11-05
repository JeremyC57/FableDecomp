// CAIStateGroup_OpinionReactionRoot::OnReplace() -- stop any speech, and when a reaction to a known thing was in
// Leaf: ?OnReplace@CAIStateGroup_OpinionReactionRoot@@UAEXXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// progress log it with the world's opinion-reaction manager; remember whether the last run was a "try and look".
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0095F660, length 131 bytes (+ int3 padding). Vtable slot 10.
// Callees: CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface, type 0xF = CTCTalk, bit 15 of the bitset
// dword at +0x20: `test ch,ch ; jns`), CTCTalk::StopCurrentSpeech @ 0x006E2E60, CBaseIntelligentPointer::GetPItem
// @ 0x00A01B50 on ThingReactingTo at +0x6b, CAIStateGroupBase::GetPWorld @ 0x008FCFC0 (opinion-reaction manager
// at +0x74), COpinionReactionManager::LogReaction(EOpinionReactionType, CThingAICreature*, bool) @ 0x00700940.
// Shape: `if (GetInterface(&talk, 0xf)) talk->StopCurrentSpeech();` then `if (CurrentReactionType != 0 &&
// ThingReactingTo.GetPItem() != 0) { manager->LogReaction(type, creature, false); LastRunWasTryAndLook = false; }
// else LastRunWasTryAndLook = true;` (the world fetched before the argument loads, the manager read between the
// pushes). Members are the engine header names (CurrentReactionType +0x18, ThingReactingTo +0x6b -- the class is
// packed to 1, LastRunWasTryAndLook +0x88); the header flattens the base so the class is modelled locally.
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
    virtual void InitStateGroup();              // slot 13 (+0x34)
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

class CTCTalk { public:                                // interface type 0xF
    void StopCurrentSpeech();                   // 0x006E2E60
};

class COpinionReactionManager { public:
    void LogReaction(long type, CThing* creature, bool flag);    // 0x00700940
};

struct CWorldR {
    unsigned char _pad[0x74];
    COpinionReactionManager* m_pOpinionReactionManager;     // +0x74
};

#pragma pack(push, 1)
class CAIStateGroup_OpinionReactionRoot : public CAIStateGroupBase { public:
    CTCTalk* PTCTalk;                           // +0x14
    long CurrentReactionType;                   // +0x18 EOpinionReactionType
    bool ReactionFlags[79];                     // +0x1c
    CIntelligentPointer<CThing> ThingReactingTo;    // +0x6b CIntelligentPointer<CThing>
    unsigned char _pad_0x73[0x5];               // +0x73
    unsigned char CurrentUpdateFunc[0x10];      // +0x78 pointer to member
    bool LastRunWasTryAndLook;                  // +0x88
    bool ReactingToPlayer;                      // +0x89
    bool AllowSpeechOnNonPureAISpeaker;         // +0x8a
    virtual void OnReplace();
};
#pragma pack(pop)

void CAIStateGroup_OpinionReactionRoot::OnReplace()
{
    CTCTalk* pTalk;
    if (PBrain->PCreature->GetTC(TCI_TALK, &pTalk))
        pTalk->StopCurrentSpeech();
    if (CurrentReactionType != 0 && ThingReactingTo.Get() != 0) {
        CWorldR* world = (CWorldR*)GetPWorld();    // named: fetched before the argument loads (landed 24a lesson)
        world->m_pOpinionReactionManager->LogReaction(CurrentReactionType, PBrain->PCreature, false);
        LastRunWasTryAndLook = false;
    } else {
        LastRunWasTryAndLook = true;
    }
}