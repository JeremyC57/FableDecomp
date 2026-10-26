#include <cstdio>
// Model of CAIStateGroup_WanderAroundShops::IsStillValid @ 0x00946F30: work hours && shop alive && pretty thing alive.
class CThing { public: unsigned char m_flags; };
struct Ptr { CThing* item; CThing* GetPItem() { return item; } bool IsAlive() { if (GetPItem() != 0) { if (!(GetPItem()->m_flags & 1)) return true; } return false; } };
static bool g_work;
struct Group {
    Ptr Shop, PrettyThing;
    bool IsInWorkHours() const { return g_work; }
    bool IsStillValid() const { Group* self = const_cast<Group*>(this); return self->IsInWorkHours() && self->Shop.IsAlive() && self->PrettyThing.IsAlive(); }
};
int main()
{
    CThing a = { 0 }, b = { 0 }; Group g; g.Shop.item = &a; g.PrettyThing.item = &b; g_work = true;
    if (!g.IsStillValid()) { std::printf("00946f30_TEST FAIL\n"); return 1; }
    g_work = false; if (g.IsStillValid()) { std::printf("00946f30_TEST FAIL\n"); return 1; }
    g_work = true; b.m_flags = 1; if (g.IsStillValid()) { std::printf("00946f30_TEST FAIL\n"); return 1; }
    b.m_flags = 0; g.Shop.item = 0; if (g.IsStillValid()) { std::printf("00946f30_TEST FAIL\n"); return 1; }
    std::printf("00946f30_TEST PASS\n"); return 0;
}