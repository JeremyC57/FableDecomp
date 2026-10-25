// Behaviour test for 00c359e0: AudioObject_Play -- forwards three words and returns the bool result as 1/0
#include <cstdio>
struct CAudioObject {
    long a, b, c;
    bool Play(long x, long y, long z) { a = x; b = y; c = z; return x + y + z > 0; }
};
static int AudioObject_Play(CAudioObject* object, long a, long b, long c) { return object->Play(a, b, c) ? 1 : 0; }
int main()
{
    CAudioObject o;
    if (AudioObject_Play(&o, 1, 2, 3) != 1 || o.a != 1 || o.b != 2 || o.c != 3) { std::printf("00c359e0_TEST FAIL\n"); return 1; }
    if (AudioObject_Play(&o, -5, 2, 3) != 0) { std::printf("00c359e0_TEST FAIL\n"); return 1; }
    std::printf("00c359e0_TEST PASS\n");
    return 0;
}