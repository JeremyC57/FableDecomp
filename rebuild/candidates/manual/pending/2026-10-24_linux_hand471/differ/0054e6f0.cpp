// NUISystem::CViewport::Draw(CIEngine& engine, CEnginePrimitiveHandle* primitives, long layer, long* count, CComponent* drawer) -- add the viewport's 2D-viewport primitive (screen-sized viewport, the front end's alpha-mask bank and this viewport's graphic index, the on-screen box as the mask clip box; indexed when the caller passes a handle array) on the front-end layer, then draw every child and every drawable fake child into the viewport's own handle
// Grade: DIFFER(690v644) -- the far corner Y (retail `fld pos.Y ; fadd st(1) ; fstp st(0)`, ours `fadd [pos.Y]`: x87 canonicalisation), the screen-size slots (retail +0x10/+0x14, ours split), and the primitive: retail copies the front end's alpha-mask bank pointer first and then stores every member in one block (an inline ctor with all arguments); ours stores most members before the copy; an inline-ctor form pushed arguments (not inlined) and a __forceinline one materialised both box temporaries. Best of 4: member-by-member form
// Leaf: ?Draw@CViewport@NUISystem@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0054E6F0, 641 bytes (+ int3 padding), `ret 14h`. Slot 2 of the CScrollingViewport
//   vtable 0x012468D4 (rtti-vtables.tsv), inherited from CViewport: Ego_r.pdb
//   ?Draw@CViewport@NUISystem@@UAEXAAVCIEngine@@PAVCEnginePrimitiveHandle@@JPAJPAVCComponent@2@@Z (the catalog's
//   "CScrollingViewport::vfunc_2" is the inherited slot).
// Callees: own slots 101 (+0x194, IsIndependent) / 104 (+0x1a0, IsLayerIndependent) as the landed CMovie::Draw
//   0x00547F60, CFrontEndManager::GetInstance 0x0041E5F2 and its slot 36 GetLayer, the engine's slots 28 (+0x70,
//   AddPrimitiveAt) and 23 (+0x5c, AddPrimitive), the children's slot 2 (Draw) and the fake children's slot 105
//   (+0x1a4); operator delete 0x00BFE9BC (the counted pointer's info block).
// Globals: g_bScreenSizeOverride 0x013B8768 with g_OverrideScreenWidth / Height 0x013B876C / 0x013B8770, else
//   g_DisplayWidth / Height 0x01375CD4 / 0x01375CD8 (as the landed rows naming them). 0.125f is an immediate.
// Layout: the CMovie::Draw component model (Size +0xcc, Position +0xf8, Scale +0x108, Layer +0x12f, Children +0xb0);
//   CViewport (Ego_r.pdb less 0x30, STLport vector) Handle +0x15c, FakeChildren +0x164, GraphicIndex +0x170; the front
//   end's alpha-mask bank counted pointer +0x10 (descriptive); CEnginePrimitive2DViewport as Ego_r.pdb (0x48 bytes).
// Shape: as CMovie::Draw: a position copy, a ctor-built scaled size, the layer chosen through two flags with
//   GetInstance in each arm; the primitive is built member by member around a counted-pointer copy of the bank.
namespace NUISystem { class CComponent; }
struct C2DVector {
    float X, Y;
    C2DVector() {}
    C2DVector(float x, float y) : X(x), Y(y) {}
};
struct C2DBoxF {
    float Left, Top, Right, Bottom;
    C2DBoxF() {}
    C2DBoxF(float l, float t, float r, float b) : Left(l), Top(t), Right(r), Bottom(b) {}
};
extern bool  g_bScreenSizeOverride;                 // 0x013B8768
extern float g_OverrideScreenWidth;                 // 0x013B876C
extern float g_OverrideScreenHeight;                // 0x013B8770
extern float g_DisplayWidth;                        // 0x01375CD4
extern float g_DisplayHeight;                       // 0x01375CD8
inline C2DVector GetScreenSize()
{
    if (g_bScreenSizeOverride)
        return C2DVector(g_OverrideScreenWidth, g_OverrideScreenHeight);
    return C2DVector(g_DisplayWidth, g_DisplayHeight);
}

