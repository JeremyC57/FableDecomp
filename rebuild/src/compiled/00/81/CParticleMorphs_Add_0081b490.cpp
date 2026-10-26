// CParticleMorphs::Add(const CParticleMorphs::CEntry& entry) -- append (entry.MorphType, entry) to the particle-morph map's vector, then re-sort the map by morph type
// Leaf: ?Add@CParticleMorphs@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0081B490, 122 bytes (+int3 pad), `ret 4`. Ego_r.pdb `?Add@CParticleMorphs@@QAEXABVCEntry@1@@Z`.
//   No direct caller or reference (an out-of-line copy; the def loader inlines it).
// Callees: vector<pair<EHeroMorphType, CEntry> >::_M_insert_overflow(pos, const T&, const __false_type&, size_type, bool)
//   @ 0x0081C990 (`ret 14h`), sort<pair<EHeroMorphType, CEntry>*, CKeyPairCompareLess> @ 0x0081E090 (landed, __fastcall
//   first/last in ecx/edx, the 1-byte comparator by value).
// Label: the catalog's "_Pop_heap_0<CDebugSphere*,CDebugSphere>" is propagated; the PDB names CParticleMorphs::Add(const
//   CEntry&) and the body is the CWoundMorphs::Add 0x007577F0 shape over the 40-byte particle-morph pairs (Ego_r.pdb
//   CParticleMorphs::Morphs = CVectorMap<EHeroMorphType, CEntry>; retail STLport vector 12 bytes, Compare +0xC, Dirty +0xD).
// Shape: the pair temporary is built FIRST in the frame (key = entry.MorphType, then the whole 36-byte entry as
//   `rep movsd` x9), STLport `push_back` is inline (placement copy with its null test, `add [ebx+4],28h`; else
//   _M_insert_overflow with the empty __false_type homed in the dead `entry` slot), then `Dirty = true` and the map's inline
//   Sort -- `if (Dirty) { sort(begin, end, Compare); Dirty = false; }`, the test folded -- which passes the 1-byte comparator
//   by value (`xor edx,edx ; mov dl,[ebx+0Ch] ; push edx`).
enum EHeroMorphType { EHeroMorphType_FIRST };

inline void* __cdecl operator new(unsigned int, void* where) { return where; }

class CDefString { public:                                         // 4 bytes: the definition string-table position
    long TablePos;
};

namespace std {
struct __false_type { __false_type() {} };

template <class A, class B> struct pair {
    A first;                                     // +0x00
    B second;                                    // +0x04
    pair(const A& a, const B& b) : first(a), second(b) {}
    pair(const pair& other) : first(other.first), second(other.second) {}
};

template <class T> class vector {
public:
    T* _M_start;                                 // +0x00
    T* _M_finish;                                // +0x04
    T* _M_end_of_storage;                        // +0x08
    void _M_insert_overflow(T* position, const T& x, const __false_type&, unsigned int fillLength, bool atEnd);   // 0x0081C990
    void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) T(x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, x, __false_type(), 1UL, true);
    }
};
}

template <class K, class V> class CKeyPairCompareLess { public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};

template <class K, class V, class C> class CVectorMap : public std::vector<std::pair<K, V> > {
public:
    C    Compare;                                // +0x0C
    bool Dirty;                                  // +0x0D
};

class CParticleMorphs {
public:
    class CEntry {                               // Ego_r.pdb, 36 bytes
    public:
        EHeroMorphType MorphType;                // +0x00
        bool       Blending;                     // +0x04
        long       Location;                     // +0x08 EMeshLocationPoint
        CDefString LocationName;                 // +0x0c
        long       LocationIndex;                // +0x10
        long       ParticleIndex;                // +0x14
        long       SecondParticleIndex;          // +0x18
        float      StartAtMorphStrength;         // +0x1c
        float      EndAtMorphStrength;           // +0x20
    };
    typedef CKeyPairCompareLess<EHeroMorphType, CEntry> TCompare;
    typedef std::pair<EHeroMorphType, CEntry> TPair;
    CVectorMap<EHeroMorphType, CEntry, TCompare> Morphs;   // +0x00
    void Add(const CEntry& entry);
};

void __fastcall sort(CParticleMorphs::TPair* first, CParticleMorphs::TPair* last, CParticleMorphs::TCompare comp);   // 0x0081E090

void CParticleMorphs::Add(const CEntry& entry)
{
    Morphs.push_back(TPair(entry.MorphType, entry));
    Morphs.Dirty = true;
    if (Morphs.Dirty) {
        sort(Morphs._M_start, Morphs._M_finish, Morphs.Compare);
        Morphs.Dirty = false;
    }
}