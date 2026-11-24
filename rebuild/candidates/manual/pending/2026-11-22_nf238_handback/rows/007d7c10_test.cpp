// Test for 0x007D7C10 CTCTavernGameSpotTheAddition::GetWinPhrase() on the shared headers (NF238), the NF236 0x004065D0 pattern: the
// row's defines, includes and body (self-contained link); the CWideString text constructor (retail 0x0099B6B0) and destructor are
// stubbed to record their argument. Case 1: one call builds the result from the empty wide literal exactly once and leaves the game
// object untouched. Case 2: a second call passes the same literal again (one shared constant).
#include <stdio.h>
#include <string.h>
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCTAVERNGAME
#define FABLE_RETAIL_CTCTAVERNGAME_GETDEF_OUTOFLINE
#define FABLE_RETAIL_CTCTAVERNGAME_POSITIONMESH
#define FABLE_RETAIL_CTCTAVERNGAME_GETGAMESTATE
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CSUBTITLERENDERER
#define FABLE_RETAIL_CSUBTITLERENDERER_CLEAR
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_C3DVECTOR_DOT_GROUPED
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CRIGHTHANDEDSET_GETRIGHT_OUTOFLINE
#define FABLE_RETAIL_INPUTPROCESS
#define FABLE_RETAIL_INPUTPROCESS_ZTARGET
#define FABLE_RETAIL_INPUTPROCESS_MOVEMENT
#define FABLE_RETAIL_CINPUTPROCESSTAVERNGAME_PRESSED_ACCESS
#define FABLE_RETAIL_CPROCESSEDINPUT_PERFORMED
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/C2DVector.h"
#include "engine/CRightHandedSet.h"
#include "engine/CTavernGameDef.h"
#include "engine/CTavernGameSpotTheAdditionDef.h"
#include "engine/CDefinitionManager.h"
#include "engine/CTCTavernGame.h"
#include "engine/CTCTavernGameSpotTheAddition.h"
#include "engine/CPlayerGui.h"
#include "engine/CWorld.h"
#include "fable_game.h"
#include "engine/CRGBColour.h"
#include "engine/EGameAction.h"
#include "engine/CInputEvent.h"
#include "engine/CProcessedInput.h"
#include "engine/CInputProcessGameBase.h"
#include "engine/CInputProcessTavernGame.h"

static const wchar_t* g_seen = 0;
static int g_ctors = 0;
CWideString::CWideString(const wchar_t* text) : PStringData(0) { g_seen = text; ++g_ctors; }
CWideString::~CWideString() {}
long CWideString::NoStrings = 0;

CWideString CTCTavernGameSpotTheAddition::GetWinPhrase()
{
    return CWideString(L"");
}

int main()
{
    int fails = 0;
    static unsigned char raw[sizeof(CTCTavernGameSpotTheAddition)];
    static unsigned char want[sizeof(CTCTavernGameSpotTheAddition)];
    memset(raw, 0x5A, sizeof(raw)); memset(want, 0x5A, sizeof(want));
    CTCTavernGameSpotTheAddition& game = *reinterpret_cast<CTCTavernGameSpotTheAddition*>(raw);
    {
        CWideString r = game.CTCTavernGameSpotTheAddition::GetWinPhrase();
        if (g_ctors != 1 || g_seen == 0 || g_seen[0] != 0) fails |= 1;
    }
    const wchar_t* first = g_seen;
    {
        CWideString r = game.CTCTavernGameSpotTheAddition::GetWinPhrase();
        if (g_ctors != 2 || g_seen != first) fails |= 2;
    }
    if (memcmp(raw, want, sizeof(raw)) != 0) fails |= 4;
    if (fails) { printf("007d7c10_TEST FAIL %d\n", fails); return 1; }
    printf("007d7c10_TEST PASS\n");
    return 0;
}
