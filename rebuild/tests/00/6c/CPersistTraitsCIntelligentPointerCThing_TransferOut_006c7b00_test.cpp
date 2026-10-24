// Behaviour test for 006c7b00 CPersistTraits<CIntelligentPointer<CThing> >::TransferOut (copy of the model; the
// string is a tiny recorder of the appended 64-bit values).
#include <cstdio>
struct CPersistContext { int id; };
static unsigned long long g_appended[4];
static int g_n = 0, g_live = 0;
struct CCharString {
    unsigned long long v;
    explicit CCharString(unsigned long long value) : v(value) { ++g_live; }
    ~CCharString() { --g_live; }
    CCharString& operator+=(const CCharString& other) { g_appended[g_n++] = other.v; return *this; }
};
class CThing {
public:
    unsigned char      _pad00[0x18];
    unsigned long long UniqueID;
};
static int g_gets = 0;
struct CBaseIntelligentPointer {
    virtual ~CBaseIntelligentPointer() {}
    CThing* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) {}
    CThing* GetPItem() const { ++g_gets; return m_pItem; }
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer { CIntelligentPointer() {} };
template <class T> struct CPersistTraits {
    T DefaultVal;
    void TransferOut(CPersistContext& context, CCharString& out, T& value);
};
template <>
void CPersistTraits<CIntelligentPointer<CThing> >::TransferOut(CPersistContext& context, CCharString& out, CIntelligentPointer<CThing>& value)
{
    unsigned long long uid = 0;
    if (value.GetPItem())
        uid = value.GetPItem()->UniqueID;
    out += CCharString(uid);
}
int main()
{
    CPersistContext ctx; CCharString out(0); g_live = 0;
    CPersistTraits<CIntelligentPointer<CThing> > traits;
    CIntelligentPointer<CThing> p;
    traits.TransferOut(ctx, out, p);                                   // no thing: 0
    CThing t; t.UniqueID = 0x123456789ULL;
    p.m_pItem = &t;
    traits.TransferOut(ctx, out, p);
    int ok = 1;
    if (g_n != 2 || g_appended[0] != 0 || g_appended[1] != 0x123456789ULL || g_live != 0 || g_gets != 3) ok = 0;
    if (!ok) { std::printf("006c7b00_TEST FAIL\n"); return 1; }
    std::printf("006c7b00_TEST PASS\n");
    return 0;
}