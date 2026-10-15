// Behaviour test for CEngineLandscapePatch::`vector deleting destructor' @ 0x00BDD3E0:
// a hand model of the compiler-generated ??_E (array bit 1 / free bit 0, count word before the array).
#include <cstdio>

static int g_order[16]; static int g_destroyed = 0;
static void* g_freedScalar = 0; static void* g_freedArray = 0; static long g_flagSeen = -1;

struct CPatch {
    int id; unsigned char body[0x44 - 4];
    void Destroy() { g_order[g_destroyed++] = id; }            // ~CEngineLandscapePatch @ 0x00BF3A40
};
static void FreeScalar(void* p, long flags) { g_freedScalar = p; g_flagSeen = flags; }   // 0x00BF36A0
static void FreeArray(void* p, long flags) { g_freedArray = p; g_flagSeen = flags; }     // 0x00BF36E0

static void* VectorDeletingDestructor(CPatch* self, unsigned int flags)
{
    if (flags & 2) {
        unsigned int* block = (unsigned int*)self - 1;
        long count = (long)*block;
        CPatch* p = self + count;
        while (--count >= 0) { --p; p->Destroy(); }
        if (flags & 1) FreeArray(block, 0);
        return block;
    }
    self->Destroy();
    if (flags & 1) FreeScalar(self, 0);
    return self;
}

int main()
{
    static unsigned int storage[1 + 3 * (sizeof(CPatch) / 4)];
    storage[0] = 3;
    CPatch* arr = (CPatch*)(storage + 1);
    for (int i = 0; i < 3; ++i) arr[i].id = i + 10;
    void* r = VectorDeletingDestructor(arr, 3);
    int ok = r == storage && g_destroyed == 3 && g_order[0] == 12 && g_order[1] == 11 && g_order[2] == 10
          && g_freedArray == storage && g_flagSeen == 0 && g_freedScalar == 0;
    g_destroyed = 0; g_freedArray = 0;
    r = VectorDeletingDestructor(arr, 2);                      // array, no free
    ok = ok && r == storage && g_destroyed == 3 && g_freedArray == 0;
    g_destroyed = 0;
    r = VectorDeletingDestructor(&arr[1], 1);                  // scalar + free
    ok = ok && r == &arr[1] && g_destroyed == 1 && g_order[0] == 11 && g_freedScalar == &arr[1];
    g_destroyed = 0; g_freedScalar = 0;
    r = VectorDeletingDestructor(&arr[2], 0);
    ok = ok && r == &arr[2] && g_destroyed == 1 && g_freedScalar == 0;
    storage[0] = 0; g_destroyed = 0;
    r = VectorDeletingDestructor(arr, 3);                      // empty array: no element destroyed
    ok = ok && g_destroyed == 0 && g_freedArray == storage;
    if (!ok) { std::printf("00bdd3e0_TEST FAIL\n"); return 1; }
    std::printf("00bdd3e0_TEST PASS\n");
    return 0;
}
