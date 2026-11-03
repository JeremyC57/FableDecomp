// Behaviour test for GFClearBadExitFlag at 0x00401780: open the game key, write the graphics-reset value FALSE, close.
#include <cstdio>
static int g_order[8]; static int g_n = 0;
static int g_written = -1;
class CWideString { public: const char* PStringData; };
class CSystemRegistry { public:
    static const CWideString GAME_REG_KEY;
    static const CWideString GFX_RESET_KEY_NAME;
    CSystemRegistry(const CWideString& subKey, long) : RootKey(0), OpenKey(subKey.PStringData) { g_order[g_n++] = 1; }
    ~CSystemRegistry() { g_order[g_n++] = 3; }
    void WriteBOOL(const CWideString& valueName, int value) const { (void)valueName; g_written = value; g_order[g_n++] = 2; }
private:
    void* RootKey; const char* OpenKey;
};
const CWideString CSystemRegistry::GAME_REG_KEY = { "Software\\Lionhead Studios Ltd\\Fable" };
const CWideString CSystemRegistry::GFX_RESET_KEY_NAME = { "GfxReset" };
void GFClearBadExitFlag()
{
    CSystemRegistry registry(CSystemRegistry::GAME_REG_KEY, 0);
    registry.WriteBOOL(CSystemRegistry::GFX_RESET_KEY_NAME, 0);
}
int main()
{
    GFClearBadExitFlag();
    if (g_n == 3 && g_order[0] == 1 && g_order[1] == 2 && g_order[2] == 3 && g_written == 0) {
        std::printf("_global_00401780_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL\n");
    return 1;
}
