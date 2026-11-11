// Behaviour test for NPlayerGui::CDrawMoney::ClearPrimitivesIfNecessary (0x00644263): the money bar's handles are cleared only
// when the drawer did not draw this frame and the GUI owns a money bar.
#include <stdio.h>
struct CStatBar { int clears; CStatBar() : clears(0) { } virtual ~CStatBar() { } virtual void ClearPrimitiveHandles() { ++clears; } };
struct CCountedPointerBar { CStatBar* Data; bool IsNull() const { return Data == 0; } CStatBar* operator->() const { return Data; } };
class CPlayerGui { public: CCountedPointerBar PMoney; };
struct CDrawMoney {
    CPlayerGui* PlayerGui; bool Drawing;
    virtual ~CDrawMoney() { }
    virtual void ClearPrimitivesIfNecessary();
};
void CDrawMoney::ClearPrimitivesIfNecessary()
{
    if (!Drawing && !PlayerGui->PMoney.IsNull())
        PlayerGui->PMoney->ClearPrimitiveHandles();
}
int main(void)
{
    CStatBar bar; CPlayerGui gui; gui.PMoney.Data = &bar;
    CDrawMoney d; d.PlayerGui = &gui;
    d.Drawing = true; d.ClearPrimitivesIfNecessary();
    int afterDrawing = bar.clears;
    d.Drawing = false; d.ClearPrimitivesIfNecessary();
    int afterIdle = bar.clears;
    gui.PMoney.Data = 0; d.ClearPrimitivesIfNecessary();     // no bar: nothing to do, no crash
    if (afterDrawing != 0 || afterIdle != 1 || bar.clears != 1) { printf("00644263_TEST FAIL\n"); return 1; }
    printf("00644263_TEST PASS\n");
    return 0;
}