#include <cstdio>
#include <new>
class CWideString {
public:
    void* PData;
    CWideString(const CWideString& s);                   // 0x0099B720
};
class CCharString {
public:
    void* PData;
    CCharString(const CCharString& s);                   // 0x0099EC30
};
class CMainGameComponentInit {
public:
    CWideString InitialWorldName;                        // +0x00
    CWideString InitialWorldHolySiteName;                // +0x04
    CCharString InitialQuestName;                        // +0x08
    CWideString SaveGameName;                            // +0x0c
};
void CopyMainGameComponentInit(void* p, const CMainGameComponentInit& init)   // emission helper, not a retail function
{
    new (p) CMainGameComponentInit(init);
}

static int g_w, g_c;
CWideString::CWideString(const CWideString& s) : PData(s.PData) { ++g_w; }
CCharString::CCharString(const CCharString& s) : PData(s.PData) { ++g_c; }
int main()
{
    static double src[2], dst[2];
    CMainGameComponentInit* a = (CMainGameComponentInit*)src;
    a->InitialWorldName.PData = (void*)1; a->InitialWorldHolySiteName.PData = (void*)2; a->InitialQuestName.PData = (void*)3; a->SaveGameName.PData = (void*)4;
    CopyMainGameComponentInit(dst, *a);
    CMainGameComponentInit* b = (CMainGameComponentInit*)dst;
    bool ok = g_w == 3 && g_c == 1 && b->InitialWorldName.PData == (void*)1 && b->InitialWorldHolySiteName.PData == (void*)2
        && b->InitialQuestName.PData == (void*)3 && b->SaveGameName.PData == (void*)4;
    std::printf(ok ? "00415e17_TEST PASS\n" : "FAIL\n");
    return 0;
}