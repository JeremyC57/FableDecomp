#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
struct CGSIStopMusicCodeSection
// Sound manager (agentSND2, 2026-11-10): PSoundManager is the shared engine/CASoundManager.h one; the local CGSIStopMusicEmitter model is gone and its slots carry the header names (Stop -> SetGainFader slot 46).
{
    unsigned char pad00[0x0c];
    unsigned char useAlternate;
    unsigned char pad0d[0x1b];
    float primaryValue;
    unsigned char pad2c[0xbc];
    float alternateValue;
};
class CUserProfileManager : public CGSIStopMusicCodeSection { public: static CUserProfileManager& __fastcall Get(); };   // 0x0040D2A0 ?Get@CUserProfileManager@@SIAAV1@XZ

struct CGSIStopMusicEvent;

class CWorld
{ public:
    unsigned char pad00[0x6c];
    CGSIStopMusicEvent* event;
};

#include "engine/CASoundManager.h"

struct CGameScriptInterface_Methods : CGameScriptInterface {
    void StopOverrideMusic(bool enabled) const;
};

extern void __fastcall CGSIStopMusic_StopEvent(CGSIStopMusicEvent* event);
extern void __fastcall CGSIStopMusic_ResumeEvent(CGSIStopMusicEvent* event);

void CGameScriptInterface_Methods::StopOverrideMusic(bool enabled) const
{
    if (enabled)
    {
        if (PSoundManager != 0)
        {
            CGSIStopMusicCodeSection* section =
                &CUserProfileManager::Get();
            float value;
            if (section->useAlternate != 0)
                value = section->alternateValue;
            else
                value = section->primaryValue;
            PSoundManager->SetGainFader(0x100, value, 500.0f);
        }
        CGSIStopMusic_StopEvent(World->event);
        return;
    }
    CGSIStopMusic_ResumeEvent(World->event);
}
