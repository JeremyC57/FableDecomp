// Behaviour test for CMediaType::Set @ 0x00CB1E50: self-assignment is a no-op, otherwise free then copy; copy failure -> E_OUTOFMEMORY.
#include <cstdio>
static int g_freed = 0, g_released = 0, g_copies = 0;
static long g_copyResult = 0;
struct IUnknownLike { virtual unsigned long Release() { ++g_released; return 0; } virtual ~IUnknownLike() {} };
struct AM_MEDIA_TYPE { IUnknownLike* pUnk; unsigned long cbFormat; unsigned char* pbFormat; long tag; };
static void CoTaskMemFree(void*) { ++g_freed; }
static long CopyMediaType(AM_MEDIA_TYPE* target, const AM_MEDIA_TYPE* source)
{ ++g_copies; if (g_copyResult < 0) return g_copyResult; target->tag = source->tag; return 0; }
inline void FreeMediaType(AM_MEDIA_TYPE& mt)
{
    if (mt.cbFormat != 0) { CoTaskMemFree(mt.pbFormat); mt.cbFormat = 0; mt.pbFormat = 0; }
    if (mt.pUnk != 0) { mt.pUnk->Release(); mt.pUnk = 0; }
}
struct CMediaType : AM_MEDIA_TYPE {
    long Set(const CMediaType& rt)
    {
        if (&rt != this) {
            FreeMediaType(*this);
            long hr = CopyMediaType(this, &rt);
            if (hr < 0) return (long)0x8007000EL;
        }
        return 0;
    }
};
int main()
{
    unsigned char block[4];
    IUnknownLike unk;
    CMediaType a; a.pUnk = &unk; a.cbFormat = 4; a.pbFormat = block; a.tag = 1;
    CMediaType b; b.pUnk = 0; b.cbFormat = 0; b.pbFormat = 0; b.tag = 2;
    if (a.Set(a) != 0 || g_freed != 0 || g_copies != 0 || a.tag != 1) { std::printf("00cb1e50_TEST FAIL\n"); return 1; }
    if (a.Set(b) != 0 || g_freed != 1 || g_released != 1 || a.pUnk != 0 || a.cbFormat != 0 || a.pbFormat != 0 || a.tag != 2)
    { std::printf("00cb1e50_TEST FAIL\n"); return 1; }
    g_copyResult = -5;
    if (b.Set(a) != (long)0x8007000EL) { std::printf("00cb1e50_TEST FAIL\n"); return 1; }
    std::printf("00cb1e50_TEST PASS\n");
    return 0;
}
