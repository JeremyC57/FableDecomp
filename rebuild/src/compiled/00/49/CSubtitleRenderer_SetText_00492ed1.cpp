// CSubtitleRenderer::SetText -- convert narrow message and supply four empty captions.
// Leaf: ?SetText@CSubtitleRenderer@@QAEXABVCFontBank@@ABVCCharString@@KM_N2@Z
// Retail: Fable.exe 0x00492ED1, 152 bytes.
// Flags: /O1 /Oy-
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CSUBTITLERENDERER_SETTEXT_CHAR
#include "fable_string.h"
#include "engine/CSubtitleRenderer.h"

void CSubtitleRenderer::SetText(const CFontBank& font, const CCharString& message,
    unsigned long flags, float display_time, bool update_while_paused, bool change_player_mode)
{
    SetText(font, message.ToWideString(), L"", L"", L"", L"", flags,
            display_time, update_while_paused, change_player_mode);
}