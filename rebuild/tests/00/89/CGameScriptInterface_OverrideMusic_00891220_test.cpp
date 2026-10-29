#include <cstdio>
#include <cstring>
#if !defined(_MSC_VER)
#  define __fastcall
#  define __cdecl
#endif
class CWorld;
class CGameScriptInterface                        // test-local view (engine/CGameScriptInterface.h is 32-bit only)
{ public:
    virtual void v000() { }
    CWorld* World;                                 // +0x04
};

enum EMusicSetType { MUSIC_SET_NONE = 0, MUSIC_SET_TEST = 0x12345678 };
struct CGSIOverrideMusicCodeSection {
    unsigned char pad00[0x0c]; unsigned char useAlternate; unsigned char pad0d[0x17];
    float primaryValue; unsigned char pad28[0xbc]; float alternateValue;
};
class CUserProfileManager : public CGSIOverrideMusicCodeSection { public: static CUserProfileManager& __fastcall Get(); };   // 0x0040D2A0 ?Get@CUserProfileManager@@SIAAV1@XZ
struct CGSIOverrideMusicEvent {
    void AddUBYTE(unsigned char musicSetType);
    void SetOverrideMusic(EMusicSetType musicSetType, bool forcePlay);
};
struct CGSIOverrideMusicInner { unsigned char pad00[0x6c]; CGSIOverrideMusicEvent* event; };
struct CGSIOverrideMusicEmitter {
    virtual void Slot00(); virtual void Slot04(); virtual void Slot08(); virtual void Slot0c();
    virtual void Slot10(); virtual void Slot14(); virtual void Slot18(); virtual void Slot1c();
    virtual void Slot20(); virtual void Slot24(); virtual void Slot28(); virtual void Slot2c();
    virtual void Slot30(); virtual void Slot34(); virtual void Slot38(); virtual void Slot3c();
    virtual void Slot40(); virtual void Slot44(); virtual void Slot48(); virtual void Slot4c();
    virtual void Slot50(); virtual void Slot54(); virtual void Slot58(); virtual void Slot5c();
    virtual void Slot60(); virtual void Slot64(); virtual void Slot68(); virtual void Slot6c();
    virtual void Slot70(); virtual void Slot74(); virtual void Slot78(); virtual void Slot7c();
    virtual void Slot80(); virtual void Slot84(); virtual void Slot88(); virtual void Slot8c();
    virtual void Slot90(); virtual void Slot94(); virtual void Slot98(); virtual void Slot9c();
    virtual void Slota0(); virtual void Slota4(); virtual void Slota8(); virtual void Slotac();
    virtual void Slotb0(); virtual void Slotb4();
    virtual void Stop(unsigned long command, float value, float fade);
};
struct CGameScriptInterface_OverrideMusicMethods : CGameScriptInterface {
    virtual void OverrideMusic(EMusicSetType musicSetType, bool enabled, bool forcePlay) const;
};

CGSIOverrideMusicEmitter* CGSIOverrideMusic_Emitter;
static CGSIOverrideMusicCodeSection g_section;
static int g_addCalls, g_setCalls;
static unsigned char g_addedMusicSet;
static EMusicSetType g_setMusicSet;
static bool g_forcePlay;
static unsigned long g_command;
static float g_value;
static float g_fade;

