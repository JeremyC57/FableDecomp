// Behaviour test for CCountedPointer<COpinionReactionManager>::DeleteData (0x004ad600): a non-null object is destroyed then freed
// exactly once; a null pointer does nothing.
#include <cstdio>
static int g_dtors = 0;
struct Object { long Value; ~Object() { ++g_dtors; } };
template <class T> struct CountedPointer { static void DeleteData(void* data) { delete (T*)data; } };
int main()
{
    CountedPointer<Object>::DeleteData(new Object);
    CountedPointer<Object>::DeleteData(0);
    if (g_dtors != 1) { std::printf("004ad600_TEST FAIL\n"); return 1; }
    std::printf("004ad600_TEST PASS\n");
    return 0;
}
