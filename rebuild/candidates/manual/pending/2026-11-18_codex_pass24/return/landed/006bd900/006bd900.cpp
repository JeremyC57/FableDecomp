// CMusicManager::PlayMusic(long music_id, const CFadeTimes& fade_times, long loop_count) -- start a registered music track on the sound manager's stream (the HD-cached copy, or the source file when it exists), then make it current and clear the pending request
// Leaf: ?PlayMusic@CMusicManager@@IAEXJABVCFadeTimes@1@J@Z
// Retail: 0x006BD900, 169 bytes, thiscall.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_CSOUNDDEF_MUSICSETENTRIES
#define FABLE_RETAIL_CMUSICMANAGER_OVERRIDE_TESTS
#include "fable_string.h"
#include "engine/CDefClassBase.h"
#include "engine/CMusicManager.h"
#include "engine/CASoundManager.h"

#include "engine/CAFile.h"
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
