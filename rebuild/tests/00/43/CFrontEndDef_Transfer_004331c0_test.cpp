#include <cstdio>
class CCharString {
public:
    void* PStringData;
};
namespace std {
template <class T> class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
};
}
class CPersistContext {
public:
    template <class T> void TransferVectorBinary(std::vector<T>& v);   // 0x004331F9 for CCharString
    void TransferLong(long& value);                      // 0x00431102
};
class CFrontEndDef {
public:
    unsigned char            _base[0x3c];                // CParentDefClassBase (vptr, 0x39 bytes, padding)
    std::vector<CCharString> vAttractModeMovie;          // +0x3c
    long                     ErrorMessageBackgroundGraphic;  // +0x48
    long                     ButtonABigGraphic;          // +0x4c
    long                     ButtonBBigGraphic;          // +0x50
    void Transfer(CPersistContext& context);
};
void CFrontEndDef::Transfer(CPersistContext& context)
{
    context.TransferVectorBinary(vAttractModeMovie);
    context.TransferLong(ErrorMessageBackgroundGraphic);
    context.TransferLong(ButtonABigGraphic);
    context.TransferLong(ButtonBBigGraphic);
}

static void* g_seq[8]; static int g_n;
template <class T> void CPersistContext::TransferVectorBinary(std::vector<T>& v) { g_seq[g_n++] = &v; }
void CPersistContext::TransferLong(long& value) { g_seq[g_n++] = &value; }
int main()
{
    static double mem[0x60 / 8];
    CFrontEndDef* d = (CFrontEndDef*)mem; CPersistContext ctx;
    d->Transfer(ctx);
    bool ok = g_n == 4 && g_seq[0] == &d->vAttractModeMovie && g_seq[1] == &d->ErrorMessageBackgroundGraphic
        && g_seq[2] == &d->ButtonABigGraphic && g_seq[3] == &d->ButtonBBigGraphic && (char*)g_seq[0] - (char*)d == 0x3c;
    std::printf(ok ? "004331c0_TEST PASS\n" : "FAIL\n");
    return 0;
}