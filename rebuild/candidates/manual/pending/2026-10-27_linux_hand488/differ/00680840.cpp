// CSimVoice::GetRandomSampleIndexVariation(EReactionSpeechType, unsigned long&) const -- pick one of the speech type's sample indices with the caller's random seed
// Grade: DIFFER(114v114) -- residue: the third by-value copy of `type` (GetSampleIndexVariation's argument for LowerBound) is stored to the dead `seed` slot [esp+0x18] instead of retail's own `type` slot [esp+0x14] (lea edx + mov store, 2 bytes); every other byte matches; tried named locals, direct LowerBound, non-fastcall/non-forceinline GFRandom, indexed iterator, /Os
// Leaf: ?GetRandomSampleIndexVariation@CSimVoice@@QBEJW4EReactionSpeechType@@AAK@Z
// Retail: Fable.exe @ 0x00680840, 114 bytes, thiscall `ret 8`.
// Identity (proven): Ego_r.pdb names ?GetRandomSampleIndexVariation@CSimVoice@@QBEJW4EReactionSpeechType@@AAK@Z next to
// GetNoOfSampleIndexVariations / GetSampleIndexVariation (both by-value type); the row sits between the landed
// CSimVoice::TransferBinaryOut 0x006808C0 / HasVoice 0x00680910 and uses CSimVoice::Voices (Ego_r +8: CVectorMap<
// EReactionSpeechType, long>) through its UpperBound 0x00680500 and LowerBound 0x006804C0 (both thiscall, `ret 4`).
// The world LCG (`seed * 0x24A1 + 0x24DF`, then GFROR13 0x00497890) is the inline GFRandom(range, &seed) of the PDB.
// Callees: CVectorMap<EReactionSpeechType,long,...>::UpperBound 0x00680500 / LowerBound 0x006804C0 (the PDB const pair;
// retail STLport's const_iterator is `const value_type*`), GFROR13 0x00497890.
// Shape: the inline helpers take the type by value, so it is re-stored to its own argument slot before each bound call;
// `UpperBound - LowerBound` evaluates left to right; the zero-range test is GFRandom's `range ? seed % range : 0`.
enum EReactionSpeechType { REACTION_SPEECH_NONE = 0 };

long __fastcall GFROR13(unsigned long value);       // 0x00497890

__forceinline unsigned long __fastcall GFRandom(unsigned long range, unsigned long* seed)
{
    *seed = *seed * 0x24a1 + 0x24df;
    *seed = GFROR13(*seed);
    return range ? *seed % range : 0;
}

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
}
template <class K, class V> class CKeyPairCompareLess { public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};
template <class K, class V, class C> class CVectorMap { public:   // retail 0x10 bytes
    typedef std::pair<K, V> value_type;
    value_type* First;                              // +0x00 STLport vector
    value_type* Last;                               // +0x04
    value_type* EndOfStorage;                       // +0x08
    C           Compare;                            // +0x0C
    bool        Dirty;                              // +0x0D
    const value_type* LowerBound(const K& key) const;      // 0x006804C0
    const value_type* UpperBound(const K& key) const;      // 0x00680500
};

class CSimVoice { public:
    unsigned char _pad_0x00[8];                     // +0x00 (unnamed in Ego_r)
    CVectorMap<EReactionSpeechType, long, CKeyPairCompareLess<EReactionSpeechType, long> > Voices;   // +0x08
    long GetNoOfSampleIndexVariations(EReactionSpeechType type) const
    {
        return Voices.UpperBound(type) - Voices.LowerBound(type);
    }
    long GetSampleIndexVariation(EReactionSpeechType type, long index) const
    {
        return (Voices.LowerBound(type) + index)->second;
    }
    long GetRandomSampleIndexVariation(EReactionSpeechType type, unsigned long& seed) const;
};

long CSimVoice::GetRandomSampleIndexVariation(EReactionSpeechType type, unsigned long& seed) const
{
    return GetSampleIndexVariation(type, GFRandom(GetNoOfSampleIndexVariations(type), &seed));
}
