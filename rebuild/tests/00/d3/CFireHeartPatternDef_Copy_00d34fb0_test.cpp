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
    CFireHeartPatternDef a, b; a.GlobalIndex = 1; a.PadActive.Count = 0; b.GlobalIndex = 7; b.PadActive.Count = 2; b.PadActive.Bits[1] = true;
    a.Copy(&b);
    ok = a.GlobalIndex == 7 && a.PadActive.Count == 2 && a.PadActive.Bits[1] && g_baseCopies == 1;
    if (!ok) { std::printf("00d34fb0_TEST FAIL\n"); return 1; }
    std::printf("00d34fb0_TEST PASS\n");
    return 0;
}