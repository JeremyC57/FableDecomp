// CMusicManager::UpdateCurrentMusicOffset() -- while the sound system's music is up, store the current track's playback position into its registered-music entry
// Leaf: ?UpdateCurrentMusicOffset@CMusicManager@@IAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006BD8A0, 91 bytes (+ int3 padding), plain `ret` (thiscall).
// Callees: CVectorMap<long, CRegisteredMusicEntry>::LowerBound(const long&) @ 0x006BE5E0, the sound system's virtual
//   slot 49 (+0xC4) on the global sound system @ 0x013B8394 (fills a float, returns bool; name from use).
// Identity: Ego_r.pdb ?UpdateCurrentMusicOffset@CMusicManager@@IAEXXZ (the only CMusicManager member that writes an
//   entry's Offset).
// Layout: CMusicManager (Ego_r.pdb, retail 12-byte vector; the landed ctor 0x006BE300 model): CurrentState +0, PSoundDef
//   +0x18, RegisteredMusic +0x1C (vector First +0x1C / Last +0x20, Compare +0x28, Dirty +0x29), CurrentMusicID +0x2C;
//   the 24-byte value pair<long, CRegisteredMusicEntry> = key +0, SrcFileName +4, DstFileName +8, Gain +0xC, Offset +0x10,
//   CacheToHD +0x14. The sound system's byte at +0x25 gates the update (descriptive name).
// Shape: the global read once into a local (ebp, reused after the LowerBound call); the inline CVectorMap::Find as the
//   ternary `(it == Last || key < it->first) ? Last : it` (signed `jl`), the caller re-reading `Last` for its `!=
//   end()` test (`cmp esi,[ebx+20h]`); the position lands in a `push ecx` frame float.
// Label correction: the catalog `Music_UpdateVolume` is descriptive.
class CWideString {
public:
    void* PStringData;                                  // +0x00
};

class CSoundSystem {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48();
    virtual bool GetMusicPosition(float* seconds);      // slot 49 (+0xC4)
    unsigned char _pad_0x04[0x21];
    bool          m_musicEnabled;                       // +0x25
};
extern CSoundSystem* g_pSoundSystem;                    // 0x013B8394

namespace std {
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
};
}

class CMusicManager {
public:
    class CRegisteredMusicEntry {                       // 20 bytes
    public:
        CWideString SrcFileName;                        // +0x00
        CWideString DstFileName;                        // +0x04
        float       Gain;                               // +0x08
        float       Offset;                             // +0x0C
        bool        CacheToHD;                          // +0x10
    };
    struct CRegisteredMusicMap {                        // CVectorMap<long, CRegisteredMusicEntry, CKeyPairCompareLess<..> >
        typedef std::pair<long, CRegisteredMusicEntry> value_type;
        value_type*   First;                            // +0x00
        value_type*   Last;                             // +0x04
        value_type*   EndOfStorage;                     // +0x08
        unsigned char Compare;                          // +0x0C
        bool          Dirty;                            // +0x0D
        value_type* LowerBound(const long& key);        // 0x006BE5E0
        value_type* Find(const long& key)
        {
            value_type* it = LowerBound(key);
            return (it == Last || key < it->first) ? Last : it;
        }
        value_type* End() { return Last; }
    };
    class CState { public:                                     // 0x18 bytes
        long RegionSet, OverrideSet, UIOverrideSet, CinematicOverrideSet, MapCellSet;
        bool Paused, Danger;
    };
    CState                 CurrentState;                // +0x00
    void*                  PSoundDef;                   // +0x18 CDefPointer<const CSoundDef>
    CRegisteredMusicMap    RegisteredMusic;             // +0x1C
    long                   CurrentMusicID;              // +0x2C
    long                   PendingMusicID;              // +0x30
    void*                  PPendingFadeTimes;           // +0x34
    long                   PendingLoopCount;            // +0x38
    bool                   Paused;                      // +0x3C
protected:
    void UpdateCurrentMusicOffset();
};

void CMusicManager::UpdateCurrentMusicOffset()
{
    CSoundSystem* sound = g_pSoundSystem;
    if (sound && sound->m_musicEnabled) {
        CRegisteredMusicMap::value_type* it = RegisteredMusic.Find(CurrentMusicID);
        if (it != RegisteredMusic.End()) {
            float offset;
            if (sound->GetMusicPosition(&offset))
                it->second.Offset = offset;
        }
    }
}