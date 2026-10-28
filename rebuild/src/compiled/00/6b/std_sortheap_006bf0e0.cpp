// std::sort_heap(SMusicEntryPair* first, SMusicEntryPair* last, CKeyPairCompareLess comp) -- SGI sort_heap over the music manager's (long, CRegisteredMusicEntry) table: pop the root to the back until one element remains
// Leaf: sort_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006BF0E0, length 162 bytes, `ret 4`.
// Identity: the catalog label `_Sort_heap<pair<unsigned long,CSmallVector<NAnimationEvents::CEvent,32>>*,...>` is
// propagated over the byte-identical skeleton. Stride 0x18 (0x2AAAAAAB / sar 2); the callee 0x006BEDC0 (manifest
// "pair<long,CMusicManager::CRegisteredMusicEntry>") is the SGI `__pop_heap` of that pair (two CWideString assignments,
// copies and destructors, `ret 24h`) feeding the manifest's `_Adjust_heap<pair<long,CMusicManager::CRegisteredMusicEntry>
// *,...>` @ 0x006BEBF0. Ego_r.pdb CRegisteredMusicEntry (20 bytes): CWideString SrcFileName, DstFileName, float Gain,
// float Offset, bool CacheToHD.
// Callees: __pop_heap @ 0x006BEDC0 (out of line, __fastcall), CWideString copy constructor @ 0x0099B720 (x2).
// Shape: /Gr SGI algorithm TU -- `while (last - first > 1) pop_heap(first, last--, comp)` with pop_heap and
// __pop_heap_aux inline. The value copy in the argument area is the implicit pair copy: the key, then the entry's
// implicit copy off its own address (`lea edi,[eax+4]`: two wide-string copies, Gain/Offset as dword moves, the
// CacheToHD byte).
class CWideString {
public:
    void* PStringData;
    CWideString(const CWideString& other);              // 0x0099B720
    ~CWideString();                                     // 0x0099B510
    const CWideString& operator=(const CWideString& other);   // 0x0099B7D0
};
class CMusicManager { public:
    class CRegisteredMusicEntry { public:                      // Ego_r.pdb, 20 bytes
        CWideString SrcFileName;                        // +0x00
        CWideString DstFileName;                        // +0x04
        float       Gain;                               // +0x08
        float       Offset;                             // +0x0c
        bool        CacheToHD;                          // +0x10
    };
};
struct SMusicEntryPair {                                // std::pair<long, CMusicManager::CRegisteredMusicEntry> (0x18 bytes)
    long                                 first;         // +0x00
    CMusicManager::CRegisteredMusicEntry second;        // +0x04
};
struct CKeyPairCompareLess { };                         // empty, passed by value

void __fastcall __pop_heap(SMusicEntryPair* first, SMusicEntryPair* last, SMusicEntryPair* result, SMusicEntryPair value,
                           CKeyPairCompareLess comp, int*);   // 0x006BEDC0

inline void __pop_heap_aux(SMusicEntryPair* first, SMusicEntryPair* last, SMusicEntryPair*, CKeyPairCompareLess comp)
{
    __pop_heap(first, last - 1, last - 1, SMusicEntryPair(*(last - 1)), comp, (int*)0);
}

inline void pop_heap(SMusicEntryPair* first, SMusicEntryPair* last, CKeyPairCompareLess comp)
{
    __pop_heap_aux(first, last, (SMusicEntryPair*)0, comp);
}

void __fastcall sort_heap(SMusicEntryPair* first, SMusicEntryPair* last, CKeyPairCompareLess comp)
{
    while (last - first > 1)
        pop_heap(first, last--, comp);
}