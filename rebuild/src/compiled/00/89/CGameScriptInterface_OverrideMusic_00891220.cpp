#include "engine/CGameScriptInterface.h"

// Sound manager (agentSND2, 2026-11-10): PSoundManager is the shared engine/CASoundManager.h one; the local CGSIOverrideMusicEmitter model is gone and its slots carry the header names (Stop -> SetGainFader slot 46).
enum EMusicSetType { MUSIC_SET_NONE = 0 };
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
#include "engine/CASoundManager.h"
struct CGameScriptInterface_OverrideMusicMethods : CGameScriptInterface {
    virtual void OverrideMusic(EMusicSetType musicSetType, bool enabled, bool forcePlay) const;
};

void CGameScriptInterface_OverrideMusicMethods::OverrideMusic(
    EMusicSetType musicSetType, bool enabled, bool forcePlay) const {
    const CGameScriptInterface* scriptInterface =
        reinterpret_cast<const CGameScriptInterface*>(this);
    if (enabled) {
        if (PSoundManager != 0) {
            CGSIOverrideMusicCodeSection* section = &CUserProfileManager::Get();
            float value;
            if (section->useAlternate != 0)
                value = section->alternateValue;
            else
                value = section->primaryValue;
            PSoundManager->SetGainFader(0x100, value, 500.0f);
        }
        reinterpret_cast<CGSIOverrideMusicInner*>(scriptInterface->World)
            ->event->AddUBYTE((unsigned char)musicSetType);
        return;
    }
    reinterpret_cast<CGSIOverrideMusicInner*>(scriptInterface->World)
        ->event->SetOverrideMusic(musicSetType, forcePlay);
}