void __cdecl operator delete(void* block);          // 0x00BFE9BC
struct CCPPointerInfo {
    unsigned long RefCount;                         // +0x00
    void (__fastcall* DeleteFunc)(void* data);      // +0x04
    void*         Data;                             // +0x08
};
template <class T> class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
    ~CCountedPointer()
    {
        if (Info && --Info->RefCount == 0) {
            Info->DeleteFunc(Info->Data);
            operator delete(Info);
        }
    }
    T* Get() const { return Data; }
};

class CGraphicDataBank;
struct CEnginePrimitiveThemeChannelDesc { bool A, B, C; };
struct CEnginePrimitive2DViewport {                 // 0x48 bytes (Ego_r.pdb)
    long          Type;                             // +0x00 (0x21)
    long          InterpolationSet;                 // +0x04
    CEnginePrimitiveThemeChannelDesc ThemeChannelDesc;   // +0x08
    long          Layer;                            // +0x0c
    C2DBoxF       Viewport;                         // +0x10
    float         HorizontalFOV;                    // +0x20
    float         VerticalFOV;                      // +0x24
    bool          Use2DFOV;                         // +0x28
    CGraphicDataBank* AlphaMaskBank;                // +0x2c
    unsigned long AlphaMaskBankIndex;               // +0x30
    C2DBoxF       AlphaMaskClipBox;                 // +0x34
    bool          MaskObjectsOutsideAlphaMask;      // +0x44
    bool          FilterMask;                       // +0x45
};

class CEnginePrimitiveHandle;
struct CIEngine {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005();
    virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011();
    virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017();
    virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022();
    virtual void AddPrimitive(CEnginePrimitiveHandle* handle, const CEnginePrimitive2DViewport& primitive, long flags);   // slot 23 (+0x5C)
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027();
    virtual void AddPrimitiveAt(CEnginePrimitiveHandle* handles, long index, const CEnginePrimitive2DViewport& primitive, long flags);   // slot 28 (+0x70)
};
class CEnginePrimitiveHandle {
public:
    virtual void v000();
    void* Data;                                     // +0x04
};

