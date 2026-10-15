// CMusicManager::CMusicManager() -- constructor: clear the music state, the sound def pointer, the registered-music map and the current/pending IDs, look up the "MAIN_SOUND_SETUP" sound def, register the music table and start unpaused
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006BE300, length 115 bytes.
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0, CGameDefinitionManager::Get() @ 0x0044C6B0 (static __fastcall singleton),
// CDefinitionManager::GetDef<CSoundDef>(const CCharString&, CDefPointer&) @ 0x004196B2 (manifest: a byte-identical GetDef instantiation),
// CCharString::~CCharString() @ 0x0099EAE0, CMusicManager::RegisterMusic() @ 0x006BE080. String literal @ 0x0122EF98 "MAIN_SOUND_SETUP".
// Label correction: the manifest's "CEngine::CEngine" is a propagated name; the only caller is CWorld::PostInit (0x004A6767, after a
// null-checked allocation) and the member pattern is the PDB CMusicManager (0x44 bytes): CurrentState (CState: five longs + Paused/Danger
// bytes) +0x00, PSoundDef +0x18, RegisteredMusic (CVectorMap: 12-byte retail vector, Compare +0x28, Dirty +0x29) +0x1c, CurrentMusicID
// +0x2c, PendingMusicID +0x30, PPendingFadeTimes +0x34, PendingLoopCount +0x38 (left unset), Paused +0x3c (retail = PDB - 4).
// Shape: one `push ecx` frame word for the CCharString local; the member zeroes come from ebx in declaration order (the CDefPointer
// address is kept in edi for the GetDef out argument); the GetDef arguments are pushed BEFORE the singleton call (the chained
// `Get().GetDef(name, PSoundDef)`); RegisterMusic runs on `this` after the string is destroyed, and Paused is a body assignment after it.
// Leaf: ??0CMusicManager
class CCharString {
public:
    CCharString(const char* text, long length);   // 0x0099EBF0
    ~CCharString();                               // 0x0099EAE0
    struct CCharStringData* PStringData;
};

struct CSoundDef;

template <class T>
struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) {}
};

struct CDefinitionManager_Calls {
    template <class T> bool GetDef(const CCharString& name, CDefPointer<T>& out);   // 0x004196B2 for T = const CSoundDef
};

struct CGameDefinitionManager : CDefinitionManager_Calls {
    static CGameDefinitionManager& __fastcall Get();   // 0x0044C6B0
};

template <class T>
struct SRetailAllocProxy {                  // STLport _STLP_alloc_proxy: the end-of-storage pointer
    T* m_data;
    SRetailAllocProxy(T* data) : m_data(data) {}
};

template <class T>
struct CRetailVector {                      // STLport vector (_Vector_base): 12 bytes
    T* m_start;
    T* m_finish;
    SRetailAllocProxy<T> m_endOfStorage;
    CRetailVector() : m_start(0), m_finish(0), m_endOfStorage(0) {}
};

struct CMusicState {                        // CMusicManager::CState (0x18 bytes)
    long RegionSet;
    long OverrideSet;
    long UIOverrideSet;
    long CinematicOverrideSet;
    long MapCellSet;
    bool Paused;
    bool Danger;
    CMusicState() : RegionSet(0), OverrideSet(0), UIOverrideSet(0), CinematicOverrideSet(0), MapCellSet(0), Paused(false), Danger(false) {}
};

struct CRegisteredMusicEntry {
    long m_key;
    long m_value[2];
};

struct CKeyPairCompareLess { };

struct CRegisteredMusicMap {                // CVectorMap<long, CRegisteredMusicEntry, CKeyPairCompareLess>
    CRetailVector<CRegisteredMusicEntry> m_entries;   // +0x00
    CKeyPairCompareLess                  Compare;     // +0x0c
    bool                                 Dirty;       // +0x0d
    CRegisteredMusicMap() : Dirty(false) {}
};

struct CFadeTimes;

struct CMusicManager {
    CMusicState                  CurrentState;          // +0x00
    CDefPointer<const CSoundDef> PSoundDef;             // +0x18
    CRegisteredMusicMap          RegisteredMusic;       // +0x1c
    long                         CurrentMusicID;        // +0x2c
    long                         PendingMusicID;        // +0x30
    CFadeTimes*                  PPendingFadeTimes;     // +0x34
    long                         PendingLoopCount;      // +0x38
    bool                         Paused;                // +0x3c
    CMusicManager();
    void RegisterMusic();                               // 0x006BE080
};

CMusicManager::CMusicManager()
    : CurrentMusicID(0), PendingMusicID(0), PPendingFadeTimes(0)
{
    {
        CCharString name("MAIN_SOUND_SETUP", -1);
        CGameDefinitionManager::Get().GetDef(name, PSoundDef);
    }
    RegisterMusic();
    Paused = false;
}
