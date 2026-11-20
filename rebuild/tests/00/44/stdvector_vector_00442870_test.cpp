// Test for 0x00442870 std::vector<CSubtitleRenderer::CEmbeddedHighlight>::~vector (NF195, 2026-11-20, q6240: rewritten on the shared
// engine/CSubtitleRenderer.h / fable_string.h / stlport/fable_containers.h; links the row object). Every 0x14-byte record's CWideString
// is destroyed in order (test-only ~CWideString body records the address), then the storage is freed; an empty vector destroys nothing.
#include <stdio.h>
#include <stdlib.h>
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CSUBTITLERENDERER_EMBEDDED
#define FABLE_RETAIL_CSUBTITLERENDERER_EMBEDDEDHIGHLIGHT_NATURAL_PACK
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C2DVector.h"
#include "engine/CSubtitleRenderer.h"
typedef CSubtitleRenderer::CEmbeddedHighlight CHighlight;
typedef std::vector<CHighlight> CHighlights;
struct CHighlightsAccess : CHighlights {                     // test-only: sets the protected STLport words
    void Set(CHighlight* first, CHighlight* last) { this->_M_start = first; this->_M_finish = last; this->_M_end_of_storage._M_data = last; }
};
static int g_n; static void* g_seen[8];
CWideString::~CWideString() { g_seen[g_n++] = this; }      // test-only body (retail 0x0099B510, not under test)
int main()
{
    CHighlights* v = (CHighlights*)calloc(1, sizeof(CHighlights));
    CHighlight* s = (CHighlight*)malloc(3 * sizeof(CHighlight));
    static_cast<CHighlightsAccess*>(v)->Set(s, s + 3);
    v->~CHighlights();
    int ok = g_n == 3 && g_seen[0] == &s[0] && g_seen[1] == &s[1] && g_seen[2] == &s[2] && sizeof(CHighlight) == 0x14;
    static_cast<CHighlightsAccess*>(v)->Set(0, 0);
    v->~CHighlights();
    ok = ok && g_n == 3;
    printf(ok ? "00442870_TEST PASS\n" : "00442870_TEST FAIL\n");
    return ok ? 0 : 1;
}
