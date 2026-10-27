#include <cstdio>
#include <cstring>
// Behaviour test: a copy of the source's logic over a mock model.
struct BoolVector { int Count; bool Bits[8]; };
static int g_baseCopies;
static char g_key[32];
class CPersistContext { public: void TransferVector(const char* name, BoolVector& v) { std::strcpy(g_key, name); v.Count = 3; } };
class CDefClassBase {
public:
    int GlobalIndex;
    virtual ~CDefClassBase() { }
    CDefClassBase& operator=(const CDefClassBase& o) { GlobalIndex = o.GlobalIndex; ++g_baseCopies; return *this; }
};
class CFireHeartPatternDef : public CDefClassBase {
public:
    BoolVector PadActive;
    void Copy(const CDefClassBase* other) { *this = *(const CFireHeartPatternDef*)other; }
    void Transfer(CPersistContext& context) { context.TransferVector("PadActive", PadActive); }
};
int main()
{
    int ok = 1;
    CFireHeartPatternDef a; a.PadActive.Count = 0; CPersistContext c;
    a.Transfer(c);
    ok = a.PadActive.Count == 3 && std::strcmp(g_key, "PadActive") == 0;
    if (!ok) { std::printf("00d35070_TEST FAIL\n"); return 1; }
    std::printf("00d35070_TEST PASS\n");
    return 0;
}