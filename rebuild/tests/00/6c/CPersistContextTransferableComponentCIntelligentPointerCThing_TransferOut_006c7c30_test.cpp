// Behaviour test for 006c7c30 CPersistContext_TransferableComponent<CIntelligentPointer<CThing> >::TransferOut (copy
// of the model; the traits' DefaultVal construction/destruction is counted).
#include <cstdio>
class CPersistContext { public: int id; };
class CStringParser { public: int id; };
static unsigned long long g_appended[4];
static int g_n = 0;
class CCharString { public:
    unsigned long long v;
    explicit CCharString(unsigned long long value) : v(value) {}
    const CCharString& operator+=(const CCharString& other) { g_appended[g_n++] = other.v; return *this; }
};
class CThing { public: unsigned char _pad00[0x18]; unsigned long long UniqueID; };
static int g_ipLive = 0;
struct CBaseIntelligentPointer {
    CThing* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) { ++g_ipLive; }
    virtual ~CBaseIntelligentPointer() { --g_ipLive; }
    CThing* GetPItem() const { return m_pItem; }
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public: CIntelligentPointer() {} };
template <class T> struct CPersistTraits {
    T DefaultVal;
    void TransferOut(CPersistContext& context, CCharString& out, T& value)
    {
        unsigned long long uid = 0;
        if (value.GetPItem())
            uid = value.GetPItem()->UniqueID;
        out += CCharString(uid);
    }
};
class CPersistContext_TransferableComponentGenericType { public:
    virtual void TransferOut(CPersistContext& context, CCharString& out) = 0;
    virtual ~CPersistContext_TransferableComponentGenericType() {}
};
template <class T> struct CPersistContext_TransferableComponent : CPersistContext_TransferableComponentGenericType {
    T* TransferableValue;
    virtual void TransferOut(CPersistContext& context, CCharString& out);
};
template <class T>
void CPersistContext_TransferableComponent<T>::TransferOut(CPersistContext& context, CCharString& out)
{
    CPersistTraits<T> traits;
    traits.TransferOut(context, out, *TransferableValue);
}
int main()
{
    CThing t; t.UniqueID = 77;
    CIntelligentPointer<CThing> p; p.m_pItem = &t;
    int before = g_ipLive;
    CPersistContext_TransferableComponent<CIntelligentPointer<CThing> > comp; comp.TransferableValue = &p;
    CPersistContext ctx; CCharString out(0);
    CPersistContext_TransferableComponentGenericType* base = &comp;
    base->TransferOut(ctx, out);
    p.m_pItem = 0;
    base->TransferOut(ctx, out);
    int ok = 1;
    if (g_n != 2 || g_appended[0] != 77 || g_appended[1] != 0 || g_ipLive != before) ok = 0;
    if (!ok) { std::printf("006c7c30_TEST FAIL\n"); return 1; }
    std::printf("006c7c30_TEST PASS\n");
    return 0;
}