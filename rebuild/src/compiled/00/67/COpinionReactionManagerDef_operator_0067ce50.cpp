// COpinionReactionManagerDef::operator=(const COpinionReactionManagerDef&) -- memberwise def assignment: the parent def base (out-of-line CDefClassBase part, name, sub-def map), the blur vector, eight reaction maps, the match list, the frequency-traits block and its two cursors; returns *this
// Leaf: ??4COpinionReactionManagerDef
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0067CE50, length 240 bytes (`ret 4`, returns `this`).
// Identity: the manifest label "CThingBuildingDef::operator=" is propagated over every def assignment; the body
//   is reached through the `jmp` thunk at 0x0067CE40 in slot 19 of vtable 0x0125B7DC =
//   COpinionReactionManagerDef (rtti-vtables.tsv).
// Callees: CDefClassBase::operator= @ 0x00431F10, SubDefInfoMap's vector operator= @ 0x00431F90,
//   vector<CBlurredAttitudesDef>::operator= @ 0x0067D450, the map assignments @ 0x0067D030 (AttitudeCondition),
//   0x0067D190 (TargetingCondition), 0x0067D2F0 (PreReactionDelay), 0x004E99AE (ToleranceToBeingHit),
//   0x0045DD2C (the four bool maps), list<CReactionMatch_*>::operator= @ 0x0067CF40. The trees keep the
//   class names their landed bodies carry (CRetailTree_<copy fn>).
// Layout: PDB names, retail offsets. Retail CParentDefClassBase is byte-packed (InstantiationName +0x25,
//   SubDefInfoMap +0x29, its sorted flag +0x36); retail containers are 12 bytes and the list is its bare
//   4-byte head pointer, so AttitudeBlur +0x3c (PDB +0x40), the maps +0x48..+0x9c, Matches +0xa8,
//   FrequencyTraitsArray +0xac (0x278-byte POD block, copied with `rep movsd`), FrequencyTraits +0x324,
//   FrequencyTraitsHigh +0x328 (PDB +0x354/+0x358).
// Shape: the implicit assignment written out; CParentDefClassBase's implicit assignment inlines (the name
//   word loaded before the map's address arithmetic, the map vector out of line, then the sorted byte).
// NF70 (round 7, land1117fu): the stand-in tree struct(s) CRetailTree_004e9a21, CRetailTree_0045dd9f are now the STLport std::map<EOpinionAttitudeType, float>, std::map<EOpinionReactionType, bool> from stlport/fable_map.h; the implicit map/set operator= is inline and calls the instance's out-of-line _Rb_tree::operator=.
#include "stlport/fable_map.h"
#include "engine/EOpinionAttitudeType.h"
enum EOpinionReactionType;
#pragma pack(push, 1)
class CDefClassBase { public:
    unsigned char _body[0x21];                          // +0x04 .. +0x24 (PDefManager, GlobalIndex, Setup bits, ...)
    CDefClassBase& operator=(const CDefClassBase& other);   // 0x00431F10
    virtual void v00();
};
#pragma pack(pop)

template<class T> struct CRetailVector {                // begin/end/capacity
    T* m_first; T* m_last; T* m_end;
    CRetailVector& operator=(const CRetailVector& other);   // CSubDefInfo: 0x00431F90
};

struct CKeyPairCompareLess {};

template<class K, class V> class CVectorMap { public:
    CRetailVector<V>    m_vector;                       // +0x00
    CKeyPairCompareLess m_compare;                      // +0x0c (empty)
    bool                m_sorted;                       // +0x0d
};

class CDefString { public: long TablePos; };
struct CSubDefInfo;

#pragma pack(push, 1)
class CParentDefClassBase : public CDefClassBase { public:
    CDefString                             InstantiationName;   // +0x25
    CVectorMap<unsigned long, CSubDefInfo> SubDefInfoMap;       // +0x29
};
#pragma pack(pop)

struct CBlurredAttitudesDefVector {                     // vector<CBlurredAttitudesDef>
    void* m_first; void* m_last; void* m_end;
    CBlurredAttitudesDefVector& operator=(const CBlurredAttitudesDefVector& other);   // 0x0067D450
};

struct CRetailTree_0067d0f0 { void* head; unsigned long size; unsigned long _compare;   // map<EOpinionReactionType, EOpinionAttitudeType>
    CRetailTree_0067d0f0& operator=(const CRetailTree_0067d0f0& other); };             // 0x0067D030
struct CRetailTree_0067d250 { void* head; unsigned long size; unsigned long _compare;   // map<EOpinionReactionType, EOpinionTargetingConditionType>
    CRetailTree_0067d250& operator=(const CRetailTree_0067d250& other); };             // 0x0067D190
struct CRetailTree_0067d3b0 { void* head; unsigned long size; unsigned long _compare;   // map<EOpinionReactionType, float>
    CRetailTree_0067d3b0& operator=(const CRetailTree_0067d3b0& other); };             // 0x0067D2F0

struct CReactionMatchList {                             // list<CReactionMatch_*> (retail: the head pointer)
    void* m_head;
    CReactionMatchList& operator=(const CReactionMatchList& other);   // 0x0067CF40
};

class CReactionFrequencyTraits;
class CReactionFrequencyTraitsArray { public: unsigned long m_words[0x9e]; };   // 0x278-byte POD block

class COpinionReactionManagerDef : public CParentDefClassBase { public:               // vtable 0x0125B7DC
    CBlurredAttitudesDefVector AttitudeBlur;                            // +0x3c
    CRetailTree_0067d0f0 AttitudeCondition;                             // +0x48
    CRetailTree_0067d250 TargetingCondition;                            // +0x54
    CRetailTree_0067d3b0 PreReactionDelay;                              // +0x60
    std::map<EOpinionAttitudeType, float> ToleranceToBeingHit;                           // +0x6c
    std::map<EOpinionReactionType, bool> BlockFurtherReactions;                         // +0x78
    std::map<EOpinionReactionType, bool> AllowSpeechOnNonPureAISpeaker;                 // +0x84
    std::map<EOpinionReactionType, bool> AllowWhileCarrying;                            // +0x90
    std::map<EOpinionReactionType, bool> AllowWhileFollowingPlayer;                     // +0x9c
    CReactionMatchList Matches;                                         // +0xa8
    CReactionFrequencyTraitsArray FrequencyTraitsArray;                 // +0xac
    CReactionFrequencyTraits** FrequencyTraits;                         // +0x324
    CReactionFrequencyTraits** FrequencyTraitsHigh;                     // +0x328
    COpinionReactionManagerDef& operator=(const COpinionReactionManagerDef& other);
};

COpinionReactionManagerDef& COpinionReactionManagerDef::operator=(const COpinionReactionManagerDef& other)
{
    CParentDefClassBase::operator=(other);
    AttitudeBlur = other.AttitudeBlur;
    AttitudeCondition = other.AttitudeCondition;
    TargetingCondition = other.TargetingCondition;
    PreReactionDelay = other.PreReactionDelay;
    ToleranceToBeingHit = other.ToleranceToBeingHit;
    BlockFurtherReactions = other.BlockFurtherReactions;
    AllowSpeechOnNonPureAISpeaker = other.AllowSpeechOnNonPureAISpeaker;
    AllowWhileCarrying = other.AllowWhileCarrying;
    AllowWhileFollowingPlayer = other.AllowWhileFollowingPlayer;
    Matches = other.Matches;
    FrequencyTraitsArray = other.FrequencyTraitsArray;
    FrequencyTraits = other.FrequencyTraits;
    FrequencyTraitsHigh = other.FrequencyTraitsHigh;
    return *this;
}