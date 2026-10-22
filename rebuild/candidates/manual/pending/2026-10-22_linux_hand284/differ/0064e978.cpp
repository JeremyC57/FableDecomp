// NPlayerGui::CDrawReceivedItems::Draw(CEnginePrimitivePrepareInfo*) -- draw the received-item presentation: for a live hero whose 0xcb UI component is showing, size the handle vector to 2, draw the definition's backing colour over the whole render target (layer 0x4b), then, for a live received item, interpolate its screen box and orientation by the prepare info's interpolation and draw its first mesh as a static 2D mesh (appearance type 5) or, for type 4, as an animating mesh playing the "DEFAULT" animation of the object's graphic sub-def
// Grade: DIFFER(910v910) -- same length and call/x87 sequence, but the frame layout differs (0x13c v 0x158): retail places the render-target dimensions, the mesh vector and the shared background / item box lower and keeps ebx (0) live from the prologue; the by-value CRGBColour argument is built in a frame temp instead of directly in its argument slot. Tried: member-wise lerp statements (fixed the x87 order), an inline box lerp returning by value
// Leaf: Draw
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0064E978, length 910 bytes (biased ebp frame, `ret 4`).
// Callees: GetMainPlayer 0x00449970, GetControlledCreature 0x00487DD0, IS_THING_ALIVE 0x0043DBF0 / 0x00654AF5, bitset::test 0x00410DE0,
//   LowerBound 0x004365B0, handle vector resize 0x00443F90, GetRenderTargetDimensions 0x009BEDC0 (@ 0x013B8390), PeekGuiDef 0x00438570,
//   CEnginePrimitive2DSprite 0x0041BEB0, GetPItem 0x00A01B50, C3DOrientationInterpolator 0x005508D0 (dtor 0x0099A2E0), GetAllMeshes
//   0x004BD420, CEnginePrimitive2DStaticMesh 0x00550940, PeekDefinitionManager 0x00415FBC (@ 0x013B86A0), GetDef 0x0042B040, GetSubDef
//   0x0042B0A2, C3DAnimationBlendState 0x004C2DD0 / 0x004C2DF0 / SetBaseAnimation 0x00A3C260, CCharString 0x0099EBF0 / 0x0099EAE0
//   ("DEFAULT" 0x01230554), FindNthValid 0x005DC340, CEnginePrimitive2DAnimatingMesh 0x00550A70, ~C3DAnimationInterpolator 0x004C2E90,
//   CRT free 0x00BFEA14.
#pragma optimize("s", on)
#include <stdlib.h>
struct CTCBase;
struct CPresenceBits {
    bool test(long bit);                           // 0x00410DE0 (std::bitset::test)
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};
#pragma auto_inline(off)
const CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const
{
    const SEntry* it = first;
    int len = int(last - first);
    while (len > 0) {
        int half = len / 2;
        const SEntry* middle = it + half;
        if (middle->type < *type) {
            it = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return it;
}
#pragma auto_inline(on)
struct CThing {
    unsigned char _pad0[0x20];
    mutable CPresenceBits m_present;               // +0x20
    unsigned char _pad1[0x23];
    CInterfaceMap m_interfaces;                    // +0x44
    template <class T> __forceinline bool GetInterface(T** ppOut, long type) const
    {
        if (m_present.test(type)) {
            long key = type;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&key);
            *ppOut = (T*)((it == map->last || type < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
template <class T> bool __fastcall IS_THING_ALIVE(const T& thing);   // 0x0043DBF0
bool IsThingAliveImpl(const void* thing);
#pragma auto_inline(off)
template <class T> bool __fastcall IS_THING_ALIVE(const T& thing)
{
    return thing != 0 && IsThingAliveImpl(thing);
}
#pragma auto_inline(on)
#include <stdlib.h>
struct CPlayer {
    CThing* GetControlledCreature();               // 0x00487DD0
};
struct CPlayerManager {
    CPlayer* GetMainPlayer();                      // 0x00449970
};
struct CThingObject;
template <class T> bool __fastcall IS_THING_ALIVE2(const T& thing);   // 0x00654AF5
#pragma auto_inline(off)
template <class T> bool __fastcall IS_THING_ALIVE2(const T& thing)
{
    return thing != 0 && IsThingAliveImpl(thing);
}
#pragma auto_inline(on)
struct C2DVector {
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) {}
};
struct C2DBoxF {
    float TLX, TLY, BRX, BRY;
    C2DBoxF(const C2DVector& tl, const C2DVector& br) : TLX(tl.X), TLY(tl.Y), BRX(br.X), BRY(br.Y) {}
    C2DBoxF(float tlx, float tly, float brx, float bry) : TLX(tlx), TLY(tly), BRX(brx), BRY(bry) {}
    C2DBoxF() {}
};
inline float Lerp(float a, float b, float t) { return a + (b - a) * t; }
struct C2DExtentsI { long Width, Height; };
class CDisplayManager { public: C2DExtentsI GetRenderTargetDimensions(); };   // 0x009BEDC0
extern CDisplayManager* g_pDisplayManager;          // 0x013B8390
struct CRGBColour {
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
struct C4DVector {
    float X, Y, Z, W;
    C4DVector(float x, float y, float z, float w) : X(x), Y(y), Z(z), W(w) {}
};
struct CRightHandedSet { float M[6]; };
class C3DOrientationInterpolator {
public:
    unsigned char _body[0x10];
    C3DOrientationInterpolator(const CRightHandedSet& from, const CRightHandedSet& to);   // 0x005508D0
    ~C3DOrientationInterpolator();                   // 0x0099A2E0 (CBaseClass dtor)
};
class C3DAnimationInterpolator { public: unsigned char _body[0x10]; ~C3DAnimationInterpolator(); };   // 0x004C2E90
struct C3DAnimation;
struct C3DAnimationStateBase {
    void* _unused;
    virtual void v00();
    ~C3DAnimationStateBase() {}
};
struct C3DAnimationState : C3DAnimationStateBase {
    long          Flag;
    C3DAnimation* Animation;
    float         Time;
    C3DAnimationState(C3DAnimation* animation) : Flag(1), Animation(animation), Time(0.0f) {}
    virtual void v00();
};
class C3DAnimationBlendState {
public:
    unsigned char _body[0x8];
    C3DAnimationBlendState();                        // 0x004C2DD0
    ~C3DAnimationBlendState();                       // 0x004C2DF0
    void SetBaseAnimation(const C3DAnimationState& state);   // 0x00A3C260
};
class CCharString {
public:
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                  // 0x0099EAE0
};
struct CAnimationPredicateBase {
    virtual bool Test(const void* entry) const = 0;
    ~CAnimationPredicateBase() {}
};
struct CAnimationIsValid : CAnimationPredicateBase {
    virtual bool Test(const void* entry) const;
};
struct CAnimationList {
    C3DAnimation** FindNthValid(const CCharString& name, long n, const CAnimationPredicateBase& valid);   // 0x005DC340
};
struct CMeshInfo { unsigned long Mesh; unsigned long _pad[3]; };
struct MeshVector {
    CMeshInfo *Start, *Finish, *End;
    MeshVector() : Start(0), Finish(0), End(0) {}
    ~MeshVector() { if (Start) free(Start); }
};
struct CTCGraphicAppearance {
    unsigned char _pad[0x51];
    unsigned char AppearanceType;                    // +0x51
    void GetAllMeshes(MeshVector& meshes);           // 0x004BD420
};
struct CThingObject {
    unsigned char         _pad[0x64];
    CTCGraphicAppearance* Appearance;                // +0x64
    unsigned char         _pad68[0x8c - 0x68];
    unsigned short        DefIndex;                  // +0x8c
};
struct CBaseIntelligentPointer { void* m_p; CThingObject* GetPItem() const; };   // 0x00A01B50
struct CRefCounted { long m_count; };
struct CDefVirtual { virtual void v00(); virtual void Destroy(); };
struct CDefBase : CDefVirtual, CRefCounted {
    __forceinline void Release() { if (--m_count == 0) Destroy(); }
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) {}
    __forceinline ~CDefPointer() { if (Object) { Object->Release(); Object = 0; } }
};
struct CGraphicDef : CDefBase { unsigned char _pad08[0x34 - 0x08]; CAnimationList Animations; };   // +0x34
struct CObjectDef : CDefBase { bool GetSubDef(CDefPointer<CGraphicDef>* out); };   // 0x0042B0A2
class CDefinitionManager { public: bool GetDef(long index, CDefPointer<CObjectDef>* out); };   // 0x0042B040
class CMainGameComponent { public: CDefinitionManager* PeekDefinitionManager(); };   // 0x00415FBC
extern CMainGameComponent* g_pMainGameComponent;    // 0x013B86A0
struct CTCReceivedItemUI {
    unsigned char           _pad00[0x1c];
    CBaseIntelligentPointer Item;                    // +0x1c
    unsigned char           _pad20[0x26 - 0x20];
    bool                    Showing;                 // +0x26
    unsigned char           _pad27[0x4c - 0x27];
    C2DBoxF                 StartBox;                // +0x4c
    C2DBoxF                 EndBox;                  // +0x5c
    CRightHandedSet         StartOrientation;        // +0x6c
    CRightHandedSet         EndOrientation;          // +0x84
};
struct CEnginePrimitiveHandle { unsigned long Data[2]; };
class CEnginePrimitive2DSprite {
public:
    unsigned char _body[0x54];
    CEnginePrimitive2DSprite(unsigned __int64 id, const C2DBoxF& box, long layer, unsigned long graphic, unsigned long b, long blend,
                             long interpolation, float scaleX, float scaleY, unsigned char alpha, unsigned char red, unsigned char green,
                             unsigned char blue, unsigned long e, const C2DVector& uv0, const C2DVector& uv1);   // 0x0041BEB0
};
class CEnginePrimitive2DStaticMesh {
public:
    unsigned char              _body[0x24];
    C3DOrientationInterpolator Orientation;           // +0x24
    unsigned char              _tail[0x10];
    CEnginePrimitive2DStaticMesh(unsigned __int64 id, unsigned long mesh, const C2DBoxF& box, const C3DOrientationInterpolator& orientation,
                                 long interpolation, unsigned char alpha, CRGBColour colour);   // 0x00550940
};
class CEnginePrimitive2DAnimatingMesh {
public:
    unsigned char              _body[0x24];
    C3DOrientationInterpolator Orientation;           // +0x24
    unsigned char              _pad34[0x58 - 0x34];
    C3DAnimationInterpolator   Animation;             // +0x58
    unsigned char              _tail[0x18];
    CEnginePrimitive2DAnimatingMesh(unsigned __int64 id, unsigned long mesh, const C3DAnimationBlendState& blend, const C2DBoxF& box,
                                    const C4DVector& position, const C3DOrientationInterpolator& orientation, long interpolation,
                                    unsigned char alpha, CRGBColour colour);   // 0x00550A70
};
class CEnginePrimitiveRenderer {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22();
    virtual void AddPrimitive(CEnginePrimitiveHandle* handle, const void* primitive, long flags);   // slot 23
};
class CEnginePrimitivePrepareInfo {
public:
    unsigned char             _pad00[0x14];
    float                     Interpolation;        // +0x14
    unsigned char             _pad18[0x34 - 0x18];
    CEnginePrimitiveRenderer* Renderer;             // +0x34
};
struct CHandleVector {
    CEnginePrimitiveHandle *Start, *Finish, *End;
    void resize(unsigned int n);                     // 0x00443F90
};
struct CPlayerGuiDef { unsigned char _pad[0x8dc]; CRGBColour ReceivedItemBackColour; };   // +0x8dc
extern CPlayerGuiDef* __fastcall PeekGuiDef();      // 0x00438570

namespace NPlayerGui {
struct CDrawReceivedItems {
    virtual void v00();
    void*           PlayerGui;                      // +0x04
    CPlayerManager* PlayerManager;                  // +0x08
    void*           World;                          // +0x0c
    void*           Resources;                      // +0x10
    void*           Bank;                           // +0x14
    bool            Drawing;                        // +0x18
    CHandleVector   Handles;                        // +0x1c
    void Draw(CEnginePrimitivePrepareInfo* info);
};

void CDrawReceivedItems::Draw(CEnginePrimitivePrepareInfo* info)
{
    CThing* hero = PlayerManager->GetMainPlayer()->GetControlledCreature();
    CTCReceivedItemUI* ui;
    if (!IS_THING_ALIVE(hero) || !hero->GetInterface(&ui, 0xcb) || !ui->Showing)
        return;
    Drawing = true;
    Handles.resize(2);
    C2DExtentsI dimensions = g_pDisplayManager->GetRenderTargetDimensions();
    C2DVector screen((float)dimensions.Width, (float)dimensions.Height);
    const CRGBColour& back = PeekGuiDef()->ReceivedItemBackColour;
    CEnginePrimitive2DSprite background(0, C2DBoxF(C2DVector(0.0f, 0.0f), screen), 0x4b, 0, 0, 2, 0, 0.0f, 0.0f, back.A, back.R, back.G,
                                        back.B, 0, C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
    info->Renderer->AddPrimitive(&Handles.Start[0], &background, 0xc0);
    CThingObject* item = ui->Item.GetPItem();
    if (!IS_THING_ALIVE2(item))
        return;
    float t = info->Interpolation;
    C2DBoxF box;
    box.TLX = Lerp(ui->StartBox.TLX, ui->EndBox.TLX, t);
    box.TLY = Lerp(ui->StartBox.TLY, ui->EndBox.TLY, t);
    box.BRX = Lerp(ui->StartBox.BRX, ui->EndBox.BRX, t);
    box.BRY = Lerp(ui->StartBox.BRY, ui->EndBox.BRY, t);
    C3DOrientationInterpolator orientation(ui->StartOrientation, ui->EndOrientation);
    MeshVector meshes;
    item->Appearance->GetAllMeshes(meshes);
    unsigned char type = item->Appearance->AppearanceType;
    if (type == 5) {
        CEnginePrimitive2DStaticMesh mesh(0, meshes.Start->Mesh, box, orientation, 1, 0xff, CRGBColour(0xff, 0xff, 0xff, 0xff));
        info->Renderer->AddPrimitive(&Handles.Start[1], &mesh, 0xc0);
    } else if (type == 4) {
        CDefPointer<CObjectDef> objectDef;
        g_pMainGameComponent->PeekDefinitionManager()->GetDef(item->DefIndex, &objectDef);
        CDefPointer<CGraphicDef> graphicDef;
        objectDef.Object->GetSubDef(&graphicDef);
        C3DAnimationBlendState blend;
        blend.SetBaseAnimation(C3DAnimationState(*graphicDef.Object->Animations.FindNthValid(CCharString("DEFAULT"), 0, CAnimationIsValid())));
        CEnginePrimitive2DAnimatingMesh mesh(0, meshes.Start->Mesh, blend, box, C4DVector(0.0f, 0.0f, 0.0f, 1.0f), orientation, 1, 0xff,
                                             CRGBColour(0xff, 0xff, 0xff, 0xff));
        info->Renderer->AddPrimitive(&Handles.Start[1], &mesh, 0xc0);
    }
}
}
