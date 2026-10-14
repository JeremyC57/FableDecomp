#include <stdio.h>
#include <string.h>
#include <stdlib.h>
static int g_live;
struct CCharString {
    char* m_pStringData;
    CCharString() : m_pStringData(0) { ++g_live; }
    CCharString(const char* s) { m_pStringData = (char*)malloc(strlen(s) + 1); strcpy(m_pStringData, s); ++g_live; }
    CCharString(const CCharString& o) { m_pStringData = (char*)malloc(strlen(o.m_pStringData) + 1); strcpy(m_pStringData, o.m_pStringData); ++g_live; }
    ~CCharString() { free(m_pStringData); --g_live; }
};
static CCharString cat2(const char* a, const char* b) { char* t = (char*)malloc(strlen(a) + strlen(b) + 1); strcpy(t, a); strcat(t, b); CCharString r(t); free(t); return r; }
CCharString __fastcall operator+(const char* left, const CCharString& right) { return cat2(left, right.m_pStringData); }
CCharString __fastcall operator+(const CCharString& left, const char* right) { return cat2(left.m_pStringData, right); }
CCharString __fastcall operator+(const CCharString& left, const CCharString& right) { return cat2(left.m_pStringData, right.m_pStringData); }
struct CBordelloLady { unsigned char _pad[0x28]; CCharString Name; CCharString GetLHTSTag(const CCharString& suffix); };
CCharString CBordelloLady::GetLHTSTag(const CCharString& suffix) { return "LHTS_" + Name + "_" + suffix; }
int main()
{
    bool ok;
    {
        CBordelloLady lady; lady.Name.m_pStringData = (char*)malloc(6); strcpy(lady.Name.m_pStringData, "Daisy");
        CCharString suffix("TALK");
        CCharString tag = lady.GetLHTSTag(suffix);
        ok = strcmp(tag.m_pStringData, "LHTS_Daisy_TALK") == 0 && g_live == 3;
    }
    ok = ok && g_live == 0;
    if (!ok) { printf("00e403d0_TEST FAIL\n"); return 1; }
    printf("00e403d0_TEST PASS\n"); return 0;
}
