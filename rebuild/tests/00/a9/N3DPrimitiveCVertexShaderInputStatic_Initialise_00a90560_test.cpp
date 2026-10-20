// Behaviour test for 0x00A90560 CVertexShaderInputStatic::Initialise: element list per flag combination and the
// inverted bool passed to AddStream on stream 0.
#include <cstdio>
enum EVertexStreamDataType { VSDT_FLOAT2 = 1, VSDT_FLOAT3 = 2, VSDT_FLOAT4 = 3, VSDT_D3DCOLOR = 4, VSDT_SHORT4 = 7,
                             VSDT_USHORT2N = 11, VSDT_USHORT4N = 12, VSDT_UDEC3 = 13 };
struct CVertexStreamFormat {
    int count; unsigned long usage[8]; int type[8];
    CVertexStreamFormat() : count(0) { }
    void AddDataElement(unsigned long u, EVertexStreamDataType t) { usage[count] = u; type[count] = t; ++count; }
};
struct CVertexShaderInput {
    CVertexStreamFormat last; unsigned long stream; bool flag;
    void AddStream(unsigned long s, const CVertexStreamFormat& f, bool b) { stream = s; last = f; flag = b; }
};
struct CVertexShaderInputStatic : CVertexShaderInput {
    void Initialise(unsigned long flags, bool b)
    {
        CVertexStreamFormat format;
        unsigned long compressed = flags & 4;
        if (compressed) {
            EVertexStreamDataType position = VSDT_UDEC3;
            if (flags & 0x10) position = VSDT_FLOAT3;
            format.AddDataElement(0, position);
            format.AddDataElement(1, VSDT_USHORT4N);
            format.AddDataElement(2, VSDT_SHORT4);
        } else {
            format.AddDataElement(0, VSDT_FLOAT3);
            format.AddDataElement(1, VSDT_FLOAT3);
            format.AddDataElement(2, VSDT_FLOAT2);
        }
        if (flags & 2) {
            if (compressed) format.AddDataElement(3, VSDT_USHORT2N);
            else format.AddDataElement(3, VSDT_FLOAT4);
        }
        AddStream(0, format, !b);
    }
};
static bool Check(unsigned long flags, bool b, int n, const int* types)
{
    CVertexShaderInputStatic s; s.stream = 99; s.Initialise(flags, b);
    if (s.stream != 0 || s.flag != !b || s.last.count != n) return false;
    for (int i = 0; i < n; ++i) if (s.last.usage[i] != (unsigned long)i || s.last.type[i] != types[i]) return false;
    return true;
}
int main()
{
    int plain[3] = { 2, 2, 1 }, plainColour[4] = { 2, 2, 1, 3 }, comp[3] = { 13, 12, 7 }, compColour[4] = { 2, 12, 7, 11 };
    bool ok = Check(0, false, 3, plain) && Check(2, true, 4, plainColour) && Check(4, true, 3, comp)
           && Check(4 | 2 | 0x10, false, 4, compColour);
    if (!ok) { std::printf("00a90560_TEST FAIL\n"); return 1; }
    std::printf("00a90560_TEST PASS\n");
    return 0;
}