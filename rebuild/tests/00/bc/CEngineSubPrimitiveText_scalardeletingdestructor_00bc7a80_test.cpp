// Behaviour test for CEngineSubPrimitiveText's scalar deleting destructor (0x00BC7A80): the text string destroyed, then the base; the class operator delete runs only on flag bit 0.
#include <cstdio>
#include <new>
static int g_log[4]; static int g_n; static int g_deletes;
struct Base { virtual ~Base() { g_log[g_n++] = 2; } static void Delete(void*) { ++g_deletes; } };
struct WideString { ~WideString() { g_log[g_n++] = 1; } };
struct Text : Base { float Pos[3]; float Scale; unsigned int Colour; WideString TextString; void* Font; void* Mask;
    void* ScalarDelete(unsigned int flags) { this->~Text(); if (flags & 1) Base::Delete(this); return this; } };
int main()
{
    unsigned char storage[sizeof(Text)]; Text* t = new (storage) Text;
    g_n = 0; t->ScalarDelete(1);
    if (g_n != 2 || g_log[0] != 1 || g_log[1] != 2 || g_deletes != 1) { std::printf("00bc7a80_TEST FAIL\n"); return 1; }
    t = new (storage) Text; g_n = 0; t->ScalarDelete(0);
    if (g_n != 2 || g_deletes != 1) { std::printf("00bc7a80_TEST FAIL\n"); return 1; }
    std::printf("00bc7a80_TEST PASS\n");
    return 0;
}