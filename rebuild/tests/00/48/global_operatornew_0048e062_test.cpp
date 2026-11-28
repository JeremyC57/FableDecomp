// Leaf: ?GetTextWidthFromText@CSubtitleRenderer@@ABEMABVCWideString@@ABVCFontBank@@@Z
// Flags: /Gr
#define FABLE_RETAIL_SUBTITLE_DRAW_TEXT_BOX_PASS30
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA_DECL_ONLY
#define FABLE_RETAIL_CRGBCOLOUR_CURSOR_COPY_INLINE_PASS22
#define FABLE_RETAIL_C2DVECTOR_ORIGINAL_COPIES_PASS26

#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CDISPLAYMANAGER_RENDERTARGET
#define FABLE_RETAIL_SUBTITLE_TEXT_DRAW_PASS29
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CSUBTITLERENDERER_EMBEDDED
#define FABLE_RETAIL_CSUBTITLERENDERER_BUTTONGRAPHICS
#define FABLE_RETAIL_CSUBTITLERENDERER_ISFLAGSET
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_C2DEXTENTSF
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_CPLAYERGUIDEF_CLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CPLAYERGUI_PEEKGUIDEF
#define FABLE_RETAIL_CFONTBANK_VSTRINGWIDTH
#define FABLE_RETAIL_CFONTBANK
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_DRAW_BUTTON_TEXT_PASS29
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CSUBTITLERENDERER_INCUTSCENE
#define FABLE_RETAIL_SUBTITLE_DRAW_PREPARE_PASS30
#define FABLE_RETAIL_CSUBTITLERENDERER_DRAWPREPAREPRIMITIVES
#define FABLE_RETAIL_CSUBTITLERENDERER
#define FABLE_RETAIL_CFADEINFADEOUTBASE_FADE
#define FABLE_RETAIL_CENGINEPRIMITIVEPREPAREINFO
#define FABLE_RETAIL_C2DBOXF_EXTENTS
#define FABLE_RETAIL_CUSERPROFILEMANAGER
#define FABLE_RETAIL_CUSERPROFILEMANAGER_SUBTITLING
#define FABLE_RETAIL_CMAINGAMECOMPONENT_GETWORLD
#define FABLE_RETAIL_CMAINGAMECOMPONENT_WORLD
#define FABLE_RETAIL_CMAINGAMECOMPONENT_SMART_POINTER_ACCESSORS
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_GAMESCRIPTINTERFACE
#define FABLE_RETAIL_CWORLD_UPDATEINPUT
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE_CLEAR_DECL_ONLY
#include "engine/Direct3D9Types.h"
#include "fable_string.h"
#include "engine/CRGBColour.h"
#include "engine/C2DVector.h"
#include "engine/C2DExtentsF.h"
#include "engine/C2DBoxF.h"
#include "engine/CPlayerGuiDef.h"
#include "engine/NUISystem_CManager.h"
#include "engine/CPlayerGui.h"
#include "engine/CDisplayManager.h"
#include "engine/CFontBank.h"
#include "engine/CSubtitleRenderer.h"
#include "engine/SubtitleDrawGlobals.h"
#include "engine/CFontManager.h"
#include "engine/CMainGameComponent.h"
#include "engine/CWorld.h"
#include "engine/CGameScriptInterface.h"
#ifdef _MSC_VER
#pragma optimize("s",on)
#pragma optimize("y",on)
#endif
// The native 0048DB35 owner is the 17-byte size-optimized IsSubtitlingEnabled.
#include "engine/CUserProfileManager.h"
#include "engine/CEnginePrimitivePrepareInfo.h"
#include "stlport/fable_algorithm.h"

#ifdef _MSC_VER
#pragma optimize("s",on)
#pragma optimize("y",on)
#endif


template<class Tag, typename Tag::Type member> struct TestMember {
    friend typename Tag::Type access(Tag){ return member; }
};
struct MeasureTextMember {
    typedef float (CSubtitleRenderer::*Type)(const CWideString&,const CFontBank&) const;
    friend Type access(MeasureTextMember);
};
template struct TestMember<MeasureTextMember,&CSubtitleRenderer::GetTextWidthFromText>;
float CSubtitleRenderer::GetTextWidthFromText(const CWideString& text, const CFontBank& font) const
{
    return font.GetVStringWidth(text);
}

#include <stdio.h>
static float answer;
static const CFontBank* expected_font;
static const CWideString* expected_text;
static unsigned int calls,failures;
float CFontBank::GetVStringWidth(const CWideString& text) const {
    ++calls;if (this!=expected_font || &text!=expected_text) ++failures;
    return answer;
}
int main() {
    union { double align; unsigned char bytes[sizeof(CSubtitleRenderer)]; } objs[2];
    union { double align; unsigned char bytes[sizeof(CFontBank)]; } fonts[2];
    for (int i=0;i<12;++i) {
        CSubtitleRenderer& r=*reinterpret_cast<CSubtitleRenderer*>(objs[i%2].bytes);
        expected_text=i%3==0?&r.Button1Text:i%3==1?&r.Button2Text:&r.Button3Text;
        expected_font=reinterpret_cast<CFontBank*>(fonts[i/6].bytes);
        answer=float(i*3-10)*.25f;
        if ((r.*access(MeasureTextMember()))(*expected_text,*expected_font)!=answer) ++failures;
    }
    if (failures || calls!=12) return 1;
    puts("0048e062_TEST PASS");return 0;
}
