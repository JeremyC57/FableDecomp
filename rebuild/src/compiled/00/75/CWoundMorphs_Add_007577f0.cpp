// CWoundMorphs::Add(CWoundMorphs::CEntry& entry) -- append (entry.Location, entry) to the morph map's vector and mark the map dirty (re-sorted on the next lookup)
// Leaf: ?Add@CWoundMorphs@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x007577F0, 116 bytes (+12 int3), `ret 4`. Ego_r.pdb `?Add@CWoundMorphs@@QAEXAAVCEntry@1@@Z`.
// Callees: vector<pair<EBodyLocation,CEntry>>::_M_insert_overflow(pos, const T&, const __false_type&, size_type, bool)
//   @ 0x00759190 (__fastcall, `ret 14h`).
// Callers: none by direct reference (an out-of-line copy; the def loader that fills the morph table inlines it).
// Label: the manifest's `_Pop_heap_0<CDebugSphere*,CDebugSphere>` is a propagation; the Dirty byte at +0xD and the
//   0x2C stride are the wound-morph CVectorMap (see GetPRandomWoundForLocation 0x00757870, which sorts while Dirty).
// Layout (retail): CWoundMorphs::Morphs = CVectorMap {STLport vector first +0, finish +4, end_of_storage +8; Compare
//   +0xC; Dirty +0xD}; entries pair<EBodyLocation, CEntry> (0x2C), CEntry 40 bytes with Location first.
// Shape: the pair temporary is built FIRST in the frame (key = entry.Location dword, then the whole 40-byte entry as
//   `rep movsd` x10), then STLport `push_back(const T&)`: placement `_Construct` with its null test copies the pair
//   memberwise (user copy ctor), the overflow path passes (finish, &temp, &__false_type(), 1, true) with the empty
//   __false_type temporary homed in the dead `entry` argument slot (`lea edx,[esp+44h]`); Dirty = true after either path.
//   The push_back is written directly in Add: an inline CVectorMap::Add(key, value) helper is NOT inlined by VC7.1 (the
//   function becomes a 14-byte forwarder) unless it is __forceinline; __false_type needs a user-written empty ctor
//   (else `__false_type()` adds a zero byte store into its slot).
enum EBodyLocation { BODY_LOCATION_NONE = 0 };

inline void* __cdecl operator new(unsigned int, void* where) { return where; }

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
    void _M_insert_overflow(T* position, const T& x, const __false_type&, unsigned int fillLength, bool atEnd);   // 0x00759190
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

class CWoundMorphs {
public:
    class CEntry {                               // 40 bytes
    public:
        EBodyLocation Location;
        long          ReplacingTextureIndex;
        long          ReplacingBumpMapIndex;
        long          WoundTextureIndex;
        long          WoundBumpMapIndex;
        long          ScarTextureIndex;
        long          ScarBumpMapIndex;
        long          FadeToScarDuration;
        long          TotalDuration;
        float         PermanentScarStrength;
    };
    CVectorMap<EBodyLocation, CEntry, CKeyPairCompareLess<EBodyLocation, CEntry> > Morphs;   // +0x00
    void Add(CEntry& entry);
};

void CWoundMorphs::Add(CEntry& entry)
{
    Morphs.push_back(std::pair<EBodyLocation, CEntry>(entry.Location, entry));
    Morphs.Dirty = true;
}