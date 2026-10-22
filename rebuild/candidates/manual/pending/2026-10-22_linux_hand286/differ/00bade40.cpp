// Grade: DIFFER(726v793) -- lane gap: retail converts the faded alpha with an inline-asm FloatToInt (fstp [esp+0x10]; fld [esp+0x10]; fistp [esp+0x14]; mov ebp,[esp+0x14]) that byte-pure C++ cannot emit (same as 00b3b240); rest of the body is first-pass
// CEnginePrimitiveManagerSprites::RenderSprite(CEngineInternalPrimitiveSprite* sprite) -- queue one 3D sprite: inside a render-state capture block, skip it beyond its fade-out distance, fade its alpha between the fade distances, compute its bounding radius, cull it against the camera's near/far distances and four side planes, then start its graphic frame loading and add a pool-allocated sprite sub-primitive at its distance
// Leaf: RenderSprite
// Layout: local model; camera @ 0x01436EA0 (position +0x54, near +0x6c, far +0x70, view-direction row +0x1b4, side planes
//   +0x1c0/+0x1d0/+0x1e0/+0x1f0); the sprite's fade distances +0x14/+0x18, position +0x48, bank index +0x54, frame +0x5c,
//   size +0x60/+0x64, size scales +0x68/+0x6c, radius +0x70, alpha byte +0x83, flags +0x84.
#include <math.h>
struct C3DVector { float X, Y, Z; };
struct CTrackedRenderState;
struct SCapturedRenderState {
    CTrackedRenderState* m_pState;
    unsigned long        m_priorValue;
};
struct CRenderManagerCoreView {
    unsigned char        _pad0000[0x8];
    SCapturedRenderState m_captured[0x400];        // +0x0008
    CTrackedRenderState* m_dirty[0x200];           // +0x2008
    long                 m_capturedCount;          // +0x2808
    long                 m_dirtyCount;             // +0x280C
    long                 m_unknown2810;            // +0x2810
    unsigned long        m_captureMask;            // +0x2814
    unsigned char        _pad2818[0x3a24 - 0x2818];
    unsigned char        m_captureBlockMarker[4];  // +0x3A24
    __forceinline void OpenCaptureBlock()
    {
        m_captured[m_capturedCount++].m_pState = (CTrackedRenderState*)m_captureBlockMarker;
        m_captureMask <<= 1;
    }
    void RestoreCaptureBlock();                    // 0x00A05840
};
extern CRenderManagerCoreView* g_pRenderStateManager;  // 0x013CB508
struct CRenderStateCaptureBlock {
    __forceinline CRenderStateCaptureBlock() { g_pRenderStateManager->OpenCaptureBlock(); }
    __forceinline ~CRenderStateCaptureBlock() { g_pRenderStateManager->RestoreCaptureBlock(); }
};
struct CEngineCamera {
    unsigned char _pad000[0x54];
    C3DVector     Position;                        // +0x54
    unsigned char _pad060[0x6c - 0x60];
    float         NearDistance;                    // +0x6c
    float         FarDistance;                     // +0x70
    unsigned char _pad074[0x1b4 - 0x74];
    C3DVector     ViewDirection;                   // +0x1b4
    unsigned char _pad1c0[4];
    C3DVector     Plane0;                          // +0x1c0
    unsigned char _pad1cc[4];
    C3DVector     Plane1;                          // +0x1d0
    unsigned char _pad1dc[4];
    C3DVector     Plane2;                          // +0x1e0
    unsigned char _pad1ec[4];
    C3DVector     Plane3;                          // +0x1f0
};
extern CEngineCamera* g_pEngineCamera_01436ea0;   // 0x01436EA0
struct CGraphicInfoFrame {                         // 16 bytes
    unsigned char _pad[0x10];
    void BeginLoading(unsigned long a, unsigned long b);   // 0x009FE620
};
struct SBankGraphic {                              // 0x2c bytes
    unsigned char      _pad[0x28];
    CGraphicInfoFrame* m_pFrames;                  // +0x28
};
template <class T> struct CRetailVector {
    T* m_pFirst;
    T* m_pLast;
    T* m_pEnd;
    T* begin() { return m_pFirst; }
    T* end() { return m_pLast; }
    unsigned long size() { return end() - begin(); }
    T& operator[](unsigned long n) { return *(begin() + n); }
};
struct CGraphicBank {
    unsigned char               _pad[0x1e0];
    CRetailVector<SBankGraphic> m_records;         // +0x1e0
    __forceinline SBankGraphic* Get(unsigned long index) { return index < m_records.size() ? &m_records[index] : 0; }
};
struct CEngineGlobal_01436e98 {
    unsigned char _pad[0x954];
    CGraphicBank* m_pGraphicBank;                  // +0x954
};
extern CEngineGlobal_01436e98* g_pEngine_01436e98;   // 0x01436E98
struct CEngineInternalPrimitiveSprite {
    unsigned char _pad00[0x14];
    float         FadeStart;                       // +0x14
    float         FadeEnd;                         // +0x18
    unsigned char _pad1c[0x48 - 0x1c];
    C3DVector     Position;                        // +0x48
    unsigned long BankIndex;                       // +0x54
    unsigned char _pad58[4];
    unsigned long FrameIndex;                      // +0x5c
    float         Width;                           // +0x60
    float         Height;                          // +0x64
    float         WidthScale;                      // +0x68
    float         HeightScale;                     // +0x6c
    float         Radius;                          // +0x70
    unsigned char _pad74[0x83 - 0x74];
    unsigned char Alpha;                           // +0x83
    unsigned char Flags;                           // +0x84
};
struct CBase {
    virtual void v00();
    CBase();                                       // 0x0099A2F0
};
struct __declspec(novtable) CEngineSubPrimitiveBase : CBase {
    unsigned char _pad04[8];
    void*         m_pPrimitive;                    // +0x0c
    long          m_pNext;                         // +0x10
    CEngineSubPrimitiveBase(void* primitive) : CBase(), m_pPrimitive(primitive), m_pNext(0) { }
    static void* __fastcall operator new(unsigned int size);   // 0x00BEB540
};
struct CEngineSubPrimitiveSprite : CEngineSubPrimitiveBase {   // 0x18 bytes, vtable 0x012A5430
    unsigned char _pad14[4];
    CEngineSubPrimitiveSprite(CEngineInternalPrimitiveSprite* sprite) : CEngineSubPrimitiveBase(sprite) { }
    virtual void v00();
};
struct CEnginePrimitiveManagerSprites;
struct CEngineSubPrimitiveRenderer {
    void AddSortedSubPrimitive(CEngineSubPrimitiveBase* sub, float distance, long alpha, long list, CEnginePrimitiveManagerSprites* manager);   // 0x00B846E0
};
extern CEngineSubPrimitiveRenderer* g_pEngineSubPrimitiveRenderer;   // 0x01436E74

