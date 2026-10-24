// Behaviour test for 006bad20 CPersistContext_TransferableComponent<CFaction>::TransferOut (copy of the model; the
// CFaction constructor/destructor and the traits TransferOut are recording stand-ins).
#include <cstdio>
struct CPersistContext { int id; };
struct CStringParser { int id; };
struct CCharString { struct SBlock* m_pBlock; };
template <class T> struct set { void* _M_header; unsigned long _M_node_count; unsigned long _M_key_compare; };
static int g_live = 0, g_liveInCall = 0;
static const void* g_ctx = 0; static const void* g_out = 0; static const void* g_val = 0;
class CFaction {
public:
    CCharString      Name;
    set<CCharString> AlliedFactions;
    set<CCharString> EnemyFactions;
    CFaction() { ++g_live; }
    ~CFaction() { --g_live; }
};
template <class T> struct CPersistTraits {
    T DefaultVal;
    void TransferOut(CPersistContext& context, CCharString& out, const T* value)
    { g_ctx = &context; g_out = &out; g_val = value; g_liveInCall = g_live; }
};
template <class T> struct CPersistContext_TransferableComponent {
    virtual ~CPersistContext_TransferableComponent() {}
    T* TransferableValue;
    virtual void TransferOut(CPersistContext& context, CCharString& out);
};
template <class T>
void CPersistContext_TransferableComponent<T>::TransferOut(CPersistContext& context, CCharString& out)
{
    CPersistTraits<T> traits;
    traits.TransferOut(context, out, TransferableValue);
}
int main()
{
    CFaction f;
    CPersistContext_TransferableComponent<CFaction> comp; comp.TransferableValue = &f;
    CPersistContext ctx; CCharString out;
    comp.TransferOut(ctx, out);
    int ok = 1;
    if (g_ctx != &ctx || g_out != &out || g_val != &f || g_liveInCall != 2 || g_live != 1) ok = 0;
    if (!ok) { std::printf("006bad20_TEST FAIL\n"); return 1; }
    std::printf("006bad20_TEST PASS\n");
    return 0;
}