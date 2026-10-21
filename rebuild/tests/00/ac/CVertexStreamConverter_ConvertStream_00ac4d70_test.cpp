// Behaviour test for 0x00AC4D70 (CVertexStreamConverter::ConvertStream): per-element conversion between two formats.
#include <cstdio>
#include <cstring>
// Host model: type 0 = float (4 bytes), type 1 = unsigned char (1 byte, X stored as an integer), type 2 = short.
struct V4 { float X, Y, Z, W; V4(float x, float y, float z, float w) : X(x), Y(y), Z(z), W(w) {} };
struct Elem { unsigned long Register; int DataType; };
struct Fmt { const Elem* b; const Elem* e; unsigned long size() const { return (unsigned long)(e - b); } };
static unsigned long Size(int t) { return t == 0 ? 4 : t == 1 ? 1 : 2; }
static int g_from = 0, g_to = 0;
static void From(const char* p, int t, V4& v)
{
    ++g_from;
    if (t == 0) std::memcpy(&v.X, p, 4); else if (t == 1) v.X = (float)(unsigned char)*p; else { short s; std::memcpy(&s, p, 2); v.X = s; }
}
static void To(char* p, int t, const V4& v)
{
    ++g_to;
    if (t == 0) std::memcpy(p, &v.X, 4); else if (t == 1) *p = (char)(unsigned char)v.X; else { short s = (short)v.X; std::memcpy(p, &s, 2); }
}
static void ConvertStream(const void* src, long srcStride, const Fmt& sf, void* dst, long dstStride, const Fmt& df, unsigned long count)
{
    unsigned long srcOffset = 0, dstOffset = 0;
    unsigned long numElements = sf.size() < df.size() ? sf.size() : df.size();
    for (unsigned long i = 0; i < numElements; ++i) {
        int st = sf.b[i].DataType, dt = df.b[i].DataType;
        const char* s = (const char*)src + srcOffset; char* d = (char*)dst + dstOffset;
        V4 v(0, 0, 0, 0);
        for (unsigned long j = 0; j < count; ++j) { From(s, st, v); To(d, dt, v); s += srcStride; d += dstStride; }
        srcOffset += Size(st); dstOffset += Size(dt);
    }
}
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d: %s\n", __LINE__, #c); } } while (0)
int main()
{
    // source: {float, byte} per vertex (stride 5), destination: {short, float} (stride 6); three vertices
    char src[15], dst[18]; std::memset(dst, 0, sizeof dst);
    for (int k = 0; k < 3; ++k) { float f = 10.0f * (k + 1); std::memcpy(src + 5 * k, &f, 4); src[5 * k + 4] = (char)(k + 7); }
    Elem se[2] = { { 0, 0 }, { 1, 1 } }, de[3] = { { 0, 2 }, { 1, 0 }, { 2, 1 } };
    Fmt sf = { se, se + 2 }, df = { de, de + 3 };
    ConvertStream(src, 5, sf, dst, 6, df, 3);
    for (int k = 0; k < 3; ++k) {
        short s; std::memcpy(&s, dst + 6 * k, 2); float f; std::memcpy(&f, dst + 6 * k + 2, 4);
        CHECK(s == 10 * (k + 1)); CHECK(f == (float)(k + 7));
    }
    CHECK(g_from == 6 && g_to == 6);
    g_from = 0; ConvertStream(src, 5, sf, dst, 6, df, 0); CHECK(g_from == 0);
    if (g_fail) { std::printf("00ac4d70_TEST FAIL\n"); return 1; }
    std::printf("00ac4d70_TEST PASS\n");
    return 0;
}