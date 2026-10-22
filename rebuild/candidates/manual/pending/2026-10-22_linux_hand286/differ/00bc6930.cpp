// Grade: DIFFER(522v530) -- the world-to-view transform: retail loads X/Y on the FPU and copies Z as an int, computes view X/Y/Z eagerly with Y*m3+Z*m6 first and spills them; ours defers view X/Y into the branch. Near limit is 0.0001f (0x0129BA3C).
// CEnginePrimitiveManagerText::RenderPrimitive(CEngineInternalPrimitiveBase& primitive, EEngineRenderLayer layer, CEngineSubPrimitiveChain& chain) -- project a 3D text primitive to the screen through the camera; when in front of the near limit queue a pool-allocated text sub-primitive (screen position, scale -- optionally perspective-scaled by the font height and half the back-buffer width -- colour, string, font) at its depth; always returns 2
// Leaf: RenderPrimitive
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BC6930, length 544 bytes (body 0x212 + int3 padding), `ret 0xc`.
// Callees: CEngineSubPrimitiveBase::operator new 0x00BEB540 (`__fastcall` pool), CBase::CBase 0x0099A2F0, CWideString() 0x0099AED0
//   / operator= 0x0099B7D0, font virtual slot 2 (height), CSurface::GetDimensions 0x009F2F90 on the display manager (0x01436E1C)
//   back buffer (+0x2c), CEngineSubPrimitiveRenderer::AddSubPrimitive 0x00B846E0 (0x01436E74). Camera 0x01436EA0: world-to-view
//   CMatrix3x4 +0x80, projection scale +0xb8/+0xbc, screen centre +0xc8/+0xcc, depth scale/offset +0xdc/+0xe0.
// Layout: local model (landed relative CEnginePrimitiveManagerDecal::RenderPrimitive 0x00BBA340 for the sub-primitive base).
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
};
struct C2DExtentsI { unsigned long Width, Height; };
struct CMatrix3x4 { float M[12]; };
class CBase {
public:
    virtual ~CBase();
    CBase();                                       // 0x0099A2F0
};
struct CWideString {
    void* m_pData;
    CWideString();                                 // 0x0099AED0
    CWideString& operator=(const CWideString& other);   // 0x0099B7D0
};
class CEngineInternalPrimitiveBase;
class CEnginePrimitiveManager;
class __declspec(novtable) CEngineSubPrimitiveBase : public CBase {
public:
    unsigned long                 RenderFlags;      // +0x04
    unsigned char                 Alpha;            // +0x08
    CEngineInternalPrimitiveBase* Parent;           // +0x0c
    CEngineSubPrimitiveBase*      NextSubPrimitive; // +0x10
    CEnginePrimitiveManager*      Manager;          // +0x14
    CEngineSubPrimitiveBase(CEngineInternalPrimitiveBase& parent) : Parent(&parent), NextSubPrimitive(0) { }
    static void* __fastcall operator new(unsigned int size);   // 0x00BEB540
};
struct CFont {
    virtual void v00(); virtual void v04();
    virtual float GetHeight();                      // slot 2
};
class CEngineSubPrimitiveText : public CEngineSubPrimitiveBase {   // vtable 0x012A6858, 0x38 bytes
public:
    C3DVector     Position;                         // +0x18
    float         Scale;                            // +0x24
    unsigned long Colour;                           // +0x28
    CWideString   Text;                             // +0x2c
    CFont*        Font;                             // +0x30
    long          Flags;                            // +0x34
    CEngineSubPrimitiveText(CEngineInternalPrimitiveBase& parent) : CEngineSubPrimitiveBase(parent) { }
    virtual ~CEngineSubPrimitiveText();
};
class CEngineInternalPrimitiveText {
public:
    unsigned char _base[0x48];
    C3DVector     Position;                         // +0x48
    float         Size;                             // +0x54
    unsigned long Colour;                           // +0x58
    unsigned char SortList;                         // +0x5b
    bool          PerspectiveScale;                 // +0x5c
    unsigned char _pad5d[3];
    CWideString   Text;                             // +0x60
    CFont*        Font;                             // +0x64
};
struct CEngineCamera {
    unsigned char _pad00[0x80];
    CMatrix3x4    WorldToView;                      // +0x80
    unsigned char _padb0[0xb8 - 0xb0];
    float         ProjectionScaleX;                 // +0xb8
    float         ProjectionScaleY;                 // +0xbc
    unsigned char _padc0[0xc8 - 0xc0];
    float         ScreenCentreX;                    // +0xc8
    float         ScreenCentreY;                    // +0xcc
    unsigned char _padd0[0xdc - 0xd0];
    float         DepthScale;                       // +0xdc
    float         DepthOffset;                      // +0xe0
};
extern CEngineCamera* g_pEngineCamera;              // 0x01436EA0
struct CSurface { C2DExtentsI GetDimensions() const; };   // 0x009F2F90
struct CDisplayManager { unsigned char _pad[0x2c]; CSurface BackBuffer; };   // +0x2c
extern CDisplayManager* g_pDisplayManager;         // 0x01436E1C
class CEngineSubPrimitiveRenderer {
public:
    void AddSubPrimitive(CEngineSubPrimitiveBase* subPrimitive, float distance, unsigned char list, unsigned long flags,
                         CEnginePrimitiveManager* manager);   // 0x00B846E0
};
extern CEngineSubPrimitiveRenderer* g_pEngineSubPrimitiveRenderer;   // 0x01436E74
enum ERendered { RENDERED_NOTHING = 0, RENDERED_SUB_PRIMITIVES = 2 };
enum EEngineRenderLayer { ENGINE_RENDER_LAYER_DEFAULT = 0 };
class CEngineSubPrimitiveChain;

