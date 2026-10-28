// behaviour test for 0x0054F6E0 NUISystem::CText::~CText
extern "C" int printf(const char*, ...);
static int g_fail;
#define CHECK(c) do { if (!(c)) { printf("check failed line %d\n", __LINE__); ++g_fail; } } while (0)
static char g_log[32]; static int g_n;
static void Log(char c) { g_log[g_n++] = c; g_log[g_n] = 0; }
static int Same(const char* a, const char* b) { while (*a && *a == *b) { ++a; ++b; } return *a == *b; }
static const char* g_released;
struct Handle { char Tag; void Clear() { Log(Tag); } ~Handle() { Log((char)(Tag + 32)); } };
struct FontPtr { void Reset() { Log('F'); } ~FontPtr() { Log('f'); } };
struct Def { const char* Font; };
static Def g_def = { "FONT_NAME" };
struct FontManager { void ReleaseFont(const char* name) { g_released = name; Log('R'); } };
static FontManager g_fonts;
struct Base { ~Base() { Log('B'); } };
struct Text : Base {
    Handle H; Handle Shadow; FontPtr PFont;
    Text() { H.Tag = 'H'; Shadow.Tag = 'S'; }
    const Def* LoadAndReturnDef() const { return &g_def; }
    ~Text() { H.Clear(); Shadow.Clear(); PFont.Reset(); g_fonts.ReleaseFont(LoadAndReturnDef()->Font); }
};
int main()
{
    { Text t; }
    CHECK(Same(g_log, "HSFRfshB"));
    CHECK(g_released && Same(g_released, "FONT_NAME"));
    if (g_fail == 0) printf("0054f6e0_TEST PASS\n");
    return g_fail;
}