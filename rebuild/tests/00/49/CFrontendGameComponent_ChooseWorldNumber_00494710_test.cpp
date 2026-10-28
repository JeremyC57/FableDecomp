// Behaviour test for CFrontendGameComponent::ChooseWorldNumber (0x00494710): number is 1-based within the
// page starting at WorldNamesOffset; an in-range pick stores "<name>.wld" in WorldToLoad and calls SetQuit,
// an out-of-range pick (0, too large, before the list) changes nothing.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned short wchar16;

static int WLen(const wchar16* s) { int n = 0; while (s[n]) ++n; return n; }

class CWideString
{
public:
    void* PData;
    CWideString() : PData(0) {}
    CWideString(const CWideString& o) : PData(0) { *this = o; }
    const CWideString& operator=(const CWideString& other)
    {
        if (this == &other) return *this;
        free(PData); PData = 0;
        if (other.PData) {
            int n = WLen((const wchar16*)other.PData);
            PData = malloc((n + 1) * sizeof(wchar16));
            memcpy(PData, other.PData, (n + 1) * sizeof(wchar16));
        }
        return *this;
    }
    ~CWideString() { free(PData); }
};

static CWideString Make(const char* a)
{
    CWideString s;
    int n = (int)strlen(a), i;
    wchar16* p = (wchar16*)malloc((n + 1) * sizeof(wchar16));
    for (i = 0; i <= n; ++i) p[i] = (wchar16)(unsigned char)a[i];
    s.PData = p;
    return s;
}

CWideString __fastcall operator+(const CWideString& left, const wchar16* right)
{
    CWideString r;
    int a = left.PData ? WLen((const wchar16*)left.PData) : 0, b = WLen(right);
    wchar16* p = (wchar16*)malloc((a + b + 1) * sizeof(wchar16));
    if (a) memcpy(p, left.PData, a * sizeof(wchar16));
    memcpy(p + a, right, (b + 1) * sizeof(wchar16));
    r.PData = p;
    return r;
}

struct CWideStringVector
{
    CWideString* _M_start;
    CWideString* _M_finish;
    CWideString* _M_end_of_storage;
    CWideString* begin() const { return _M_start; }
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    CWideString& operator[](unsigned long n) const { return *(begin() + n); }
};

static int g_quits = 0;

class CFrontendGameComponent
{
public:
    virtual void v00() {}
    virtual void v01() {}
    virtual void v02() {}
    virtual void v03() {}
    virtual void SetQuit() { ++g_quits; }
    void ChooseWorldNumber(long number);

    unsigned char _members[16];
    CWideStringVector WorldNames;
    long WorldNamesOffset;
    CWideString WorldToLoad;
};

void CFrontendGameComponent::ChooseWorldNumber(long number)
{
    unsigned long index = WorldNamesOffset + number - 1;
    if (index < WorldNames.size())
    {
        WorldToLoad = WorldNames[index] + (const wchar16*)L".wld";
        SetQuit();
    }
}

static int Is(const CWideString& s, const char* want)
{
    const wchar16* p = (const wchar16*)s.PData;
    int i, n = (int)strlen(want);
    if (!p) return n == 0;
    for (i = 0; i <= n; ++i) if (p[i] != (wchar16)(unsigned char)want[i]) return 0;
    return 1;
}

int main()
{
    CWideString names[4];
    CFrontendGameComponent fe;
    names[0] = Make("Albion"); names[1] = Make("Oakvale"); names[2] = Make("Bowerstone"); names[3] = Make("Arena");
    fe.WorldNames._M_start = names; fe.WorldNames._M_finish = names + 4; fe.WorldNames._M_end_of_storage = names + 4;
    fe.WorldNamesOffset = 1;

    fe.ChooseWorldNumber(2);                       // index 2 -> Bowerstone
    if (!Is(fe.WorldToLoad, "Bowerstone.wld") || g_quits != 1) { printf("FAIL pick\n"); return 1; }
    fe.ChooseWorldNumber(4);                       // index 4: past the end
    fe.ChooseWorldNumber(0);                       // index 0 -> Albion (offset 1, number 0)
    if (!Is(fe.WorldToLoad, "Albion.wld") || g_quits != 2) { printf("FAIL zero\n"); return 1; }
    fe.WorldNamesOffset = 0;
    fe.ChooseWorldNumber(0);                       // index -1 -> unsigned wrap, rejected
    if (!Is(fe.WorldToLoad, "Albion.wld") || g_quits != 2) { printf("FAIL wrap\n"); return 1; }
    fe.ChooseWorldNumber(4);                       // index 3 -> Arena
    if (!Is(fe.WorldToLoad, "Arena.wld") || g_quits != 3) { printf("FAIL last\n"); return 1; }
    printf("FRONTEND_00494710_TEST PASS\n");
    return 0;
}
