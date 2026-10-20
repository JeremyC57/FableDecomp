// Behaviour test for the CShadowRendererSceneData copy constructor (0x00B38600): every member is copied, the clip volumes through their own copy constructor.
#include <cstdio>
#include <new>
struct C3DVector {
    float X, Y, Z;
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
};

struct CMatrix4x4 { float M[4][4]; };
struct CMatrix3x4 { float M[3][4]; };

struct CShadowClipVolume {                                  // 0x98 bytes (landed 0x00B36C50)
    CShadowClipVolume(const CShadowClipVolume& other);      // 0x00B36C50
    unsigned char _body[0x98];
};

struct CShadowRendererSceneData {                          // no user copy constructor: retail's is generated
    CMatrix4x4        WorldToShadowBufferMatrix;            // +0x000
    CMatrix4x4        WorldToShadowBufferMatrixHomogenousScreenSpace;   // +0x040
    CMatrix4x4        WorldToShadowBufferMatrixTextureSpace;            // +0x080
    CMatrix3x4        SpotLightFalloffMatrix;               // +0x0c0
    float             ShadowFadeStart;                      // +0x0f0
    float             ShadowFadeEnd;                        // +0x0f4
    C3DVector         ShadowLightVector;                    // +0x0f8
    C3DVector         CameraPos;                            // +0x104
    CShadowClipVolume CasterClipVolume;                     // +0x110
    CShadowClipVolume RecieverClipVolume;                   // +0x1a8
};


// A copy site in the TU makes VC7.1 emit the implicit ??0CShadowRendererSceneData@@QAE@ABU0@@Z out of line.
void CopyShadowRendererSceneData(void* where, const CShadowRendererSceneData& other)
{
    new (where) CShadowRendererSceneData(other);
}
static int g_clipCopies = 0;
CShadowClipVolume::CShadowClipVolume(const CShadowClipVolume& other)
{
    ++g_clipCopies;
    for (int i = 0; i < 0x98; ++i) _body[i] = (unsigned char)(other._body[i] + 1);   /* marks the path */
}
static unsigned char g_src[sizeof(CShadowRendererSceneData)];
static unsigned char g_dst[sizeof(CShadowRendererSceneData)];
int main()
{
    for (unsigned i = 0; i < sizeof g_src; ++i) { g_src[i] = (unsigned char)(0x40 + i % 13);   /* every dword a normal float */ g_dst[i] = 0; }
    const CShadowRendererSceneData& s = *(const CShadowRendererSceneData*)g_src;
    CopyShadowRendererSceneData(g_dst, s);
    CShadowRendererSceneData& d = *(CShadowRendererSceneData*)g_dst;
    bool ok = g_clipCopies == 2;
    ok = ok && d.CameraPos.Y == s.CameraPos.Y && d.ShadowFadeEnd == s.ShadowFadeEnd;
    /* every byte of the plain part */
    for (unsigned i = 0; i < 0x110; ++i) if (g_dst[i] != g_src[i]) ok = false;
    for (unsigned i = 0x110; i < 0x1a8 + 0x98; ++i) if (g_dst[i] != (unsigned char)(g_src[i] + 1)) ok = false;
    if (!ok) { std::printf("00b38600_TEST FAIL\n"); return 1; }
    std::printf("00b38600_TEST PASS\n");
    return 0;
}
