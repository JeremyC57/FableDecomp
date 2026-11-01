// CMusicManager::PlayMusic(long music_id, const CFadeTimes& fade_times, long loop_count) -- start a registered music track on the sound manager's stream (the HD-cached copy, or the source file when it exists), then make it current and clear the pending request
// Grade: DIFFER(170v169) -- pure callee-saved register permutation: retail sound->ebx, this->esi, key->ebp, entry->edi; built sound->ebp, this->edi, key->ebx, entry->esi (the +1 byte is the [ebp] vtable load's disp8). Every instruction and the block layout match otherwise.
// Next step: the allocation priority differs (retail colours the outer values first). Try the real CMusicManager TU context
//   (the neighbouring Set/Clear*OverridingMusicSet, UpdateCurrentMusicOffset and CacheMusic bodies before PlayMusic) and an inline
//   helper split of the play block; both change VC7.1's temp numbering without changing the bytes' shape.
// Leaf: ?PlayMusic@CMusicManager@@IAEXJABVCFadeTimes@1@J@Z
// Retail: Fable.exe @ 0x006BD900, 169 bytes, ret 0Ch. Identity: Ego_r ?PlayMusic@CMusicManager@@IAEXJABVCFadeTimes@1@J@Z is the only
//   CMusicManager member taking (long, const CFadeTimes&, long); it reads RegisteredMusic (+0x1C) and writes CurrentMusicID/
//   PendingMusicID/PPendingFadeTimes/PendingLoopCount (+0x2C..+0x38, retail 12-byte vector). Catalog label "Music_LoadAndStartTrack" is descriptive.
// Callees: CVectorMap<long,CRegisteredMusicEntry,...>::LowerBound 0x006BE5E0 (landed), CAFile::PathExists 0x00999230 (Ego_r static
//   fastcall), PSoundManager 0x013B8394 (established, DECISIONS_W12) slot 17 (+0x44) = CSoundManagerLH 0x00A3E4B0; the base
//   CASoundManager slot is _purecall 0x00BFEA08, so it is pure there. PlayStream is the landed descriptive spelling (no PDB name).
// Shape: two levers. (1) the LowerBound template body is in the TU (retail's TU sees it): VC7.1 then knows the call only reads
//   music_id, keeps the post-call key in a callee-saved register for the final CurrentMusicID store, and CSEs the PSoundManager
//   load across it (the first PlayStream reuses the tested value, the second re-reads after PathExists). Without the body: 178v169.
//   (2) stores in retail order CurrentMusicID, PendingMusicID, PendingLoopCount, PPendingFadeTimes (one xor eax for both zeros).
class CWideString { public: void* PStringData; };
class CAFile { public: static bool __fastcall PathExists(const CWideString& path); };   // 0x00999230
class CASoundManager {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16();
    virtual void PlayStream(const CWideString& file_name, unsigned long fade_out_ms, unsigned long fade_in_ms, float gain, float offset, long loop_count) = 0;   // slot 17 (+0x44)
};
extern CASoundManager* PSoundManager;                    // 0x013B8394
namespace std {
template <class _T1, class _T2> struct pair { typedef _T1 first_type; typedef _T2 second_type; _T1 first; _T2 second; };
template <class _Tp> class allocator { };
template <class _Value, class _Tp, class _MaybeReboundAlloc> struct _STLP_alloc_proxy { _Value _M_data; };
template <class _Tp, class _Alloc> class vector {
public:
    typedef _Tp* iterator;
    _Tp* _M_start; _Tp* _M_finish; _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
};
}
template <class T> class CArray : public std::vector<T, std::allocator<T> > { };
template <class K, class V> class CKeyPairCompareLess {
public:
    bool operator()(const std::pair<K, V>& entry, const K& key) const { return entry.first < key; }
};
template <class K, class V, class TCompare> class CVectorMap {
public:
    typedef std::pair<K, V> value_type;
    typedef typename CArray<value_type>::iterator iterator;
    CArray<value_type> Data;
    TCompare Compare;
    bool Dirty;
    iterator LowerBound(const K& key);
    iterator Find(const K& key) { iterator it = LowerBound(key); return (it == Data.end() || key < it->first) ? Data.end() : it; }
    iterator End() { return Data.end(); }
};
template <class K, class V, class TCompare>
typename CVectorMap<K, V, TCompare>::iterator CVectorMap<K, V, TCompare>::LowerBound(const K& key)
{
    iterator first = Data.begin();
    int count = Data.end() - Data.begin();
    while (count > 0) {
        int half = count / 2;
        iterator middle = first + half;
        if (Compare(*middle, key)) { first = middle + 1; count -= half + 1; }
        else count = half;
    }
    return first;
}
class CMusicManager {
public:
    class CRegisteredMusicEntry { public:
        CWideString SrcFileName; CWideString DstFileName; float Gain; float Offset; bool CacheToHD;
    };
    class CFadeTimes { public: unsigned long FadeOutMS; unsigned long FadeInMS; };
    class CState { public: long RegionSet, OverrideSet, UIOverrideSet, CinematicOverrideSet, MapCellSet; bool Paused, Danger; };
    CState CurrentState;
    void* PSoundDef;
    CVectorMap<long, CRegisteredMusicEntry, CKeyPairCompareLess<long, CRegisteredMusicEntry> > RegisteredMusic;
    long CurrentMusicID;
    long PendingMusicID;
    CFadeTimes* PPendingFadeTimes;
    long PendingLoopCount;
    bool Paused;
protected:
    void PlayMusic(long music_id, const CFadeTimes& fade_times, long loop_count);
};
void CMusicManager::PlayMusic(long music_id, const CFadeTimes& fade_times, long loop_count)
{
    if (!PSoundManager) return;
    CVectorMap<long, CRegisteredMusicEntry, CKeyPairCompareLess<long, CRegisteredMusicEntry> >::iterator it = RegisteredMusic.Find(music_id);
    if (it == RegisteredMusic.End()) return;
    const CRegisteredMusicEntry& entry = it->second;
    if (entry.CacheToHD) {
        PSoundManager->PlayStream(entry.DstFileName, fade_times.FadeOutMS, fade_times.FadeInMS, entry.Gain, entry.Offset, loop_count);
    } else {
        if (!CAFile::PathExists(entry.SrcFileName)) return;
        PSoundManager->PlayStream(entry.SrcFileName, fade_times.FadeOutMS, fade_times.FadeInMS, entry.Gain, entry.Offset, loop_count);
    }
    CurrentMusicID = music_id;
    PendingMusicID = 0;
    PendingLoopCount = -1;
    PPendingFadeTimes = 0;
}
