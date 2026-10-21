// test for 0052ae10 CCompressorZlib::CompressAsImage
#include <stdio.h>
class CCompressionInfo;
class CBaseClass {
public:
    virtual ~CBaseClass() {}
};
class CACompressor : public CBaseClass {
public:
    virtual void CompressAsImage(void* src, unsigned long src_size, void* dst, unsigned long dst_size, long width, long height, long bpp, CCompressionInfo& info);
    virtual void Compress(void* src, unsigned long src_size, void* dst, unsigned long dst_size, CCompressionInfo& info);
    virtual void Decompress(void* src, unsigned long src_size, void* dst, unsigned long dst_size, CCompressionInfo& info);
    virtual void SetQuality(float quality);
};
class CCompressorZlib : public CACompressor {
public:
    virtual void Compress(const void* src, unsigned long src_size, void* dst, unsigned long dst_size, CCompressionInfo& info);
    virtual void CompressAsImage(const void* src, unsigned long src_size, void* dst, unsigned long dst_size, long width, long height, long bpp, CCompressionInfo& info);
    virtual void Decompress(const void* src, unsigned long src_size, void* dst, unsigned long dst_size, CCompressionInfo& info);
};
void CCompressorZlib::CompressAsImage(const void* src, unsigned long src_size, void* dst, unsigned long dst_size, long, long, long, CCompressionInfo& info)
{
    Compress(src, src_size, dst, dst_size, info);
}

class CCompressionInfo { public: int x; };
static const void* g_src; static unsigned long g_ss; static void* g_dst; static unsigned long g_ds; static CCompressionInfo* g_info; static int g_calls;
void CACompressor::CompressAsImage(void*, unsigned long, void*, unsigned long, long, long, long, CCompressionInfo&) {}
void CACompressor::Compress(void*, unsigned long, void*, unsigned long, CCompressionInfo&) {}
void CACompressor::Decompress(void*, unsigned long, void*, unsigned long, CCompressionInfo&) {}
void CACompressor::SetQuality(float) {}
void CCompressorZlib::Compress(const void* s, unsigned long ss, void* d, unsigned long ds, CCompressionInfo& i)
{ g_src = s; g_ss = ss; g_dst = d; g_ds = ds; g_info = &i; ++g_calls; }
void CCompressorZlib::Decompress(const void*, unsigned long, void*, unsigned long, CCompressionInfo&) {}
int main()
{
    CCompressorZlib z; CCompressionInfo info; char a[4], b[8];
    CACompressor* base = &z;
    (void)base;
    z.CompressAsImage(a, 4, b, 8, 640, 480, 32, info);
    int ok = g_calls == 1 && g_src == a && g_ss == 4 && g_dst == b && g_ds == 8 && g_info == &info;
    printf(ok ? "0052ae10_TEST PASS\n" : "0052ae10_TEST FAIL\n");
    return ok ? 0 : 1;
}