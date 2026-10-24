// Behaviour test for 006f9ee0 (NAnimationEvents::SaveAnimationEvents): the game and the sound event managers save,
// in that order, to the misc directory + their file names, and every temporary string is released.
// The wide string is a narrow stand-in here (the lane's VC7.1 has no built-in wchar_t; the model takes unsigned short).
#include <cstdio>
#include <cstring>

static int gLiveStrings = 0;

class CWideString {
public:
    char Text[64];
    CWideString(const char* text)              // 0x0099B6B0 stand-in
    {
        std::strcpy(Text, text);
        ++gLiveStrings;
    }
    CWideString(const CWideString& other)
    {
        std::strcpy(Text, other.Text);
        ++gLiveStrings;
    }
    ~CWideString() { --gLiveStrings; }        // 0x0099B510
};
CWideString operator+(const CWideString& left, const CWideString& right)   // 0x0099BE70 stand-in
{
    CWideString sum(left.Text);
    std::strcat(sum.Text, right.Text);
    return sum;
}

namespace NResourceDirectoryNames {
CWideString GetMiscDir() { return CWideString("Data\\Misc\\"); }   // 0x0041A0A0
}

template<class T> class CCountedPointer {
public:
    T* Data;
    void* Info;
    T& operator*() const { return *Data; }
};

namespace NAnimationEvents {
class CManager {
public:
    char Saved[64];
    int SaveOrder;
    void Save(const CWideString& fileName);   // 0x006F9BE0 stand-in
};
static int gSaves = 0;
void CManager::Save(const CWideString& fileName)
{
    std::strcpy(Saved, fileName.Text);
    SaveOrder = ++gSaves;
}

CCountedPointer<CManager> GameEventManager;
CCountedPointer<CManager> SoundEventManager;

inline CManager& GetGameEventManager() { return *GameEventManager; }
inline CManager& GetSoundEventManager() { return *SoundEventManager; }

void SaveAnimationEvents()
{
    GetGameEventManager().Save(NResourceDirectoryNames::GetMiscDir() + CWideString("game_animation_events.txt"));
    GetSoundEventManager().Save(NResourceDirectoryNames::GetMiscDir() + CWideString("sound_animation_events.txt"));
}
}

int main()
{
    NAnimationEvents::CManager game, sound;
    game.Saved[0] = sound.Saved[0] = 0;
    game.SaveOrder = sound.SaveOrder = 0;
    NAnimationEvents::GameEventManager.Data = &game;
    NAnimationEvents::SoundEventManager.Data = &sound;
    NAnimationEvents::SaveAnimationEvents();
    int ok = !std::strcmp(game.Saved, "Data\\Misc\\game_animation_events.txt") &&
             !std::strcmp(sound.Saved, "Data\\Misc\\sound_animation_events.txt") && game.SaveOrder == 1 && sound.SaveOrder == 2 &&
             gLiveStrings == 0;
    if (!ok) {
        std::printf("006f9ee0_TEST FAIL\n");
        return 1;
    }
    std::printf("006f9ee0_TEST PASS\n");
    return 0;
}