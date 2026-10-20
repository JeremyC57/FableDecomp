// Behaviour test for 0x00446BE0 CGamePlayerInterface::~CGamePlayerInterface: the packet file is closed by the body only
// while saving or loading; the members then go in reverse order (button vectors, controls, def release, file, base).
#include <cstdio>

static char g_order[32]; static int g_n; static int g_bodyCloses, g_dtorCloses, g_defDestroyed;
static void Mark(char c) { g_order[g_n++] = c; g_order[g_n] = 0; }
struct CDiskFileWin32 {
    bool inBody;
    void Close() { if (inBody) ++g_bodyCloses; else ++g_dtorCloses; }
    ~CDiskFileWin32() { inBody = false; Close(); Mark('F'); }
};
struct CDef { long RefCount; void Destroy() { ++g_defDestroyed; } };
struct CDefPointer { CDef* Object; ~CDefPointer() { if (Object) { if (--Object->RefCount == 0) Object->Destroy(); Object = 0; } Mark('D'); } };
struct CVec { char tag; ~CVec() { Mark(tag); } };
struct CBase { ~CBase() { Mark('B'); } };
struct CGamePlayerInterface : CBase {
    bool PacketSave, PacketLoad;
    CDiskFileWin32 PacketFile;
    CDefPointer PControlsDef;
    CVec Controls, Assignable, QuickAccess, ContextSensitive;
    ~CGamePlayerInterface()
    {
        PacketFile.inBody = true;
        if (PacketSave || PacketLoad)
            PacketFile.Close();
    }
};
static int Run(bool save, bool load, CDef* def)
{
    g_n = 0; g_bodyCloses = 0; g_dtorCloses = 0;
    {
        CGamePlayerInterface gpi;
        gpi.PacketSave = save; gpi.PacketLoad = load; gpi.PControlsDef.Object = def;
        gpi.Controls.tag = 'c'; gpi.Assignable.tag = 'a'; gpi.QuickAccess.tag = 'q'; gpi.ContextSensitive.tag = 'x';
    }
    return g_bodyCloses;
}
static int Same(const char* a, const char* b) { while (*a && *a == *b) { ++a; ++b; } return *a == *b; }
int main()
{
    CDef def = { 1 };
    if (Run(false, false, &def) != 0 || g_dtorCloses != 1 || !Same(g_order, "xqacDFB") || g_defDestroyed != 1) { std::printf("00446be0_TEST FAIL\n"); return 1; }
    if (Run(true, false, 0) != 1 || Run(false, true, 0) != 1 || Run(true, true, 0) != 1) { std::printf("00446be0_TEST FAIL\n"); return 1; }
    std::printf("00446be0_TEST PASS\n");
    return 0;
}