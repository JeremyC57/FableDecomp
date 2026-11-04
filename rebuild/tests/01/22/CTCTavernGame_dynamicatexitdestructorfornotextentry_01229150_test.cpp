// Behaviour test for CTCTavernGame::SetStateAfterInstructions's `dynamic atexit destructor for 'no_text_entry'' 0x01229150: the function-local static is built on
// first use only (one construction however often the owner runs) and the registered exit stub destroys exactly that object once.
#include <cstdio>
#include <cstdlib>
static int g_ctor = 0, g_dtor = 0;
static const void* g_dtorThis = 0;
struct CWideString {
    const char* Text;
    explicit CWideString(const char* t) : Text(t) { ++g_ctor; }
    ~CWideString() { ++g_dtor; g_dtorThis = this; }
};
static const CWideString* g_static = 0;
static const CWideString& Owner()
{
    static CWideString no_text_entry("Missing text entry");
    g_static = &no_text_entry;
    return no_text_entry;
}
static void Check()
{
    if (g_ctor != 1 || g_dtor != 1 || g_dtorThis != g_static) { std::printf("01229150_TEST FAIL\n"); std::fflush(stdout); return; }
    std::printf("01229150_TEST PASS\n"); std::fflush(stdout);
}
int main()
{
    std::atexit(Check);           // registered first, so it runs after the static's exit stub
    Owner(); Owner(); Owner();
    if (g_ctor != 1 || g_dtor != 0) { std::printf("01229150_TEST FAIL\n"); return 1; }
    return 0;
}