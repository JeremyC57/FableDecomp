// Behaviour test for 006c9b30 CPersistContext_TransferableComponent<CIntelligentPointer<CThing> >::TransferIn (copy
// of the model; the traits TransferIn records its arguments, the DefaultVal lifetime is counted).
#include <cstdio>
struct CPersistContext { int id; };
struct CStringParser { int id; };
struct CCharString;
class CThing { public: int id; };
static int g_ipLive = 0, g_liveInCall = 0;
static const void* g_ctx = 0; static const void* g_parser = 0; static const void* g_val = 0;
struct CBaseIntelligentPointer {
    CThing* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) { ++g_ipLive; }
    virtual ~CBaseIntelligentPointer() { --g_ipLive; }
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer { CIntelligentPointer() {} };
template <class T> struct CPersistTraits {
    T DefaultVal;
    void TransferIn(CPersistContext& context, CStringParser& parser, T& value)
    { g_ctx = &context; g_parser = &parser; g_val = &value; g_liveInCall = g_ipLive; }
};
template <class T> struct CPersistContext_TransferableComponent {
    virtual ~CPersistContext_TransferableComponent() {}
    T* TransferableValue;
    virtual void TransferIn(CPersistContext& context, CStringParser& parser);
};
template <class T>
void CPersistContext_TransferableComponent<T>::TransferIn(CPersistContext& context, CStringParser& parser)
{
    CPersistTraits<T> traits;
    traits.TransferIn(context, parser, *TransferableValue);
}
int main()
{
    CIntelligentPointer<CThing> p;
    int before = g_ipLive;
    CPersistContext_TransferableComponent<CIntelligentPointer<CThing> > comp; comp.TransferableValue = &p;
    CPersistContext ctx; CStringParser parser;
    comp.TransferIn(ctx, parser);
    int ok = 1;
    if (g_ctx != &ctx || g_parser != &parser || g_val != &p) ok = 0;
    if (g_liveInCall != before + 1 || g_ipLive != before) ok = 0;   // the default lives only during the call
    if (!ok) { std::printf("006c9b30_TEST FAIL\n"); return 1; }
    std::printf("006c9b30_TEST PASS\n");
    return 0;
}