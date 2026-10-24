#include <cstdio>

// Model of 0x0040E350 STLport __copy<CActionInputControl*>(first, last, result, random_access_iterator_tag, int*):
// counted forward assignment of the 28-byte bindings, returning the advanced result.
struct C2DVector {
    float X;
    float Y;
};
struct CActionInputControl {
    long GameAction;
    long ControllerType;
    long KeyboardKey;
    long XboxButton;
    long MouseButton;
    C2DVector ControlDirection;
};
struct random_access_iterator_tag { random_access_iterator_tag() { } };
static CActionInputControl* Copy(const CActionInputControl* first, const CActionInputControl* last, CActionInputControl* result,
                                 const random_access_iterator_tag&, int*)
{
    for (int n = int(last - first); n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}

int main()
{
    CActionInputControl src[3];
    CActionInputControl dst[4];
    for (int i = 0; i < 3; ++i) {
        src[i].GameAction = i + 1;
        src[i].ControllerType = 10 + i;
        src[i].KeyboardKey = 20 + i;
        src[i].XboxButton = 30 + i;
        src[i].MouseButton = 40 + i;
        src[i].ControlDirection.X = (float)i;
        src[i].ControlDirection.Y = (float)-i;
    }
    dst[3].GameAction = 99;
    CActionInputControl* end = Copy(src, src + 3, dst, random_access_iterator_tag(), 0);
    if (end != dst + 3 || dst[3].GameAction != 99) {
        std::printf("0040e350_TEST FAIL\n");
        return 1;
    }
    for (int i = 0; i < 3; ++i)
        if (dst[i].GameAction != i + 1 || dst[i].MouseButton != 40 + i || dst[i].ControlDirection.Y != (float)-i) {
            std::printf("0040e350_TEST FAIL\n");
            return 1;
        }
    if (Copy(src, src, dst + 1, random_access_iterator_tag(), 0) != dst + 1) {
        std::printf("0040e350_TEST FAIL\n");
        return 1;
    }
    std::printf("0040e350_TEST PASS\n");
    return 0;
}