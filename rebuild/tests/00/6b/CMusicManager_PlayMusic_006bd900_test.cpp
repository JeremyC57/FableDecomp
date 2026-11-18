#include <stdio.h>

typedef int CWideString;
class CAFile {
public:
    static bool exists;
    static int calls;
    static bool PathExists(const CWideString& path) { ++calls; return path == 21 && exists; }
};
bool CAFile::exists;
int CAFile::calls;
class CASoundManager {
public:
    int calls, path;
    unsigned long outTime, inTime;
    float gain, offset;
    long loops;
    void PlayMusic(const CWideString& file, unsigned long outMS, unsigned long inMS, float g, float off, long n) {
        ++calls; path=file; outTime=outMS; inTime=inMS; gain=g; offset=off; loops=n;
    }
};
CASoundManager* PSoundManager;
template<class K, class V> class CKeyPairCompareLess {};
template<class K, class V, class C=CKeyPairCompareLess<K,V> > class CVectorMap {
public:
    struct Entry { K first; V second; } slot;
    typedef Entry* iterator;
    iterator end() { return &slot+1; }
    iterator Find(const K& k) { return k==slot.first ? &slot : end(); }
};
class CMusicManager {
public:
    struct CFadeTimes { unsigned long FadeOutMS, FadeInMS; };
    struct CRegisteredMusicEntry { CWideString SrcFileName, DstFileName; float Gain, Offset; bool CacheToHD; };
    CVectorMap<long,CRegisteredMusicEntry> RegisteredMusic;
    long CurrentMusicID, PendingMusicID, PendingLoopCount;
    CFadeTimes* PPendingFadeTimes;
    void PlayMusic(long music_id, const CFadeTimes& fade_times, long loop_count);
};

void CMusicManager::PlayMusic(long music_id, const CFadeTimes& fade_times, long loop_count)
{
    if (!PSoundManager) return;
    CVectorMap<long, CRegisteredMusicEntry, CKeyPairCompareLess<long, CRegisteredMusicEntry> >::iterator it = RegisteredMusic.Find(music_id);
    if (it == RegisteredMusic.end()) return;
    const CRegisteredMusicEntry& entry = it->second;
    if (entry.CacheToHD) {
        PSoundManager->PlayMusic(entry.DstFileName, fade_times.FadeOutMS, fade_times.FadeInMS, entry.Gain, entry.Offset, loop_count);
        CurrentMusicID = music_id;
        PendingMusicID = 0;
        PendingLoopCount = -1;
        PPendingFadeTimes = 0;
    } else {
        if (!CAFile::PathExists(entry.SrcFileName)) return;
        PSoundManager->PlayMusic(entry.SrcFileName, fade_times.FadeOutMS, fade_times.FadeInMS, entry.Gain, entry.Offset, loop_count);
        CurrentMusicID = music_id;
        PendingMusicID = 0;
        PendingLoopCount = -1;
        PPendingFadeTimes = 0;
    }
}

int main()
{
    for (int mask=0; mask<16; ++mask) {
        const bool sound=(mask&1)!=0, found=(mask&2)!=0, cached=(mask&4)!=0, exists=(mask&8)!=0;
        CMusicManager m;
        CMusicManager::CFadeTimes fade={101,202};
        m.RegisteredMusic.slot.first=14;
        CMusicManager::CRegisteredMusicEntry& track=m.RegisteredMusic.slot.second;
        track.SrcFileName=21; track.DstFileName=45; track.Gain=0.5f; track.Offset=1.25f; track.CacheToHD=cached;
        m.CurrentMusicID=67; m.PendingMusicID=78; m.PendingLoopCount=4; m.PPendingFadeTimes=&fade;
        CASoundManager device; device.calls=0;
        PSoundManager=sound ? &device : 0; CAFile::calls=0; CAFile::exists=exists;
        m.PlayMusic(found ? 14 : 29,fade,3);
        bool played=sound && found && (cached || exists);
        if (CAFile::calls != int(sound && found && !cached) || device.calls!=int(played)) return 1;
        if (played && (device.path!=(cached ? 45 : 21) || device.outTime!=101 || device.inTime!=202 ||
                       device.gain!=0.5f || device.offset!=1.25f || device.loops!=3)) return 2;
        if (m.CurrentMusicID!=(played ? 14 : 67) || m.PendingMusicID!=(played ? 0 : 78) ||
            m.PendingLoopCount!=(played ? -1 : 4) || m.PPendingFadeTimes!=(played ? 0 : &fade)) return 3;
    }
    puts("006bd900_TEST PASS");
    return 0;
}