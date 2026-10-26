#include <cstdio>
// Model of CAIStateGroup_TavernKeeper::IsStillValid @ 0x009681E0: pub alive && bar alive.
class CThing { public: unsigned char m_flags; };
struct Ptr { CThing* item; CThing* GetPItem() { return item; } bool IsAlive() { if (GetPItem() != 0) { if (!(GetPItem()->m_flags & 1)) return true; } return false; } };
struct Group { Ptr PJug, PBar, PPub; bool IsStillValid() const { Group* s = const_cast<Group*>(this); return s->PPub.IsAlive() && s->PBar.IsAlive(); } };
int main()
{
    CThing pub = { 0 }, bar = { 0 }; Group g; g.PJug.item = 0; g.PBar.item = &bar; g.PPub.item = &pub;
    if (!g.IsStillValid()) { std::printf("009681e0_TEST FAIL\n"); return 1; }
    bar.m_flags = 1; if (g.IsStillValid()) { std::printf("009681e0_TEST FAIL\n"); return 1; }
    bar.m_flags = 0; g.PPub.item = 0; if (g.IsStillValid()) { std::printf("009681e0_TEST FAIL\n"); return 1; }
    std::printf("009681e0_TEST PASS\n"); return 0;
}