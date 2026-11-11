// CTCCreatureOpinionOfHero::AdjustOpinionTransient(EOpinion opinion, float peakOffset, long framesToRunIn, long framesToRunOut, float finalOffset, bool ignorePersonality) -- queue a transient opinion offset, scaled by the creature's personality traits for that opinion unless told to ignore them or it has no personality def
// Leaf: ?AdjustOpinionTransient@CTCCreatureOpinionOfHero@@QAEXW4EOpinion@@MJJM_N@Z
//
// Retail 0x00682140, 160 bytes, thiscall `ret 18h`.
// Callees: COpinionTransientOffset::COpinionTransientOffset(EOpinion, float, long, long, float) @ 0x00681610 (landed),
//   COpinionTransientOffset::COpinionTransientOffset(const COpinionTransientOffset&, const COpinionPersonalityTraits&)
//   @ 0x00681680 (landed), list<COpinionTransientOffset>::push_back(const COpinionTransientOffset&) @ 0x00479449
//   (landed).
// Callers: CTCWife (0x006CFE37, 0x006CFE67, 0x006D039A, 0x006D03BE, 0x006D1B75; landed callers declare it at this
//   address).
// Layout (retail, as the landed CTCCreatureOpinionOfHero ctor 0x00684420; engine/CTCCreatureOpinionOfHero.h is the
//   PDB layout and does not fit past +0x24): PPersonalityDef +0x20, TransientOffsets (STLport list, head pointer)
//   +0x338; COpinionPersonalityDef's PersonalityTraits pointer +0x3C (PDB +0x40) to 36-byte COpinionPersonalityTraits
//   indexed by opinion.
// Shape: `if (!ignorePersonality && (personality = PPersonalityDef) != 0) { COpinionTransientOffset offset(opinion,
//   peakOffset, framesToRunIn, framesToRunOut, finalOffset); TransientOffsets.push_back(COpinionTransientOffset(offset,
//   personality->PersonalityTraits[opinion])); } else TransientOffsets.push_back(COpinionTransientOffset(opinion, ...));`
//   -- the plain offset is a named local so it is built BEFORE the traits address is formed (as one nested temporary,
//   VC7.1 evaluates the traits argument first); the def pointer assigned inside the condition gets edi and `this` ebx
//   (a local initialised before the `if` swaps them).  Both temporaries share the 0x30 frame; each arm has its own
//   epilogue.
// Name (naming agent NF21, 2026-11-11): Ego_r.pdb and FableWin.pdb both spell ?AdjustOpinionTransient@CTCCreatureOpinionOfHero@@
//   QAEXW4EOpinion@@MJJM_N@Z, the only CTCCreatureOpinionOfHero member taking (EOpinion, float, long, long, float, bool); the
//   landed callers (CTCWife OnHeroViolence 0x006CFDA0, Divorce 0x006D0180, Marry 0x006D18A0) push exactly those six arguments.
//   The catalog's "ApplyOpinionTransientOffset" was descriptive. EOpinion is the PDB enum (the COpinionTransientOffset ctor
//   0x00681610 is ??0COpinionTransientOffset@@QAE@W4EOpinion@@MJJM@Z in Ego_r.pdb).
enum EOpinion;                                              // MS forward enum (Ego_r.pdb)

class COpinionPersonalityTraits {                           // 36 bytes (Ego_r.pdb)
public:
    float BaseOffset;                                       // +0x00
    float HeroStatScaling;                                  // +0x04
    float VillageOpinionScaling;                            // +0x08
    float TimeScalingRunIn;                                 // +0x0c
    float TimeScalingRunOut;                                // +0x10
    float PeakScalingIfPos;                                 // +0x14
    float PeakScalingIfNeg;                                 // +0x18
    float PersistScalingIfPos;                              // +0x1c
    float PersistScalingIfNeg;                              // +0x20
};

class COpinionTransientOffset {                             // 0x18 bytes (Ego_r.pdb)
public:
    EOpinion Opinion;                                       // +0x00
    float    OffsetPerFrameRunIn;                           // +0x04
    long     FramesToRunIn;                                 // +0x08
    long     FramesOfCappedPeak;                            // +0x0c
    float    OffsetPerFrameRunOut;                          // +0x10
    long     FramesToRunOut;                                // +0x14
    COpinionTransientOffset(EOpinion opinion, float peakOffset, long framesToRunIn, long framesToRunOut,
                            float finalOffset);             // 0x00681610
    COpinionTransientOffset(const COpinionTransientOffset& offset,
                            const COpinionPersonalityTraits& traits);   // 0x00681680
};

class COpinionPersonalityDef {
public:
    unsigned char              _pad00[0x3C];
    COpinionPersonalityTraits* PersonalityTraits;           // +0x3C (PDB +0x40)
};

template <class T> class CDefPointer {
public:
    T* Object;
    T* operator->() const { return Object; }
    operator T*() const { return Object; }
};

namespace std {
template <class _Tp> class allocator { };
template <class _Tp, class _Alloc = allocator<_Tp> > class list {
public:
    void* _M_node;                                          // header node
    void push_back(const _Tp& __x);                         // 0x00479449
};
}

class CTCCreatureOpinionOfHero {
public:
    unsigned char                        _pad00[0x20];      // CTCBase + counters + PDef
    CDefPointer<COpinionPersonalityDef>  PPersonalityDef;   // +0x20
    unsigned char                        _pad24[0x338 - 0x24];
    std::list<COpinionTransientOffset>   TransientOffsets;  // +0x338
    void AdjustOpinionTransient(EOpinion opinion, float peakOffset, long framesToRunIn, long framesToRunOut,
                                float finalOffset, bool ignorePersonality);
};

void CTCCreatureOpinionOfHero::AdjustOpinionTransient(EOpinion opinion, float peakOffset, long framesToRunIn,
                                                      long framesToRunOut, float finalOffset,
                                                      bool ignorePersonality)
{
    const COpinionPersonalityDef* personality;
    if (!ignorePersonality && (personality = PPersonalityDef) != 0) {
        COpinionTransientOffset offset(opinion, peakOffset, framesToRunIn, framesToRunOut, finalOffset);
        TransientOffsets.push_back(COpinionTransientOffset(offset, personality->PersonalityTraits[opinion]));
    } else
        TransientOffsets.push_back(
            COpinionTransientOffset(opinion, peakOffset, framesToRunIn, framesToRunOut, finalOffset));
}