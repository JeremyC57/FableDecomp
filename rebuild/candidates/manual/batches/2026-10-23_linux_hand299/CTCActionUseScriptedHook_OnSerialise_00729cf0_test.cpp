#include <cstdio>
#include <cstring>
#ifdef _MSC_VER
typedef unsigned __int64 u64;
#else
typedef unsigned long long u64;
#endif
// Behaviour model of CTCActionUseScriptedHook::OnSerialise: versioned block, conditional UID save, remap on load.
struct Ctx {
    long Mode; int calls; char names[32][32]; long loadVersion;
    bool IsSaving() const { return Mode == 1 || Mode == 3; }
    bool IsLoading() const { return Mode == 2 || Mode == 3; }
    void Note(const char* n) { std::strcpy(names[calls++], n); }
    void TransferBool(const char* n, bool&, bool) { Note(n); }
    void TransferLong(const char* n, long& v, long) { Note(n); if (!IsSaving() && std::strcmp(n, "VersionNumber") == 0) v = loadVersion; }
    void TransferUID(const char* n, u64&, u64) { Note(n); }
    void TransferName(const char* n) { Note(n); }
};
struct Hook { bool Usable, Force, Teleport, Rev, Hidden; u64 Entrance, Camera; long Replacement; };
static u64 Remap(u64 uid) { return uid + 100; }
static void Tail(Ctx& c, Hook& h) { c.TransferName("SoundName"); c.TransferName("AnimationName"); c.TransferLong("ReplacementObject", h.Replacement, 0); }
static void OnSerialise(Ctx& c, Hook& h)
{
    c.TransferBool("Usable", h.Usable, true); c.TransferBool("ReversedOnMiniMap", h.Rev, false); c.TransferBool("HiddenOnMiniMap", h.Hidden, false);
    if (c.IsSaving()) {
        long version = 1; c.TransferLong("VersionNumber", version, 0);
        c.TransferBool("ForceConfirmation", h.Force, false); c.TransferBool("TeleportToRegionEntrance", h.Teleport, false);
        if (h.Entrance != 0 || c.IsLoading()) c.TransferUID("EntranceConnectedToUID", h.Entrance, 0);
        if (h.Camera != 0 || c.IsLoading()) c.TransferUID("CameraTrackUID", h.Camera, 0);
        Tail(c, h);
    } else {
        long version = 0; c.TransferLong("VersionNumber", version, 0);
        if (version != 1) return;
        c.TransferBool("ForceConfirmation", h.Force, false); c.TransferBool("TeleportToRegionEntrance", h.Teleport, false);
        c.TransferUID("EntranceConnectedToUID", h.Entrance, 0); if (h.Entrance != 0) h.Entrance = Remap(h.Entrance);
        c.TransferUID("CameraTrackUID", h.Camera, 0); if (h.Camera != 0) h.Camera = Remap(h.Camera);
        Tail(c, h);
    }
}
int main()
{
    Hook h; std::memset(&h, 0, sizeof h); h.Camera = 5;
    Ctx s; std::memset(&s, 0, sizeof s); s.Mode = 1;
    OnSerialise(s, h);
    if (s.calls != 10 || std::strcmp(s.names[6], "CameraTrackUID") != 0 || std::strcmp(s.names[9], "ReplacementObject") != 0)
    { std::printf("00729cf0_TEST FAIL\n"); return 1; }
    Ctx l; std::memset(&l, 0, sizeof l); l.Mode = 2; l.loadVersion = 1;
    OnSerialise(l, h);
    if (l.calls != 11 || h.Camera != 105 || h.Entrance != 0) { std::printf("00729cf0_TEST FAIL\n"); return 1; }
    Ctx o; std::memset(&o, 0, sizeof o); o.Mode = 2; o.loadVersion = 0;
    OnSerialise(o, h);
    if (o.calls != 4) { std::printf("00729cf0_TEST FAIL\n"); return 1; }
    std::printf("00729cf0_TEST PASS\n");
    return 0;
}
