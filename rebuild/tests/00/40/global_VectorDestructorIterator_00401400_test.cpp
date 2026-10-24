#include <cstdio>

// Model of 0x00401400 (`vector destructor iterator'): destroy `count` elements of `size` bytes, last to first.
struct SElement {
    int Id;
    int Pad[3];
};
static int g_destroyed[16];
static int g_count = 0;
static void DestroyElement(void* element) { g_destroyed[g_count++] = static_cast<SElement*>(element)->Id; }
typedef void (*PFNElementDestructor)(void* element);

static void VectorDestructorIterator(void* ptr, unsigned int size, int count, PFNElementDestructor pDtor)
{
    ptr = (char*)ptr + size * count;
    while (--count >= 0) {
        ptr = (char*)ptr - size;
        (*pDtor)(ptr);
    }
}

int main()
{
    SElement items[5];
    for (int i = 0; i < 5; ++i)
        items[i].Id = 10 + i;
    VectorDestructorIterator(items, sizeof(SElement), 5, DestroyElement);
    if (g_count != 5) {
        std::printf("00401400_TEST FAIL\n");
        return 1;
    }
    for (int i = 0; i < 5; ++i)
        if (g_destroyed[i] != 14 - i) {
            std::printf("00401400_TEST FAIL\n");
            return 1;
        }
    VectorDestructorIterator(items, sizeof(SElement), 0, DestroyElement);
    if (g_count != 5) {
        std::printf("00401400_TEST FAIL\n");
        return 1;
    }
    std::printf("00401400_TEST PASS\n");
    return 0;
}