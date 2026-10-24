#include <cstdio>
#ifndef _MSC_VER
#define __fastcall
#endif

// Model: a tiny wide string, the defs directory getter, operator+ and a symbol map that counts its loads.
static int g_liveStrings = 0;

class CWideString {
public:
    unsigned short Text[64];
    CWideString() { Text[0] = 0; ++g_liveStrings; }
    CWideString(const CWideString& o) { int i = 0; for (; o.Text[i]; ++i) Text[i] = o.Text[i]; Text[i] = 0; ++g_liveStrings; }
    ~CWideString() { --g_liveStrings; }
};

static CWideString __fastcall GetDefsDirectory()
{
    const char* dir = "Data\\Defs\\";
    CWideString r;
    int i = 0;
    for (; dir[i]; ++i)
        r.Text[i] = (unsigned short)dir[i];
    r.Text[i] = 0;
    return r;
}

static CWideString __fastcall operator+(const CWideString& left, const unsigned short* right)
{
    CWideString r(left);
    int i = 0;
    while (r.Text[i])
        ++i;
    for (int j = 0; right[j]; ++j)
        r.Text[i++] = right[j];
    r.Text[i] = 0;
    return r;
}

class CSymbolMap {
public:
    int  Loads;
    char LastFile[64];
    bool LoadFromFile(const CWideString& filename)
    {
        ++Loads;
        int i = 0;
        for (; filename.Text[i]; ++i)
            LastFile[i] = (char)filename.Text[i];
        LastFile[i] = 0;
        return true;
    }
};
CSymbolMap g_DoorTypeSymbolMap;

static const unsigned short kThingDefH[] = { 't','h','i','n','g','_','d','e','f','.','h', 0 };

CSymbolMap* __fastcall GetDoorTypeSymbolMap()
{
    static bool s_loaded = false;
    if (!s_loaded) {
        g_DoorTypeSymbolMap.LoadFromFile(GetDefsDirectory() + kThingDefH);
        s_loaded = true;
    }
    return &g_DoorTypeSymbolMap;
}

static bool Same(const char* a, const char* b)
{
    for (; *a && *a == *b; ++a, ++b)
        ;
    return *a == *b;
}

int main()
{
    CSymbolMap* a = GetDoorTypeSymbolMap();
    CSymbolMap* b = GetDoorTypeSymbolMap();
    if (a != &g_DoorTypeSymbolMap || b != a || a->Loads != 1 || !Same(a->LastFile, "Data\\Defs\\thing_def.h")
        || g_liveStrings != 0) {
        std::printf("0077f510_TEST FAIL\n");
        return 1;
    }
    std::printf("0077f510_TEST PASS\n");
    return 0;
}