CUserProfileManager& __fastcall CUserProfileManager::Get() { return *(CUserProfileManager*)(&g_section); }
void CGSIOverrideMusicEvent::AddUBYTE(unsigned char value) { ++g_addCalls; g_addedMusicSet = value; }
void CGSIOverrideMusicEvent::SetOverrideMusic(EMusicSetType value, bool force) {
    ++g_setCalls; g_setMusicSet = value; g_forcePlay = force;
}
void CGSIOverrideMusicEmitter::Slot00() { }
void CGSIOverrideMusicEmitter::Slot04() { }
void CGSIOverrideMusicEmitter::Slot08() { }
void CGSIOverrideMusicEmitter::Slot0c() { }
void CGSIOverrideMusicEmitter::Slot10() { }
void CGSIOverrideMusicEmitter::Slot14() { }
void CGSIOverrideMusicEmitter::Slot18() { }
void CGSIOverrideMusicEmitter::Slot1c() { }
void CGSIOverrideMusicEmitter::Slot20() { }
void CGSIOverrideMusicEmitter::Slot24() { }
void CGSIOverrideMusicEmitter::Slot28() { }
void CGSIOverrideMusicEmitter::Slot2c() { }
void CGSIOverrideMusicEmitter::Slot30() { }
void CGSIOverrideMusicEmitter::Slot34() { }
void CGSIOverrideMusicEmitter::Slot38() { }
void CGSIOverrideMusicEmitter::Slot3c() { }
void CGSIOverrideMusicEmitter::Slot40() { }
void CGSIOverrideMusicEmitter::Slot44() { }
void CGSIOverrideMusicEmitter::Slot48() { }
void CGSIOverrideMusicEmitter::Slot4c() { }
void CGSIOverrideMusicEmitter::Slot50() { }
void CGSIOverrideMusicEmitter::Slot54() { }
void CGSIOverrideMusicEmitter::Slot58() { }
void CGSIOverrideMusicEmitter::Slot5c() { }
void CGSIOverrideMusicEmitter::Slot60() { }
void CGSIOverrideMusicEmitter::Slot64() { }
void CGSIOverrideMusicEmitter::Slot68() { }
void CGSIOverrideMusicEmitter::Slot6c() { }
void CGSIOverrideMusicEmitter::Slot70() { }
void CGSIOverrideMusicEmitter::Slot74() { }
void CGSIOverrideMusicEmitter::Slot78() { }
void CGSIOverrideMusicEmitter::Slot7c() { }
void CGSIOverrideMusicEmitter::Slot80() { }
void CGSIOverrideMusicEmitter::Slot84() { }
void CGSIOverrideMusicEmitter::Slot88() { }
void CGSIOverrideMusicEmitter::Slot8c() { }
void CGSIOverrideMusicEmitter::Slot90() { }
void CGSIOverrideMusicEmitter::Slot94() { }
void CGSIOverrideMusicEmitter::Slot98() { }
void CGSIOverrideMusicEmitter::Slot9c() { }
void CGSIOverrideMusicEmitter::Slota0() { }
void CGSIOverrideMusicEmitter::Slota4() { }
void CGSIOverrideMusicEmitter::Slota8() { }
void CGSIOverrideMusicEmitter::Slotac() { }
void CGSIOverrideMusicEmitter::Slotb0() { }
void CGSIOverrideMusicEmitter::Slotb4() { }
void CGSIOverrideMusicEmitter::Stop(unsigned long, float, float) { }
struct CRecordingEmitter : CGSIOverrideMusicEmitter {
    void Stop(unsigned long command, float value, float fade) { g_command = command; g_value = value; g_fade = fade; }
};

void CGameScriptInterface_OverrideMusicMethods::OverrideMusic(
    EMusicSetType musicSetType, bool enabled, bool forcePlay) const {
    const CGameScriptInterface* scriptInterface =
        reinterpret_cast<const CGameScriptInterface*>(this);
    if (enabled) {
        if (CGSIOverrideMusic_Emitter != 0) {
            CGSIOverrideMusicCodeSection* section = &CUserProfileManager::Get();
            float value;
            if (section->useAlternate != 0)
                value = section->alternateValue;
            else
                value = section->primaryValue;
            CGSIOverrideMusic_Emitter->Stop(0x100, value, 500.0f);
        }
        reinterpret_cast<CGSIOverrideMusicInner*>(scriptInterface->World)
            ->event->AddUBYTE((unsigned char)musicSetType);
        return;
    }
    reinterpret_cast<CGSIOverrideMusicInner*>(scriptInterface->World)
        ->event->SetOverrideMusic(musicSetType, forcePlay);
}

int main() {
    CRecordingEmitter emitterObject;
    CGSIOverrideMusicEvent event;
    CGSIOverrideMusicInner inner; std::memset(&inner, 0, sizeof(inner)); inner.event = &event;
    CGameScriptInterface_OverrideMusicMethods scriptInterface;
    reinterpret_cast<CGameScriptInterface*>(&scriptInterface)->World = reinterpret_cast<CWorld*>(&inner);

    CGSIOverrideMusic_Emitter = 0;
    scriptInterface.OverrideMusic(MUSIC_SET_TEST, true, false);
    if (g_addCalls != 1 || g_setCalls != 0 || g_addedMusicSet != 0x78) return 1;

    CGSIOverrideMusic_Emitter = &emitterObject;
    std::memset(&g_section, 0, sizeof(g_section)); g_section.primaryValue = 0.75f;
    scriptInterface.OverrideMusic(MUSIC_SET_TEST, true, true);
    if (g_command != 0x100 || g_value != 0.75f || g_fade != 500.0f || g_addCalls != 2) return 2;

    g_section.useAlternate = 1; g_section.alternateValue = 0.25f;
    scriptInterface.OverrideMusic(MUSIC_SET_TEST, true, false);
    if (g_value != 0.25f || g_addCalls != 3) return 3;

    scriptInterface.OverrideMusic(MUSIC_SET_TEST, false, true);
    if (g_addCalls != 3 || g_setCalls != 1 || g_setMusicSet != MUSIC_SET_TEST || !g_forcePlay) return 4;
    scriptInterface.OverrideMusic(MUSIC_SET_NONE, false, false);
    if (g_setCalls != 2 || g_setMusicSet != MUSIC_SET_NONE || g_forcePlay) return 5;

    std::printf("FSE2_00891220_TEST PASS\n");
    return 0;
}
