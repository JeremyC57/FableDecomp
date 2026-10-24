// CMusicManager::PlayMusic(long musicID, const CFadeTimes& fadeTimes, long loopCount) -- start a registered track (the cached copy when CacheToHD, else the source file if it exists) and make it current, clearing any pending request
// Grade: DIFFER(ours v retail) -- 170v169: register permutation only (retail global ebx / this esi / id ebp / entry edi, ours ebp / edi / ebx / esi; the +1 byte is `mov edx,[ebp+0]`); every instruction, branch and the shared call tail otherwise identical. Tried: entry reference (this file), a local sound pointer, `it->second.` everywhere, an entry pointer + else-if.
// Leaf: ?PlayMusic@CMusicManager@@IAEXJABVCFadeTimes@1@J@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006BD900, 169 bytes (+ int3 padding), `ret 0Ch` (thiscall).
// Callees: CVectorMap<long, CRegisteredMusicEntry>::LowerBound(const long&) @ 0x006BE5E0 (same-TU stand-in body),
//   CAFile::PathExists(const CWideString&) @ 0x00999230 (static `__fastcall`), the sound system's virtual slot 17
//   (+0x44) on the global sound system @ 0x013B8394 (file, fade-out, fade-in, gain, offset, loop count; name from use).
// Identity: Ego_r.pdb ?PlayMusic@CMusicManager@@IAEXJABVCFadeTimes@1@J@Z; CFadeTimes = FadeOutMS +0, FadeInMS +4.
// Layout: CMusicManager (Ego_r.pdb, retail 12-byte vector; the landed ctor 0x006BE300 model): RegisteredMusic +0x1C
//   (First +0x1C, Last +0x20), CurrentMusicID +0x2C, PendingMusicID +0x30, PPendingFadeTimes +0x34, PendingLoopCount
//   +0x38; CRegisteredMusicEntry = SrcFileName +0, DstFileName +4, Gain +8, Offset +0xC, CacheToHD +0x10 (at pair +4).
// Shape: the global tested once (ebx); the inline CVectorMap::Find ternary (signed `jge`) with the caller re-reading
//   `Last`; the cached arm reuses ebx for the call because the LowerBound body is in the TU (VC7.1 then knows it leaves
//   the global alone), while the source arm re-reads the global after PathExists; the two virtual calls share their
//   tail (`push FadeOutMS ; push file ; call [edx+44h]`); then CurrentMusicID = id, PendingMusicID = 0,
//   PendingLoopCount = -1, PPendingFadeTimes = 0 (zero register shared).
// Label correction: the catalog `Music_LoadAndStartTrack` is descriptive.
class CWideString {
public:
    void* PStringData;                                  // +0x00
};

class CAFile {
public:
    static bool __fastcall PathExists(const CWideString& path);   // 0x00999230
};

class CSoundSystem {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16();
    virtual void PlayMusic(const CWideString& file, unsigned long fadeOutMS, unsigned long fadeInMS, float gain,
                           float offset, long loopCount);  // slot 17 (+0x44)
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
    class CFadeTimes {
    public:
        unsigned long FadeOutMS;                        // +0x00
        unsigned long FadeInMS;                         // +0x04
    };
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
        __declspec(noinline) value_type* LowerBound(const long& key)   // 0x006BE5E0 (same-TU stand-in)
        {
            value_type* first = First;
            int len = int(Last - First);
            while (len > 0) {
                int half = len / 2;
                value_type* middle = first + half;
                if (middle->first < key) {
                    first = middle + 1;
                    len = len - half - 1;
                }
                else
                    len = half;
            }
            return first;
        }
        value_type* Find(const long& key)
        {
            value_type* it = LowerBound(key);
            return (it == Last || key < it->first) ? Last : it;
        }
        value_type* End() { return Last; }
    };
    struct CState {                                     // 0x18 bytes
        long RegionSet, OverrideSet, UIOverrideSet, CinematicOverrideSet, MapCellSet;
        bool Paused, Danger;
    };
    CState                 CurrentState;                // +0x00
    void*                  PSoundDef;                   // +0x18 CDefPointer<const CSoundDef>
    CRegisteredMusicMap    RegisteredMusic;             // +0x1C
    long                   CurrentMusicID;              // +0x2C
    long                   PendingMusicID;              // +0x30
    CFadeTimes*            PPendingFadeTimes;           // +0x34
    long                   PendingLoopCount;            // +0x38
    bool                   Paused;                      // +0x3C
protected:
    void PlayMusic(long musicID, const CFadeTimes& fadeTimes, long loopCount);
};

void CMusicManager::PlayMusic(long musicID, const CFadeTimes& fadeTimes, long loopCount)
{
    if (g_pSoundSystem) {
        CRegisteredMusicMap::value_type* it = RegisteredMusic.Find(musicID);
        if (it != RegisteredMusic.End()) {
            CRegisteredMusicEntry& entry = it->second;
            if (entry.CacheToHD)
                g_pSoundSystem->PlayMusic(entry.DstFileName, fadeTimes.FadeOutMS, fadeTimes.FadeInMS, entry.Gain, entry.Offset, loopCount);
            else {
                if (!CAFile::PathExists(entry.SrcFileName))
                    return;
                g_pSoundSystem->PlayMusic(entry.SrcFileName, fadeTimes.FadeOutMS, fadeTimes.FadeInMS, entry.Gain, entry.Offset, loopCount);
            }
            CurrentMusicID = musicID;
            PendingMusicID = 0;
            PendingLoopCount = -1;
            PPendingFadeTimes = 0;
        }
    }
}
