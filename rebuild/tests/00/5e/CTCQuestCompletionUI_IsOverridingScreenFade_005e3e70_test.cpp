#include <cstdio>
#include <cstring>

class CTCQuestCompletionUI { public:
    char   pad00[0x0c];
    unsigned char Active;
    char   pad0d[0x10-0x0d];
    int    m_10;
    int    m_14;
    char   pad18[0x1c-0x18];
    unsigned char m_1c;
    char   pad1d[0xc0-0x1d];
    unsigned char m_c0;
};

bool __fastcall IsOverridingScreenFade(CTCQuestCompletionUI* completionUI)
{
    if (completionUI->Active != 0 && completionUI->m_1c == 0 && completionUI->m_c0 == 0
        && completionUI->m_10 != completionUI->m_14)
        return true;
    return false;
}

typedef bool (__fastcall *FN)(void* ecx, void* edx);

static bool call_it(CTCQuestCompletionUI* obj) {
    FN fn = (FN)(void*)&IsOverridingScreenFade;
    return fn(obj, 0);
}

int main() {
    CTCQuestCompletionUI o;
    memset(&o, 0, sizeof(o));
    o.Active = 1; o.m_1c = 0; o.m_c0 = 0; o.m_10 = 5; o.m_14 = 9;
    if (call_it(&o) != true) { printf("FAIL case1\n"); return 1; }

    memset(&o, 0, sizeof(o));
    o.Active = 0; o.m_10 = 5; o.m_14 = 9;
    if (call_it(&o) != false) { printf("FAIL case2\n"); return 1; }

    memset(&o, 0, sizeof(o));
    o.Active = 1; o.m_1c = 1; o.m_10 = 5; o.m_14 = 9;
    if (call_it(&o) != false) { printf("FAIL case3\n"); return 1; }

    memset(&o, 0, sizeof(o));
    o.Active = 1; o.m_c0 = 1; o.m_10 = 5; o.m_14 = 9;
    if (call_it(&o) != false) { printf("FAIL case4\n"); return 1; }

    memset(&o, 0, sizeof(o));
    o.Active = 1; o.m_10 = 7; o.m_14 = 7;
    if (call_it(&o) != false) { printf("FAIL case5\n"); return 1; }

    printf("OK_0x005e3e70\n");
    return 0;
}