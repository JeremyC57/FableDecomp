#include <cstdio>
#ifndef _MSC_VER
#define __fastcall
#define __stdcall
typedef long long Int64;
typedef unsigned long long UInt64;
#else
typedef __int64 Int64;
typedef unsigned __int64 UInt64;
#endif
// Behaviour test for 00815010: the cylinder writes BaseName, Radius and Height to the stream in that order.
static long g_longs[4]; static int g_nLongs = 0;
static float g_floats[4]; static int g_nFloats = 0;
static int g_seq[4]; static int g_nSeq = 0;
struct CDataOutputStream {
    void WriteLong(long value) { g_longs[g_nLongs++] = value; g_seq[g_nSeq++] = 1; }
    void WriteFloat(float value) { g_floats[g_nFloats++] = value; g_seq[g_nSeq++] = 2; }
};
struct CDefString { long TablePos; };
struct CPhysicalPrimitiveCylinder {
    virtual void TransferBinaryOut(CDataOutputStream* stream);
    CDefString BaseName;
    float Radius;
    float Height;
};
void CPhysicalPrimitiveCylinder::TransferBinaryOut(CDataOutputStream* stream)
{
    stream->WriteLong(BaseName.TablePos);
    stream->WriteFloat(Radius);
    stream->WriteFloat(Height);
}
int main()
{
    CPhysicalPrimitiveCylinder c; c.BaseName.TablePos = 1234; c.Radius = 0.5f; c.Height = 2.0f;
    CDataOutputStream s;
    c.TransferBinaryOut(&s);
    if (g_nSeq != 3 || g_seq[0] != 1 || g_seq[1] != 2 || g_seq[2] != 2 || g_longs[0] != 1234 || g_floats[0] != 0.5f || g_floats[1] != 2.0f) { std::printf("00815010_TEST FAIL\n"); return 1; }
    std::printf("00815010_TEST PASS\n");
    return 0;
}