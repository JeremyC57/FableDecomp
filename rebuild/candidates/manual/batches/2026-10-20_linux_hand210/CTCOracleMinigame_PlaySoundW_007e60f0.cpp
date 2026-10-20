// CTCOracleMinigame::PlaySoundW(const CCharString& sound) -- play a sample-bank sound by symbol on the minigame thing's sound player (interface 0x10): the symbol's index as a one-sound pair with default play settings; the invalid handle when the thing has no player or the symbol is unknown
// Leaf: PlaySoundW
// Retail: Fable.exe (TLC, VC7.1) @ 0x007E60F0, 183 bytes (`ret 4`; PDB ?PlaySoundW@CTCOracleMinigame@@QAEKABVCCharString@@@Z --
//   the name carries the <windows.h> PlaySound -> PlaySoundW macro).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020, CASoundBank::GetSoundIndexFromSymbol(const CCharString&)
//   const @ 0x00A01920 (g_pMainGameComponent @ 0x013B86A0 ->PSampleBank +0x10), CTCSoundPlayer::PlayAttachedSample(const CSoundPair&,
//   const CSoundInit&, bool, bool, bool) @ 0x0066B280 (PDB QAEKABVCSoundPair@@ABVCSoundInit@@_N22@Z; three landed callers declare it
//   `void PlaySound(...)`, but the result is this function's return value). g_InvalidSoundHandle @ 0x0129D25C.
// Layout: Ego_r.pdb CSoundPair {FirstSound, LastSound}, CSoundInit {Volume, Pitch, Pan, NoPlays, PlaybackType, GainFaderBits,
//   PitchFaderBits, OverrideID} (defaults 1, 1, 0, 1, 1, 0, 0, -1, as the landed SetWhatsThatSound 0x0093C600 block).
// Shape: the out-parameter GetInterface keeps the player in esi; the sample bank is read into a named local before the
//   symbol push (the chained form pushes first and loads the bank after); both play arguments are constructor temporaries
//   built during the right-to-left argument evaluation.
struct CTCBase;
struct CCharString { void* PStringData; };
extern unsigned long g_InvalidSoundHandle;              // 0x0129D25C
struct CASoundBank {
    long GetSoundIndexFromSymbol(const CCharString& symbol) const;   // 0x00A01920
};
struct CMainGameComponent {
    unsigned char _pad00[0x10];
    CASoundBank*  PSampleBank;                          // +0x10
};
extern CMainGameComponent* g_pMainGameComponent;        // 0x013B86A0
struct CSoundPair {
    long FirstSound;                                    // +0x00
    long LastSound;                                     // +0x04
    CSoundPair(long first, long last) : FirstSound(first), LastSound(last) { }
};
enum ESoundPlaybackType { SOUND_PLAYBACK_1 = 1 };
struct CSoundInit {                                     // 0x20
    float              Volume;                          // +0x00
    float              Pitch;                           // +0x04
    float              Pan;                             // +0x08
    long               NoPlays;                         // +0x0c
    ESoundPlaybackType PlaybackType;                    // +0x10
    unsigned long      GainFaderBits;                   // +0x14
    unsigned long      PitchFaderBits;                  // +0x18
    long               OverrideID;                      // +0x1c
    CSoundInit() : Volume(1.0f), Pitch(1.0f), Pan(0.0f), NoPlays(1), PlaybackType(SOUND_PLAYBACK_1), GainFaderBits(0),
                   PitchFaderBits(0), OverrideID(-1) { }
};
struct CTCSoundPlayer {                                 // interface 0x10
    unsigned long PlayAttachedSample(const CSoundPair& sound, const CSoundInit& init, bool a, bool b, bool c);   // 0x0066B280
};
struct CInterfaceMap {                                  // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                     // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                         // +0x44
    template <class T> __forceinline bool GetInterface(T** out, long type)
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31))) {
            long key = type;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&key);
            if (it == map->last || type < it->type)
                it = map->last;
            *out = (T*)it->component;
            return true;
        }
        return false;
    }
};
struct CTCOracleMinigame {
    virtual void v00();
    CThing* ParentThing;                                // +0x04 (CTCBase)
    unsigned long PlaySoundW(const CCharString& sound);
};

unsigned long CTCOracleMinigame::PlaySoundW(const CCharString& sound)
{
    CTCSoundPlayer* player;
    if (ParentThing->GetInterface(&player, 0x10)) {
        CASoundBank* bank = g_pMainGameComponent->PSampleBank;
        long index = bank->GetSoundIndexFromSymbol(sound);
        if (index != 0) {
            return player->PlayAttachedSample(CSoundPair(index, index), CSoundInit(), true, false, false);
        }
    }
    return g_InvalidSoundHandle;
}
