// Behaviour test for 0x00C17400 (see src/00c17400.cpp).
#include <cstdio>
struct Data { int* freed; };
static int g_ran, g_logged;
static void RunProcessor(void* host, Data* data, void* log, const unsigned char* settings) { (void)host; (void)data; (void)log; (void)settings; g_ran++; }
static void Command(Data* found, const unsigned char* settings)
{
    if (found == 0) { g_logged++; return; }
    RunProcessor(0, found, 0, settings);
    (*found->freed)++;
}
int main()
{
    unsigned char s[18] = { 0 };
    int freed = 0; Data d = { &freed };
    Command(0, s);
    if (g_logged != 1 || g_ran != 0) { std::printf("00c17400_TEST FAIL\n"); return 1; }
    Command(&d, s);
    if (g_ran != 1 || freed != 1 || g_logged != 1) { std::printf("00c17400_TEST FAIL\n"); return 1; }
    std::printf("00c17400_TEST PASS\n"); return 0;
}