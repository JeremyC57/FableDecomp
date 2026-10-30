class CXMVPlaybackInfo
{
public:
    void FadeOut(float fadeOutTime, void* pFadeOutContext);
};

struct Field18Overlay
{
    char m_Pad00[0x1964];
    CXMVPlaybackInfo* m_pPlaybackInfo;
};

struct Field04Overlay
{
    char m_Pad00[0x18];
    Field18Overlay* m_Field18;
};

struct SelfOverlay
{
    char m_Pad00[0x04];
    Field04Overlay* m_Field04;
};

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

void CGameScriptInterface::ScreenFilterFadeOut(float fadeOutTime, void* pFadeOutContext) const
{
    const SelfOverlay* self = (const SelfOverlay*)this;
    Field04Overlay* field04 = self->m_Field04;
    Field18Overlay* field18 = field04->m_Field18;
    field18->m_pPlaybackInfo->FadeOut(fadeOutTime, pFadeOutContext);
}