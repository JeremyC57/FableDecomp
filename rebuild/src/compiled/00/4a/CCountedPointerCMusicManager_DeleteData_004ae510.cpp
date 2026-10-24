// CCountedPointer<CMusicManager>::DeleteData(void* data) -- the counted-pointer deleter for the world's music manager: destroy it (registered-music vector freed, sound-def pointer released) and free it
// Leaf: ?DeleteData@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004AE510, 50 bytes (64 with padding), plain `ret` (static __fastcall: data
//   in ecx).
// Identity: Ego_r.pdb `?DeleteData@?$CCountedPointer@VCMusicManager@@@@SIXPAX@Z` (the family's mangling); its address
//   is stored as the deleter of the count block by CCountedPointer<CMusicManager>::Reset 0x004AE650 (`mov [eax+4],
//   0x004AE510`), which CWorld calls on PMusicManager +0x6C right after `new CMusicManager` (0x40 bytes, ctor 0x006BE300).
// Callees: 0x004AE040 ~vector<pair<long, CMusicManager::CRegisteredMusicEntry> > (the CVectorMap's 24-byte entries,
//   two CWideString dtors each, then free), the sound def's slot 1 (Destroy) on its last release, 0x00BFE9BC operator
//   delete.
// Layout: Ego_r.pdb CMusicManager, retail STLport sizes: CState CurrentState +0 (24 bytes, trivial), CDefPointer<const
//   CSoundDef> PSoundDef +0x18, CVectorMap<long, CRegisteredMusicEntry> RegisteredMusic +0x1C (vector 12 + comparator),
//   CurrentMusicID +0x2C, PendingMusicID +0x30, PPendingFadeTimes +0x34, PendingLoopCount +0x38, Paused +0x3C (0x40).
// Shape: `delete (CMusicManager*)data;` (null-tested) with the implicit ~CMusicManager inlined in reverse member order:
//   the vector-map's out-of-line vector dtor, then the def pointer's `if (m_p) { m_p->Release(); m_p = 0; }` with the
//   counted release on the def (`if (--m_count == 0) Destroy();`, ecx kept for the virtual call).
// Label correction: catalogued as `DeleteData` (right name); the class is CCountedPointer<CMusicManager>.
struct CWideString {
    void* PStringData;
    ~CWideString();                                  // 0x0099B510
};

class CDefBase {
public:
    virtual void v00();
    virtual void Destroy();                          // slot 1
    mutable long m_count;                            // +0x04
    void Release() const { if (--m_count == 0) const_cast<CDefBase*>(this)->Destroy(); }
};
class CSoundDef : public CDefBase { };

template <class T> class CDefPointer {
public:
    T* m_p;
    ~CDefPointer() { if (m_p) { m_p->Release(); m_p = 0; } }
};

class CMusicManager {
public:
    struct CState {
        long RegionSet;                              // +0x00
        long OverrideSet;                            // +0x04
        long UIOverrideSet;                          // +0x08
        long CinematicOverrideSet;                   // +0x0C
        long MapCellSet;                             // +0x10
        bool Paused;                                 // +0x14
        bool Danger;                                 // +0x15
    };
    struct CRegisteredMusicEntry {
        CWideString SrcFileName;                     // +0x00
        CWideString DstFileName;                     // +0x04
        float       Gain;                            // +0x08
        float       Offset;                          // +0x0C
        bool        CacheToHD;                       // +0x10
    };
    struct CEntryPair { long first; CRegisteredMusicEntry second; };   // pair<long, CRegisteredMusicEntry>, 0x18 bytes
    struct CEntryVector {                            // STLport vector<CEntryPair>
        CEntryPair* _M_start;
        CEntryPair* _M_finish;
        CEntryPair* _M_end_of_storage;
        ~CEntryVector();                             // 0x004AE040
    };
    struct CRegisteredMusicMap {                     // CVectorMap<long, CRegisteredMusicEntry, CKeyPairCompareLess<...> >
        CEntryVector Data;                           // +0x00
        char         Compare;                        // +0x0C
    };
    struct CFadeTimes;

    CState                       CurrentState;       // +0x00
    CDefPointer<const CSoundDef> PSoundDef;          // +0x18
    CRegisteredMusicMap          RegisteredMusic;    // +0x1C
    long                         CurrentMusicID;     // +0x2C
    long                         PendingMusicID;     // +0x30
    CFadeTimes*                  PPendingFadeTimes;  // +0x34
    long                         PendingLoopCount;   // +0x38
    bool                         Paused;             // +0x3C
};

template <class T> class CCountedPointer {
public:
    T*    Data;
    void* Count;
    static void __fastcall DeleteData(void* data) { delete (T*)data; }
};

template void __fastcall CCountedPointer<CMusicManager>::DeleteData(void*);