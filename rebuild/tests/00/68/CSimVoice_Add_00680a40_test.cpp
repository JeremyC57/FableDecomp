// Behaviour test for 00680a40: src/00680a40.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CSimVoice::Add(EReactionSpeechType type, std::vector<long>& samples)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CSimVoice::Add(EReactionSpeechType type, std::vector<long>& samples) -- register every sample index of the list under the reaction-speech type, then re-sort the voice map if anything was added
// Retail: Fable.exe (TLC, VC7.1) @ 0x00680A40, 162 bytes, `ret 8` (PDB
//   `?Add@CSimVoice@@QAEXW4EReactionSpeechType@@AAV?$vector@JV?$allocator@J@std@@@std@@@Z`).
// Leaf: ?Add@CSimVoice
// Callees: STLport vector<pair<EReactionSpeechType,long> >::_M_insert_overflow @ 0x00680E60 (manifest
//   "std::vector_insert_pairs"), SGI sort<pair*, CKeyPairCompareLess> @ 0x006813A0 (`__fastcall` under /Gr: first in ecx,
//   last in edx, the empty comparator by value on the stack; manifest "CNavigationMesh_SortAndHeapify" is propagated).
// Shape: CVectorMap::Add inlined per sample: the pair temporary {type, samples[i]} (its loop-invariant first word stored
//   once before the loop), STLport push_back (in-place copy forwarding both words from their sources, else the reallocating
//   insert with the `__true_type` tag homed in the type argument's slot), then Dirty = true; the indexed walk re-reads
//   size() each pass. After the loop the map's inline Sort() runs only when dirty.
// Fix (retry 10): STLport pair declares its own copy constructor (the "VC++ 6.0 bug workaround" member): with it VC7.1
//   copies the pair's first word straight from the `type` argument slot in the in-place push_back (and homes the tag there);
//   the growth member has a same-TU body.
// Layout = Ego_r.pdb CSimVoice (28 bytes): Voices +0x08 (CVectorMap: 12-byte vector, Compare +0x14, Dirty +0x15).
typedef long EReactionSpeechType;

struct __true_type { __true_type() { } };

template <class T1, class T2> struct pair {             // STLport pair
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) {}
    pair(const pair& p) : first(p.first), second(p.second) {}   // STLport declares this copy ctor ("VC++ bug workaround")
};

template <class T> struct vector {                      // retail STLport vector: 12 bytes
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    T* begin() { return _M_start; }
    T* end() { return _M_finish; }
    T& operator[](unsigned long n) { return *(begin() + n); }
    void _M_insert_overflow(T* pos, const T& x, const __true_type& tag, unsigned long fill_len, bool atend);   // 0x00680E60
    void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) T(x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, x, __true_type(), 1, true);
    }
};
void* GrowVectorStorage_s7(void* vector, unsigned long count);

#pragma auto_inline(off)
// Same-TU body of the vector growth member (_M_insert_overflow, instantiated in retail's TU): the new storage receives the value
// read through its const reference; the empty tag is never touched.
template <class T>
__declspec(noinline) void vector<T>::_M_insert_overflow(T* pos, const T& x, const __true_type& tag, unsigned long fill_len, bool atend)
{
    T* p = (T*)GrowVectorStorage_s7(this, fill_len);
    if (p)
        new (p) T(x);
}
#pragma auto_inline(on)

inline void* operator new(unsigned int, void* p) { return p; }

template <class K, class V> class CKeyPairCompareLess { public: };

void __fastcall sort(pair<EReactionSpeechType, long>* first, pair<EReactionSpeechType, long>* last,
                     CKeyPairCompareLess<EReactionSpeechType, long> comp);   // 0x006813A0

template <class K, class V, class C> class CVectorMap { public:
    vector< pair<K, V> > Data;                          // +0x00
    C                    Compare;                       // +0x0c
    bool                 Dirty;                         // +0x0d
    void Add(const K& key, const V& value) { Data.push_back(pair<K, V>(key, value)); Dirty = true; }
    void Sort()
    {
        if (Dirty) {
            sort(Data.begin(), Data.end(), Compare);
            Dirty = false;
        }
    }
};

class CSimVoice { public:
    long _padding_[2];                                  // +0x00
    CVectorMap<EReactionSpeechType, long, CKeyPairCompareLess<EReactionSpeechType, long> > Voices;   // +0x08
    void Add(EReactionSpeechType type, vector<long>& samples);
};

void CSimVoice::Add(EReactionSpeechType type, vector<long>& samples)
{
    for (unsigned long i = 0; i < samples.size(); ++i)
        Voices.Add(type, samples[i]);
    Voices.Sort();
}

// ---- stubs and driver ----

#include <stdlib.h>
static int g_grow = 0;
void* GrowVectorStorage_s7(void* vector, unsigned long count)
{
    ++g_grow; char** v = (char**)vector;
    unsigned long used = (unsigned long)(v[1] - v[0]); unsigned long cap = (unsigned long)(v[2] - v[0]);
    char* block = (char*)malloc(cap * 2 + 8 * count + 16);
    if (v[0]) { memcpy(block, v[0], used); free(v[0]); }
    v[0] = block; v[1] = block + used + 8 * count; v[2] = block + cap * 2 + 8 * count + 16;
    return block + used;
}
static int g_sorts = 0; static long g_sortCount = 0;
void __fastcall sort(pair<EReactionSpeechType, long>* first, pair<EReactionSpeechType, long>* last,
                     CKeyPairCompareLess<EReactionSpeechType, long> comp)
{ ++g_sorts; g_sortCount = (long)(last - first); }
int main()
{
    static CSimVoice voice; static long sampleStore[5] = { 11, 22, 33, 44, 55 };
    vector<long> samples; samples._M_start = sampleStore; samples._M_finish = sampleStore + 3; samples._M_end_of_storage = sampleStore + 5;
    voice.Voices.Data._M_start = voice.Voices.Data._M_finish = voice.Voices.Data._M_end_of_storage = 0; voice.Voices.Dirty = false;
    voice.Add(7, samples);
    pair<EReactionSpeechType, long>* d = voice.Voices.Data._M_start;
    CHECK(voice.Voices.Data._M_finish - d == 3); CHECK(d[0].first == 7 && d[0].second == 11); CHECK(d[1].first == 7 && d[1].second == 22);
    CHECK(d[2].first == 7 && d[2].second == 33); CHECK(g_grow >= 1); CHECK(g_sorts == 1); CHECK(g_sortCount == 3); CHECK(voice.Voices.Dirty == false);
    samples._M_finish = sampleStore + 5; int grows = g_grow;
    voice.Add(2, samples); d = voice.Voices.Data._M_start;
    CHECK(voice.Voices.Data._M_finish - d == 8); CHECK(d[3].first == 2 && d[3].second == 11); CHECK(d[7].first == 2 && d[7].second == 55);
    CHECK(g_sorts == 2); CHECK(g_sortCount == 8);
    samples._M_finish = sampleStore; voice.Add(3, samples); CHECK(g_sorts == 2); CHECK(voice.Voices.Data._M_finish - voice.Voices.Data._M_start == 8);   // nothing added: no sort
    if (fails == 0) printf("00680a40_TEST PASS\n"); else printf("00680a40_TEST FAIL\n");
    return fails;
}