struct CEnginePrimitiveManagerSprites {
    void RenderSprite(CEngineInternalPrimitiveSprite* sprite);
};

void CEnginePrimitiveManagerSprites::RenderSprite(CEngineInternalPrimitiveSprite* sprite)
{
    CRenderStateCaptureBlock capture;
    float width = sprite->Width;
    float height = sprite->Height;
    unsigned long alpha = sprite->Alpha;
    float dx = sprite->Position.X - g_pEngineCamera_01436ea0->Position.X;
    float dy = sprite->Position.Y - g_pEngineCamera_01436ea0->Position.Y;
    float dz = sprite->Position.Z - g_pEngineCamera_01436ea0->Position.Z;
    float distance = (float)sqrt(dx * dx + dy * dy + dz * dz);
    if (distance > sprite->FadeEnd)
        return;
    if (distance > sprite->FadeStart) {
        float faded = (sprite->FadeEnd - distance) * (float)alpha / (sprite->FadeEnd - sprite->FadeStart);
        alpha = (long)faded;
    }
    float w = (float)fabs(width * sprite->WidthScale) + width;
    float h = (float)fabs(height * sprite->HeightScale) + height;
    float radius = sprite->Radius = (float)sqrt(w * w + h * h);
    CEngineCamera* camera = g_pEngineCamera_01436ea0;
    C3DVector v;
    v.X = sprite->Position.X - camera->Position.X;
    v.Y = sprite->Position.Y - camera->Position.Y;
    v.Z = sprite->Position.Z - camera->Position.Z;
    float depth = v.Z * camera->ViewDirection.Z + v.Y * camera->ViewDirection.Y + v.X * camera->ViewDirection.X;
    if (depth + radius < camera->NearDistance)
        return;
    if (depth - radius > camera->FarDistance)
        return;
    if (v.Z * camera->Plane2.Z + v.Y * camera->Plane2.Y + v.X * camera->Plane2.X > radius)
        return;
    if (v.Z * camera->Plane3.Z + v.Y * camera->Plane3.Y + v.X * camera->Plane3.X > radius)
        return;
    if (v.Z * camera->Plane0.Z + v.Y * camera->Plane0.Y + v.X * camera->Plane0.X > radius)
        return;
    if (v.Z * camera->Plane1.Z + v.Y * camera->Plane1.Y + v.X * camera->Plane1.X > radius)
        return;
    sprite->Flags &= ~0x20;
    SBankGraphic* graphic = g_pEngine_01436e98->m_pGraphicBank->Get(sprite->BankIndex);
    (graphic ? &graphic->m_pFrames[sprite->FrameIndex] : 0)->BeginLoading(0, 0);
    CEngineSubPrimitiveSprite* sub = new CEngineSubPrimitiveSprite(sprite);
    g_pEngineSubPrimitiveRenderer->AddSortedSubPrimitive(sub, distance, alpha, 1, this);
}
