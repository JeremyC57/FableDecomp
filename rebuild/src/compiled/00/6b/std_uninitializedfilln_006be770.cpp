// std::__uninitialized_fill_n(pair<long, CMusicManager::CRegisteredMusicEntry>* first, unsigned int n, const pair<..>& x, const __false_type&) -- copy-construct n copies of a registered music entry into raw storage at first and return first + n
// Leaf: __uninitialized_fill_n
// Retail: Fable.exe @ 0x006BE770, 99 bytes, `ret 8`: __fastcall (first ecx -> ebx, n edx -- kept in the `push ecx` frame
// word -- x and the empty tag by reference on the stack), returning the end pointer in eax. Only call site 0x006BF26F.
// Types (Ego_r.pdb): CMusicManager::CRegisteredMusicEntry (20 bytes) = SrcFileName CWideString +0, DstFileName
//   CWideString +4, Gain +8, Offset +0xC, CacheToHD bool +0x10; the pair is 24 bytes (first long +0, second +4).
// Callees: CWideString::CWideString(const CWideString&) @ 0x0099B720 (twice).
// Shape: STLport's pair copy constructor ("VC++ needs this") over the entry's implicit memberwise copy constructor
// (CWideString declares one): the key, the two string copy calls, Gain, Offset and the flag byte. That nesting keeps the
// second induction register at second.DstFileName + 4 (`lea esi,[ebx+0Ch]`, GOTCHAS 00769000/00720310); with every
// register taken, n counts down in its frame word.
#include <new>
struct CWideString {
    void* m_pStringData;                        // +0x00
    CWideString(const CWideString& other);      // 0x0099B720
};
struct CMusicManager {
    struct CRegisteredMusicEntry {              // 20 bytes
        CWideString SrcFileName;                // +0x00
        CWideString DstFileName;                // +0x04
        float       Gain;                       // +0x08
        float       Offset;                     // +0x0C
        bool        CacheToHD;                  // +0x10
    };
};
namespace std {
template <class T1, class T2>
struct pair {
    T1 first;
    T2 second;
    pair(const pair<T1, T2>& o) : first(o.first), second(o.second) {}
};
}
typedef std::pair<long, CMusicManager::CRegisteredMusicEntry> CMusicEntryPair;

struct __false_type { __false_type() {} };
namespace std {
template <class ForwardIter, class Size, class T>
ForwardIter __fastcall __uninitialized_fill_n(ForwardIter first, Size n, const T& x, const __false_type&)
{
    ForwardIter cur = first;
    for (; n > 0; --n, ++cur)
        new (cur) T(x);
    return cur;
}
template CMusicEntryPair* __fastcall __uninitialized_fill_n(CMusicEntryPair*, unsigned int, const CMusicEntryPair&, const __false_type&);
}