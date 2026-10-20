// CShadowRendererSceneData::CShadowRendererSceneData(const CShadowRendererSceneData& other) -- copy constructor: the three shadow-buffer matrices, the spotlight falloff matrix, the fade range, light vector, camera position and the two clip volumes
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B38600, 213 bytes, `ret 4`, returns `this`.
// Leaf: ??0CShadowRendererSceneData
// Callees: CShadowClipVolume::CShadowClipVolume(const CShadowClipVolume&) @ 0x00B36C50 (landed; the manifest's
//   "CEngineCameraDesc" label there is propagated), twice.
// Shape: the compiler-GENERATED copy constructor (CShadowClipVolume's copy is non-trivial, so VC7.1 emits it out of
//   line for the copy site below): the POD matrices are block copies (`rep movsd` of 16, 16, 16 and 12 dwords),
//   the two fade floats move as dwords, the two C3DVectors go word by word through their member-wise user copy
//   constructor, then the clip volumes are copy-constructed in place.
// Layout = Ego_r.pdb (0x240 bytes): WorldToShadowBufferMatrix +0x00, ...HomogenousScreenSpace +0x40,
//   ...TextureSpace +0x80, SpotLightFalloffMatrix +0xC0, ShadowFadeStart +0xF0, ShadowFadeEnd +0xF4,
//   ShadowLightVector +0xF8, CameraPos +0x104, CasterClipVolume +0x110, RecieverClipVolume +0x1A8.
// No engine header exists for CShadowRendererSceneData.
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

inline void* operator new(unsigned int, void* where) { return where; }

// A copy site in the TU makes VC7.1 emit the implicit ??0CShadowRendererSceneData@@QAE@ABU0@@Z out of line.
void CopyShadowRendererSceneData(void* where, const CShadowRendererSceneData& other)
{
    new (where) CShadowRendererSceneData(other);
}