class CEnginePrimitiveManagerText {
public:
    virtual ERendered RenderPrimitive(CEngineInternalPrimitiveBase& primitive, EEngineRenderLayer layer, CEngineSubPrimitiveChain& chain);
};

ERendered CEnginePrimitiveManagerText::RenderPrimitive(CEngineInternalPrimitiveBase& primitive, EEngineRenderLayer layer,
                                                       CEngineSubPrimitiveChain& chain)
{
    CEngineInternalPrimitiveText& text = (CEngineInternalPrimitiveText&)primitive;
    const CEngineCamera* camera = g_pEngineCamera;
    C3DVector pos = text.Position;
    float vx = pos.X * camera->WorldToView.M[0] + pos.Y * camera->WorldToView.M[3] + pos.Z * camera->WorldToView.M[6] + camera->WorldToView.M[9];
    float vy = pos.X * camera->WorldToView.M[1] + pos.Y * camera->WorldToView.M[4] + pos.Z * camera->WorldToView.M[7] + camera->WorldToView.M[10];
    float vz = pos.X * camera->WorldToView.M[2] + pos.Y * camera->WorldToView.M[5] + pos.Z * camera->WorldToView.M[8] + camera->WorldToView.M[11];
    if (vz > 0.0001f) {
        float invZ = 1.0f / vz;
        float sx = invZ * camera->ProjectionScaleX * vx + camera->ScreenCentreX;
        float sy = camera->ScreenCentreY - vy * camera->ProjectionScaleY * invZ;
        float sz = invZ * camera->DepthScale + camera->DepthOffset;
        float scale = text.Size;
        if (text.PerspectiveScale) {
            scale = invZ * scale / text.Font->GetHeight();
            scale = (float)(g_pDisplayManager->BackBuffer.GetDimensions().Width >> 1) * scale;
        }
        C3DVector screen(sx, sy, sz);
        CEngineSubPrimitiveText* sub = new CEngineSubPrimitiveText(primitive);
        sub->Position = screen;
        sub->Scale = scale;
        sub->Colour = text.Colour;
        sub->Text = text.Text;
        sub->Font = text.Font;
        sub->Flags = 0;
        g_pEngineSubPrimitiveRenderer->AddSubPrimitive(sub, sz, text.SortList, 0, (CEnginePrimitiveManager*)this);
    }
    return RENDERED_SUB_PRIMITIVES;
}
