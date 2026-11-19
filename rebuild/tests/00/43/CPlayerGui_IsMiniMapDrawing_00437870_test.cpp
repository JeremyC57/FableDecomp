// Test for 0x00437870 CPlayerGui::IsMiniMapDrawing() const (NF95 part 2, q11376; NF97 2026-11-19 port): the class is the shared
// engine/CPlayerGui.h (no local model); the test links the row and calls it on raw storage laid out as retail reads it:
// PDrawMiniMap (+0x160, the CCountedPointer's data word) -> NPlayerGui::CDrawBase::Drawing (+0x18).
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CPLAYERGUI_MEMBERS
#define FABLE_RETAIL_NHEROINFORMATIONSCREENS_CMANAGER
#define FABLE_RETAIL_NHEROINFORMATIONSCREENS_CMANAGER_LAYOUT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CPLAYERGUI_MINIMAPDRAWING
#include <cstdio>
#include <cstring>
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CPlayerGui.h"

static unsigned long g_gui[0x800 / 4];       // storage read as a CPlayerGui
static unsigned long g_miniMap[0x40 / 4];    // storage read as the NPlayerGui::CDrawMiniMap

int main()
{
    void* miniMap = g_miniMap;
    std::memcpy(reinterpret_cast<unsigned char*>(g_gui) + 0x160, &miniMap, sizeof(miniMap));
    const CPlayerGui* gui = reinterpret_cast<const CPlayerGui*>(g_gui);
    unsigned char* drawing = reinterpret_cast<unsigned char*>(g_miniMap) + 0x18;
    *drawing = 1;
    bool r1 = gui->IsMiniMapDrawing();
    *drawing = 0;
    bool r2 = gui->IsMiniMapDrawing();
    if (r1 && !r2) {
        std::printf("CTCShotDefinitionBase_00437870_TEST PASS\n");
        return 0;
    }
    std::printf("00437870 FAIL\n");
    return 1;
}
