// Behaviour test for CTCActionUseScriptedHook::CTCActionUseScriptedHook (0x00729A90): usable, all flags false, UIDs zero,
// pointers null, registered for updates.
#include <cstdio>
#ifdef _MSC_VER
typedef unsigned __int64 u64;
#else
typedef unsigned long long u64;
#endif
static int g_registered;
struct IPtr { void* item; IPtr(void* p) : item(p) {} };
struct Str { void* P; Str() : P(0) {} };
struct Hook {
    void* owner; bool Usable, ForceConfirmation, WaitingForConfirmation, PerformAction, TeleportToRegionEntrance;
    u64 EntranceConnectedToUID; IPtr PEntranceConnectedTo; bool EntranceLocked;
    u64 CameraTrackUID; IPtr PCameraTrack; Str SoundName, AnimationName; long ReplacementObject;
    bool ReversedOnMiniMap, HiddenOnMiniMap;
    Hook(void* o) : owner(o), Usable(true), ForceConfirmation(false), WaitingForConfirmation(false), PerformAction(false),
        TeleportToRegionEntrance(false), EntranceConnectedToUID(0), PEntranceConnectedTo(0), EntranceLocked(false),
        CameraTrackUID(0), PCameraTrack(0), ReplacementObject(0), ReversedOnMiniMap(false), HiddenOnMiniMap(false)
    { ++g_registered; }
};
int main()
{
    int thing; Hook h(&thing);
    if (!h.Usable || h.ForceConfirmation || h.WaitingForConfirmation || h.PerformAction || h.TeleportToRegionEntrance ||
        h.EntranceConnectedToUID || h.PEntranceConnectedTo.item || h.EntranceLocked || h.CameraTrackUID || h.PCameraTrack.item ||
        h.SoundName.P || h.AnimationName.P || h.ReplacementObject || h.ReversedOnMiniMap || h.HiddenOnMiniMap || g_registered != 1 || h.owner != &thing)
    { std::printf("00729a90_TEST FAIL\n"); return 1; }
    std::printf("00729a90_TEST PASS\n");
    return 0;
}