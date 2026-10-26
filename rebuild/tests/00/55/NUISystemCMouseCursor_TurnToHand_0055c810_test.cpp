#include <cstdio>
static int g_destroyed = 0;
struct CMouseCursorDef {
    long Ref; unsigned long HandGraphicIndex; unsigned long ClosedHandGraphicIndex;
    void ReleaseTemporary() { if (--Ref == 0) ++g_destroyed; }
};
template <class T> class CDefPointer { public:
    T* Data;
    CDefPointer() : Data(0) {}
    CDefPointer(const CDefPointer& o) : Data(o.Data) { if (Data) ++Data->Ref; }
    ~CDefPointer() { if (Data) Data->ReleaseTemporary(); }
    T* operator->() const { return Data; }
};
struct CMouseCursor {
    CMouseCursorDef* Def; unsigned long Graphic;
    CDefPointer<CMouseCursorDef> GetDef() { CDefPointer<CMouseCursorDef> p; p.Data = Def; ++Def->Ref; return p; }
    void SetGraphicIndex(unsigned long i) { Graphic = i; }
    void TurnToHand() { SetGraphicIndex(GetDef()->HandGraphicIndex); }
};
int main()
{
    CMouseCursorDef d; d.Ref = 1; d.HandGraphicIndex = 7; d.ClosedHandGraphicIndex = 9;
    CMouseCursor m; m.Def = &d; m.Graphic = 0;
    m.TurnToHand();
    if (m.Graphic != 7 || d.Ref != 1 || g_destroyed != 0) { std::printf("0055c810_TEST FAIL\n"); return 1; }
    std::printf("0055c810_TEST PASS\n");
    return 0;
}