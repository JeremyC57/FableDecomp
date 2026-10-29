// Behaviour test for 0x00DDE7F0 NScript::CQ_PrisonWardenGameScript::CSecretBook::Init: the data string RED / GREEN / BLUE
// sets BookType 0 / 1 / 2 (anything else leaves it), and the data string is then replaced with "NULL".
#include <cstdio>
#include <cstring>

class CCharString { public:
    char Text[16];
    CCharString(const char* t) { std::strcpy(Text, t); }
    bool operator==(const char* t) const { return !std::strcmp(Text, t); }
};
class CScriptThing { public:
    char Data[16]; int Reads;
    CCharString GetDataString() const { ++const_cast<CScriptThing*>(this)->Reads; return CCharString(Data); }
    void SetDataString(const CCharString& d) { std::strcpy(Data, d.Text); }
};
struct CSecretBook {
    CScriptThing Me; long BookType;
    void Init()
    {
        if (Me.GetDataString() == "RED")
            BookType = 0;
        else if (Me.GetDataString() == "GREEN")
            BookType = 1;
        else if (Me.GetDataString() == "BLUE")
            BookType = 2;
        Me.SetDataString("NULL");
    }
};

static int Run(const char* data, long before, long* type, int* reads)
{
    CSecretBook b; std::strcpy(b.Me.Data, data); b.Me.Reads = 0; b.BookType = before;
    b.Init();
    *type = b.BookType; *reads = b.Me.Reads;
    return !std::strcmp(b.Me.Data, "NULL");
}

int main()
{
    long t; int r; int ok = 1;
    ok = ok && Run("RED", 9, &t, &r) && t == 0 && r == 1;
    ok = ok && Run("GREEN", 9, &t, &r) && t == 1 && r == 2;
    ok = ok && Run("BLUE", 9, &t, &r) && t == 2 && r == 3;
    ok = ok && Run("PINK", 9, &t, &r) && t == 9 && r == 3;
    if (!ok) { std::printf("00dde7f0_TEST FAIL\n"); return 1; }
    std::printf("00dde7f0_TEST PASS\n");
    return 0;
}