// _global::CopyBackward_PairLongMusicEntry_006beaa0(pair<long,CMusicManager::CRegisteredMusicEntry>*, pair<..>*, pair<..>*, const random_access_iterator_tag&, int*) -- SGI __copy_backward over the music manager's (id, registered entry) pairs
// Leaf: CopyBackward_PairLongMusicEntry_006beaa0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006BEAA0, length 134 bytes.
// SGI-STL random-access `__copy_backward(first, last, result, random_access_iterator_tag, Distance*)`, /Gr TU (__fastcall: first
// in ecx, last in edx, result + the tag reference + the distance pointer on the stack, `ret 0Ch`); the count is
// (last - first) / sizeof(T) by reciprocal multiply, the loop counts it down and the moved-back result is returned.
// Element: std::pair<long, CMusicManager::CRegisteredMusicEntry> (the manifest label; 0x18 bytes). Ego_r.pdb
// CRegisteredMusicEntry (0x14): CWideString SrcFileName +0, CWideString DstFileName +4, float Gain +8, float Offset
// +0xC, bool CacheToHD +0x10. The implicit operator= copies the key, calls CWideString::operator= @ 0x0099B7D0 for the
// two file names, then Gain, Offset and CacheToHD.
class CWideString { public:                         // one-word handle
    void* m_pData;                           // +0x00
    const CWideString& operator=(const CWideString& other);     // 0x0099B7D0
};

struct CRegisteredMusicEntry {               // CMusicManager::CRegisteredMusicEntry, 0x14 bytes
    CWideString SrcFileName;                 // +0x00
    CWideString DstFileName;                 // +0x04
    float       Gain;                        // +0x08
    float       Offset;                      // +0x0C
    bool        CacheToHD;                   // +0x10
};

struct PairLongMusicEntry {                  // std::pair<long, CMusicManager::CRegisteredMusicEntry>
    long                  first;             // +0x00
    CRegisteredMusicEntry second;            // +0x04
};

struct random_access_iterator_tag { random_access_iterator_tag() { } };

PairLongMusicEntry* __fastcall CopyBackward_PairLongMusicEntry_006beaa0(PairLongMusicEntry* first, PairLongMusicEntry* last, PairLongMusicEntry* result, const random_access_iterator_tag& tag, int* distance)
{
    for (int n = last - first; n > 0; --n)
        *--result = *--last;
    return result;
}