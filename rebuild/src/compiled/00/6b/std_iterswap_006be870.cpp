// std::iter_swap(pair<long, CMusicManager::CRegisteredMusicEntry>* a, pair<long, CMusicManager::CRegisteredMusicEntry>* b) -- STLport three-copy swap of two registered-music map entries
// Leaf: ??$iter_swap@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006BE870, 185 bytes (+ int3 padding), plain `ret`.
// Callers: the CMusicManager registered-music sort helpers (__unguarded_partition 0x006BED50 and its siblings).
// Callees: CWideString::CWideString(const CWideString&) @ 0x0099B720, CWideString::operator=(const CWideString&)
//   @ 0x0099B7D0, CWideString::~CWideString() @ 0x0099B510.
// Types (Ego_r.pdb): CMusicManager::CRegisteredMusicEntry = SrcFileName CWideString +0, DstFileName CWideString +4, Gain
//   +8, Offset +0xC, CacheToHD bool +0x10 (20 bytes); the 24-byte value is pair<long, CRegisteredMusicEntry> (key +0,
//   entry +4), the CVectorMap element of CMusicManager::RegisteredMusic (landed RegisterMusic 0x006BE080 /
//   __uninitialized_fill_n 0x006BE770 models).
// Shape: STLport 4.x `iter_swap(a, b)` from a /Gr TU (`__fastcall`: a in ecx, b in edx, nothing on the stack) with
//   `__iter_swap(a, b, _Tp*)` inlined (`__forceinline`: a plain inline one stays a call, 13v185):
//   `_Tp tmp = *a; *a = *b; *b = tmp;` -- the frame temporary built by STLport's
//   user pair copy constructor over the entry's memberwise user copy constructor (two string copies, then the three
//   POD words), both assignments the IMPLICIT memberwise operator= (key, two string assignments, the POD words), then
//   the temporary's two string destructors (DstFileName first).
// Label correction: the catalog `swap<CScriptInfo>` is propagated; the 24-byte stride, the CWideString pair and the
//   callers in the CMusicManager sort name the element.
class CWideString {
public:
    void* PStringData;                                  // +0x00
    CWideString(const CWideString& other);              // 0x0099B720
    const CWideString& operator=(const CWideString& other);   // 0x0099B7D0
    ~CWideString();                                     // 0x0099B510
};

class CMusicManager {
public:
    class CRegisteredMusicEntry {                       // 20 bytes
    public:
        CWideString SrcFileName;                        // +0x00
        CWideString DstFileName;                        // +0x04
        float       Gain;                               // +0x08
        float       Offset;                             // +0x0C
        bool        CacheToHD;                          // +0x10
        CRegisteredMusicEntry(const CRegisteredMusicEntry& o)
            : SrcFileName(o.SrcFileName), DstFileName(o.DstFileName), Gain(o.Gain), Offset(o.Offset), CacheToHD(o.CacheToHD) {}
    };
};

namespace std {
template <class T1, class T2>
struct pair {
    T1 first;
    T2 second;
    pair(const pair& o) : first(o.first), second(o.second) { }
};

template <class ForwardIter1, class ForwardIter2, class T>
__forceinline void __iter_swap(ForwardIter1 a, ForwardIter2 b, T*)
{
    T tmp = *a;
    *a = *b;
    *b = tmp;
}

template <class ForwardIter1, class ForwardIter2>
void __fastcall iter_swap(ForwardIter1 a, ForwardIter2 b)
{
    __iter_swap(a, b, (pair<long, CMusicManager::CRegisteredMusicEntry>*)0);
}

template void __fastcall iter_swap<pair<long, CMusicManager::CRegisteredMusicEntry>*, pair<long, CMusicManager::CRegisteredMusicEntry>*>(
    pair<long, CMusicManager::CRegisteredMusicEntry>*, pair<long, CMusicManager::CRegisteredMusicEntry>*);
}