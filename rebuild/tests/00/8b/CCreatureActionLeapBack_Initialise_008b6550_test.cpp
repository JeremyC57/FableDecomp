#include <cstdio>
#include <cstring>
// Behaviour model: the animation is set before the base initialisation runs.
static char anim[40]; static int order;
static void setAnim(const char* n) { std::strcpy(anim, n); order = order * 10 + 1; }
static void baseInit() { order = order * 10 + 2; }
int main()
{
    setAnim("MOVEMENT_STRAFE_BACKWARD"); baseInit();
    if (order != 12 || std::strcmp(anim, "MOVEMENT_STRAFE_BACKWARD") != 0) { std::printf("008b6550_TEST FAIL\n"); return 1; }
    std::printf("008b6550_TEST PASS\n");
    return 0;
}