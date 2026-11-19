// Test for 0x00437870 CPlayerGui::IsMiniMapDrawing() const: a self-contained model of the read (PDrawMiniMap +0x160 ->
// NPlayerGui::CDrawBase::Drawing +0x18). Prints the 00437870 pass line (build_candidates PassPattern changes from the old
// CTCShotDefinitionBase-prefixed one at landing; NF95 part 2, q11376).
#include <cstdio>
#include <cstddef>

namespace NPlayerGui {
class CDrawBase { public: unsigned char _pad00[0x18]; bool Drawing; bool IsDrawing() const { return Drawing; } };
}
class CPlayerGui {
public:
    unsigned char _pad000[0x160];
    NPlayerGui::CDrawBase* PDrawMiniMap;   // +0x160 (CCountedPointer<NPlayerGui::CDrawMiniMap>::Data)
    bool IsMiniMapDrawing() const { return PDrawMiniMap->IsDrawing(); }
};

int main()
{
    if (offsetof(NPlayerGui::CDrawBase, Drawing) != 0x18 || offsetof(CPlayerGui, PDrawMiniMap) != 0x160) {
        std::printf("00437870_TEST FAIL layout\n");
        return 1;
    }
    NPlayerGui::CDrawBase map;
    CPlayerGui gui;
    gui.PDrawMiniMap = &map;
    map.Drawing = true;
    bool r1 = gui.IsMiniMapDrawing();
    map.Drawing = false;
    bool r2 = gui.IsMiniMapDrawing();
    if (r1 && !r2) {
        std::printf("00437870_TEST PASS\n");
        return 0;
    }
    std::printf("00437870_TEST FAIL\n");
    return 1;
}
