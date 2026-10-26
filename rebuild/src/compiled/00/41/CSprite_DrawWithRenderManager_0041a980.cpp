// CSprite::DrawWithRenderManager() -- draw the sprite's graphic scaled into its render box (optionally centred) through the 2D render manager
// Leaf: DrawWithRenderManager
// Retail 0x0041A980, 377 bytes. Callees: the sprite's virtual IsCentred (slot 106, +0x1A8), GFAXToVX 0x009E1DA0 /
// GFAYToVY 0x009E1DD0 (__stdcall float), CFrontEndManager::GetInstance 0x0041E5F2 (graphic bank counted pointer at +0x10),
// GFGetSystemManager 0x009A4EC0 (render manager +0x60, 2D manager +8), CRenderManager2D::Draw2DScaledGraphic 0x009DD210,
// operator delete 0x00BFE9BC (bank copy release). Pools: 2^32 0x0122DCB4 (unsigned to float), 0.5f 0x0122F59C.
// Layout: engine/CSprite.h is PDB padding with Width at +0x198; retail sits 0x30 lower (as the landed CSlider rows):
// CComponent Size +0xCC, RelativeRenderPosition +0xF8, RelativeRenderZoom +0x108 (both PDB-0x24), Width +0x168,
// Height +0x16C, GraphicIndex +0x178. Hence a local model.
// Shape: a zero Width/Height falls back to the component size (unsigned to float through `fild ; fadd 2^32`); the box
// is filled field by field (a two-vector ctor computes both sums before storing); the centring offset is built from two
// scalar expressions (a `pos - dim * 0.5f` vector expression spills dim*0.5); the system manager is fetched into a local
// before the call so its call precedes the argument pushes and the bank's Data stays in ebx.
void operator delete(void* p);                   // 0x00BFE9BC
class C2DVector { public:
    float X, Y;
    C2DVector() {}
    C2DVector(float x, float y) : X(x), Y(y) {}
    C2DVector operator+(const C2DVector& v) const { return C2DVector(X + v.X, Y + v.Y); }
    C2DVector operator-(const C2DVector& v) const { return C2DVector(X - v.X, Y - v.Y); }
    C2DVector operator*(float s) const { return C2DVector(X * s, Y * s); }
};
class C2DBoxF { public:
    float TLX, TLY, BRX, BRY;

    void MoveTo(const C2DVector& tl)
    {
        BRX += tl.X - TLX;
        BRY += tl.Y - TLY;
        TLX = tl.X;
        TLY = tl.Y;
    }
};
struct SCountBlock {
    long  m_references;                            // +0
    void (__fastcall* m_pfnDestroy)(void*);        // +4
    void* m_pObject;                               // +8
};
template <class T> class CCountedPointer { public:
    T*           Data;
    SCountBlock* Info;
    CCountedPointer(const CCountedPointer& o) : Data(o.Data), Info(o.Info) { if (Info) ++Info->m_references; }
    ~CCountedPointer()
    {
        if (Info && --Info->m_references == 0) {
            Info->m_pfnDestroy(Info->m_pObject);
            operator delete(Info);
        }
    }
};
class CGraphicDataBank;
float __fastcall GFAXToVX(float);                 // 0x009E1DA0
float __stdcall GFAYToVY_009e1dd0(float y);                 // 0x009E1DD0
struct CRenderManager2D {
    bool Draw2DScaledGraphic(const C2DBoxF& box, CGraphicDataBank* bank, unsigned long graphic,
                             unsigned long a, unsigned long b);   // 0x009DD210
};
class CRenderManager { public: unsigned char _pad[8]; CRenderManager2D* RenderManager2D; };   // +0x08
class CSystemManager { public: unsigned char _pad[0x60]; CRenderManager* RenderManager; };     // +0x60
CSystemManager* __fastcall GFGetSystemManager();   // 0x009A4EC0
struct CFrontEndManager {
    static CFrontEndManager* __fastcall GetInstance();   // 0x0041E5F2
    unsigned char _pad[0x10];
    CCountedPointer<CGraphicDataBank> PGraphicBank;      // +0x10
};
struct CSprite {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055();
    virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063();
    virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071();
    virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079();
    virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086(); virtual void v087();
    virtual void v088(); virtual void v089(); virtual void v090(); virtual void v091(); virtual void v092(); virtual void v093(); virtual void v094(); virtual void v095();
    virtual void v096(); virtual void v097(); virtual void v098(); virtual void v099(); virtual void v100(); virtual void v101(); virtual void v102(); virtual void v103();
    virtual void v104(); virtual void v105();
    virtual bool IsCentred();                     // slot 106 (+0x1a8)
    unsigned char _pad04[0xcc - 4];
    C2DVector     Size;                           // +0xcc
    unsigned char _padd4[0xf8 - 0xd4];
    C2DVector     RelativeRenderPosition;         // +0xf8
    unsigned char _pad100[8];
    C2DVector     RelativeRenderZoom;             // +0x108
    unsigned char _pad110[0x168 - 0x110];
    unsigned long Width;                          // +0x168
    unsigned long Height;                         // +0x16c
    unsigned char _pad170[8];
    unsigned long GraphicIndex;                   // +0x178
    void DrawWithRenderManager();
};

void CSprite::DrawWithRenderManager()
{
    float w = (Width == 0) ? Size.X : (float)Width;
    float h = (Height == 0) ? Size.Y : (float)Height;
    C2DVector pos = RelativeRenderPosition;
    C2DVector dim(w * RelativeRenderZoom.X, h * RelativeRenderZoom.Y);
    C2DBoxF box;
    box.TLX = pos.X;
    box.TLY = pos.Y;
    box.BRX = pos.X + dim.X;
    box.BRY = pos.Y + dim.Y;
    if (IsCentred())
        box.MoveTo(C2DVector(pos.X - dim.X * 0.5f, pos.Y - dim.Y * 0.5f));
    box.TLX = GFAXToVX(box.TLX);
    box.TLY = GFAYToVY_009e1dd0(box.TLY);
    box.BRX = GFAXToVX(box.BRX);
    box.BRY = GFAYToVY_009e1dd0(box.BRY);
    CCountedPointer<CGraphicDataBank> bank = CFrontEndManager::GetInstance()->PGraphicBank;
    CSystemManager* system = GFGetSystemManager();
    system->RenderManager->RenderManager2D->Draw2DScaledGraphic(box, bank.Data, GraphicIndex, 0, 0);
}