namespace NUISystem {
class CFrontEndManager {
public:
    static CFrontEndManager* __fastcall GetInstance();   // 0x0041E5F2
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005();
    virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011();
    virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017();
    virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035();
    virtual long GetLayer(long depth);              // slot 36 (+0x90)
    unsigned char _pad04[0x10 - 4];
    CCountedPointer<CGraphicDataBank> AlphaMaskBank;   // +0x10 (descriptive)
};

struct CChildPointer {                              // CCountedPointer<CComponent> (8 bytes)
    CComponent* Data;
    void*       Info;
    CComponent* operator->() const { return Data; }
};
struct CChildVector {                               // STLport vector<CCountedPointer<CComponent>>
    CChildPointer* _M_start; CChildPointer* _M_finish; CChildPointer* _M_end_of_storage;
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    CChildPointer& operator[](unsigned int n) { return _M_start[n]; }
};
struct CComponentVector {                           // STLport vector<CComponent*>
    CComponent** _M_start; CComponent** _M_finish; CComponent** _M_end_of_storage;
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    CComponent*& operator[](unsigned int n) { return _M_start[n]; }
};

class CComponent {
public:
    virtual void v000(); virtual void v001();
    virtual void Draw(CIEngine& engine, CEnginePrimitiveHandle* primitives, long layer, long* count, CComponent* drawer);   // slot 2
    virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008();
    virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014();
    virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020();
    virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026();
    virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031(); virtual void v032();
    virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038();
    virtual void v039(); virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044();
    virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049(); virtual void v050();
    virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056();
    virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062();
    virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068();
    virtual void v069(); virtual void v070(); virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074();
    virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079(); virtual void v080();
    virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086();
    virtual void v087(); virtual void v088(); virtual void v089(); virtual void v090(); virtual void v091(); virtual void v092();
    virtual void v093(); virtual void v094(); virtual void v095(); virtual void v096(); virtual void v097(); virtual void v098();
    virtual void v099(); virtual void v100();
    virtual bool IsIndependent();                                   // slot 101 (+0x194)
    virtual void v102(); virtual void v103();
    virtual bool IsLayerIndependent();                              // slot 104 (+0x1a0)
    virtual bool IsDrawable();                                      // slot 105 (+0x1a4) (descriptive)
    unsigned char _pad04[0xb0 - 0x04];
    CChildVector  Children;                         // +0xb0
    unsigned char _padbc[0xcc - 0xbc];
    C2DVector     Size;                             // +0xcc
    unsigned char _padd4[0xf8 - 0xd4];
    C2DVector     Position;                         // +0xf8
    unsigned char _pad100[0x108 - 0x100];
    C2DVector     Scale;                            // +0x108
    unsigned char _pad110[0x12f - 0x110];
    char          Layer;                            // +0x12f
    unsigned char _pad130[0x15c - 0x130];
};

class CViewport : public CComponent {
public:
    CEnginePrimitiveHandle Handle;                  // +0x15c
    CComponentVector       FakeChildren;            // +0x164
    unsigned long          GraphicIndex;            // +0x170
    virtual void Draw(CIEngine& engine, CEnginePrimitiveHandle* primitives, long layer, long* count, CComponent* drawer);
};

void CViewport::Draw(CIEngine& engine, CEnginePrimitiveHandle* primitives, long layer, long* count, CComponent* drawer)
{
    C2DVector pos = Position;
    C2DVector size(Size.X * Scale.X, Size.Y * Scale.Y);
    C2DVector screen = GetScreenSize();
    C2DVector far(pos.X + size.X, pos.Y + size.Y);
    bool independent = IsIndependent();
    bool layerIndependent = IsLayerIndependent();
    long renderLayer;
    if (!independent && !layerIndependent)
        renderLayer = CFrontEndManager::GetInstance()->GetLayer(Layer + layer);
    else
        renderLayer = CFrontEndManager::GetInstance()->GetLayer(Layer);
    CEnginePrimitive2DViewport viewport;
    viewport.Type = 0x21;
    viewport.InterpolationSet = 0;
    viewport.ThemeChannelDesc.A = false;
    viewport.ThemeChannelDesc.B = false;
    viewport.ThemeChannelDesc.C = false;
    viewport.Layer = renderLayer;
    viewport.Viewport = C2DBoxF(0.0f, 0.0f, screen.X, screen.Y);
    viewport.HorizontalFOV = 0.125f;
    viewport.VerticalFOV = 0.0f;
    viewport.Use2DFOV = false;
    viewport.AlphaMaskBank = CCountedPointer<CGraphicDataBank>(CFrontEndManager::GetInstance()->AlphaMaskBank).Get();
    viewport.AlphaMaskBankIndex = GraphicIndex;
    viewport.AlphaMaskClipBox = C2DBoxF(pos.X, pos.Y, far.X, far.Y);
    viewport.MaskObjectsOutsideAlphaMask = false;
    viewport.FilterMask = true;
    if (primitives)
        engine.AddPrimitiveAt(primitives, (*count)++, viewport, 0x40);
    else
        engine.AddPrimitive(&Handle, viewport, 0x40);
    long index = 0;
    unsigned int i;
    for (i = 0; i < Children.size(); ++i)
        Children[i]->Draw(engine, &Handle, 0, &index, 0);
    for (i = 0; i < FakeChildren.size(); ++i)
        if (FakeChildren[i]->IsDrawable())
            FakeChildren[i]->Draw(engine, &Handle, 0, &index, 0);
}

}
