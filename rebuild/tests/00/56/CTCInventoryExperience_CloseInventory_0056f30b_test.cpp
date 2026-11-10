// Behaviour test for CTCInventoryExperience::CloseInventory (0x0056F30B): a model of the close sequence. 3D rendering is turned
// back on and its start frame stamped with the current world frame; with a sound manager the two sound groups (0x10, 0x80) get
// the profile's sound volume (the new-profile value when that flag is set) and every tutorial sound handle is stopped; then the
// exit sound plays, the screen closes, the close message is posted with a lifetime of ten seconds of frames and the open-menu
// count drops by one.
#include <cstdio>
#include <map>
#include <string>
#include <vector>
struct SProfile { bool useNew; float volume, newVolume; float Get() const { return useNew ? newVolume : volume; } };
struct SState {
    bool rendering3D; long startFrame, worldFrame; bool hasSound;
    std::map<unsigned long, float> groupVolume; std::vector<unsigned long> stopped;
    int exitSounds, closes, openMenus; long messageType, messageFrames;
};
static void CloseInventory(SState& s, const SProfile& profile, const std::map<std::string, unsigned long>& handles, long fps)
{
    s.rendering3D = true;
    s.startFrame = s.worldFrame;
    if (s.hasSound) {
        s.groupVolume[0x10] = profile.Get();
        s.groupVolume[0x80] = profile.Get();
        for (std::map<std::string, unsigned long>::const_iterator it = handles.begin(); it != handles.end(); ++it)
            s.stopped.push_back(it->second);
    }
    ++s.exitSounds;
    ++s.closes;
    s.messageType = 0x4A;
    s.messageFrames = fps * 10;
    --s.openMenus;
}
int main()
{
    int fails = 0;
    std::map<std::string, unsigned long> handles;
    handles["B"] = 7; handles["A"] = 3;
    SProfile profile; profile.useNew = true; profile.volume = 0.25f; profile.newVolume = 0.75f;
    SState s; s.rendering3D = false; s.startFrame = 0; s.worldFrame = 1234; s.hasSound = true; s.exitSounds = 0; s.closes = 0;
    s.openMenus = 2; s.messageType = 0; s.messageFrames = 0;
    CloseInventory(s, profile, handles, 30);
    if (!s.rendering3D || s.startFrame != 1234 || s.groupVolume[0x10] != 0.75f || s.groupVolume[0x80] != 0.75f) ++fails;
    if (s.stopped.size() != 2 || s.stopped[0] != 3 || s.stopped[1] != 7) ++fails;   // map order: "A" then "B"
    if (s.exitSounds != 1 || s.closes != 1 || s.messageType != 0x4A || s.messageFrames != 300 || s.openMenus != 1) ++fails;
    SState q = s; q.hasSound = false; q.stopped.clear(); q.groupVolume.clear();
    CloseInventory(q, profile, handles, 30);
    if (!q.stopped.empty() || !q.groupVolume.empty() || q.openMenus != 0) ++fails;
    std::printf(fails ? "0056f30b_TEST FAIL\n" : "0056f30b_TEST PASS\n");
    return fails;
}