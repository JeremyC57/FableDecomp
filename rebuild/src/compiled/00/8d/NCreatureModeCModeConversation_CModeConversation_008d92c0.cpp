// NCreatureMode::CModeConversation::CModeConversation(CThing*, EConversationAttitude) -- conversation mode with an explicit attitude; the owner's CTCTalk attitude becomes current and re-targets the filter when it differs from the initial one
// Leaf: ??0CModeConversation@NCreatureMode@@QAE@PAVCThing@@J@Z
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: map pointer made const (the row grades the const LowerBound).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008D92C0, 144 bytes (padding included).
// Shape: CModeBase ctor (0x0073EAC0), vtable 0x01285990, CurrentAttitude (+0x1c) = InitialAttitude
// (+0x20) = attitude (one register), the out-of-line CAnimFilterOfGroup ctor (0x0073E8F0) over
// GetAnimGroupName(attitude) (0x004CD9C0), ResetIdleAnim = true. Body: when the owner has interface 15
// (CTCTalk, inlined out-parameter GetInterface, LowerBound 0x004365B0) its GetAttitude (0x006E3180)
// becomes CurrentAttitude, and if InitialAttitude differs (`cmp [esi+20h],eax`: the member is the
// left operand) the filter is re-targeted (0x0073DFF0). Unlike the one-argument twin @ 0x008D9220 the
// stores go to CurrentAttitude only. Field names from the PDB (CModeConversation, 52 bytes).
class CCharString;
class CTCBase;

typedef long EConversationAttitude;

const CCharString& __fastcall GetAnimGroupName(EConversationAttitude attitude);   // 0x004CD9C0

#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CTCTalk { public:
    EConversationAttitude GetAttitude() const;     // 0x006E3180
};

class CAnimFilterOfGroup { public:                  // vtable 0x01267568
    virtual void v00();
    void* Group;                             // +4 CCharString
    unsigned long GroupCRC;                  // +8
    CAnimFilterOfGroup(const CCharString& group);   // 0x0073E8F0
    void SetGroup(const CCharString& group);        // 0x0073DFF0
};

struct CModeBase {
    virtual void v00();
    CThing* ParentThing;                     // +4
    unsigned char _body[0x14];               // WeaponBasedFilter, FramesBeforeRemovingSelf, Active
    CModeBase(CThing* thing);                // 0x0073EAC0
};

namespace NCreatureMode {

struct CModeConversation : CModeBase {
    EConversationAttitude CurrentAttitude;   // +0x1c
    EConversationAttitude InitialAttitude;   // +0x20
    CAnimFilterOfGroup Filter;               // +0x24
    bool ResetIdleAnim;                      // +0x30
    virtual void v00();
    CModeConversation(CThing* thing, EConversationAttitude attitude);
};

CModeConversation::CModeConversation(CThing* thing, EConversationAttitude attitude)
    : CModeBase(thing)
    , CurrentAttitude(attitude)
    , InitialAttitude(attitude)
    , Filter(GetAnimGroupName(attitude))
    , ResetIdleAnim(true)
{
    const CTCTalk* talk;
    if (ParentThing->PeekTC(TCI_TALK, &talk)) {
        EConversationAttitude current = talk->GetAttitude();
        CurrentAttitude = current;
        if (InitialAttitude != current)
            Filter.SetGroup(GetAnimGroupName(current));
    }
}

}