// Behaviour test for 0x0046EFB8 __copy_ptrs<CQuickAccessItemButtonGuiDef*>: assigns last-first elements in order, returns result + n
#include <cstdio>
struct C2DVector { float X; float Y; };
struct Base { int body[15]; static int assigns; Base& operator=(const Base& o) { ++assigns; for (int i = 0; i < 15; ++i) body[i] = o.body[i]; return *this; } };
int Base::assigns = 0;
struct Button : Base {
    int ButtonGraphic; C2DVector Position; C2DVector Offset;
    Button& operator=(const Button& o) { Base::operator=(o); ButtonGraphic = o.ButtonGraphic; Position = o.Position; Offset = o.Offset; return *this; }
};
static Button* CopyPtrs(Button* first, Button* last, Button* result)
{
    for (long n = last - first; n > 0; --n) { *result = *first; ++first; ++result; }
    return result;
}
int main()
{
    Button src[3], dst[4];
    for (int i = 0; i < 3; ++i) { src[i].body[0] = 100 + i; src[i].ButtonGraphic = i * 7; src[i].Position.X = (float)i; src[i].Offset.Y = -(float)i; }
    dst[3].ButtonGraphic = 99;
    Button* end = CopyPtrs(src, src + 3, dst);
    bool ok = end == dst + 3 && Base::assigns == 3 && dst[2].body[0] == 102 && dst[1].ButtonGraphic == 7
        && dst[2].Position.X == 2.0f && dst[2].Offset.Y == -2.0f && dst[3].ButtonGraphic == 99;
    ok = ok && CopyPtrs(src, src, dst) == dst && Base::assigns == 3;
    if (!ok) { std::printf("0046efb8_TEST FAIL\n"); return 1; }
    std::printf("0046efb8_TEST PASS\n");
    return 0;
}