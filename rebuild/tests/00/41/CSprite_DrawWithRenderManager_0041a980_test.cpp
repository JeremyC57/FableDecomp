#include <cstdio>
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
    void (*m_pfnDestroy)(void*);        // +4
    void* m_pObject;                               // +8
};
template <class T> class CCountedPointer { public:
    T*           Data;
    SCountBlock* Info;
    CCountedPointer() : Data(0), Info(0) {}
    CCountedPointer(const CCountedPointer& o) : Data(o.Data), Info(o.Info) { if (Info) ++Info->m_references; }
    ~CCountedPointer()
    {
        if (Info && --Info->m_references == 0) {
            Info->m_pfnDestroy(Info->m_pObject);
            delete Info;
        }
    }
};
class CGraphicDataBank;
float GFAXToVX_009e1da0(float x) { return x * 2.0f; }
float GFAYToVY_009e1dd0(float y) { return y * 4.0f; }
struct CRenderManager2D {
    C2DBoxF got; CGraphicDataBank* gotBank; unsigned long gotGraphic; long refsDuring;
    bool Draw2DScaledGraphic(const C2DBoxF& box, CGraphicDataBank* bank, unsigned long graphic,
                             unsigned long a, unsigned long b);
};
class CRenderManager { public: unsigned char _pad[8]; CRenderManager2D* RenderManager2D; };   // +0x08
class CSystemManager { public: unsigned char _pad[0x60]; CRenderManager* RenderManager; };     // +0x60
CSystemManager* GFGetSystemManager();
struct CFrontEndManager {
    static CFrontEndManager* GetInstance();
    unsigned char _pad[0x10];
    CCountedPointer<CGraphicDataBank> PGraphicBank;      // +0x10
};
struct CSprite {
    bool centred;
    virtual bool IsCentred() { return centred; }
    C2DVector     Size;                           // +0xcc
    C2DVector     RelativeRenderPosition;         // +0xf8
    C2DVector     RelativeRenderZoom;             // +0x108
    unsigned long Width;                          // +0x168
    unsigned long Height;                         // +0x16c
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
    box.TLX = GFAXToVX_009e1da0(box.TLX);
    box.TLY = GFAYToVY_009e1dd0(box.TLY);
    box.BRX = GFAXToVX_009e1da0(box.BRX);
    box.BRY = GFAYToVY_009e1dd0(box.BRY);
    CCountedPointer<CGraphicDataBank> bank = CFrontEndManager::GetInstance()->PGraphicBank;
    CSystemManager* system = GFGetSystemManager();
    system->RenderManager->RenderManager2D->Draw2DScaledGraphic(box, bank.Data, GraphicIndex, 0, 0);
}

static CFrontEndManager g_fe;
static CRenderManager2D g_rm2d;
static CRenderManager g_rm;
static CSystemManager g_sys;
static SCountBlock g_block;
CFrontEndManager* CFrontEndManager::GetInstance() { return &g_fe; }
CSystemManager* GFGetSystemManager() { return &g_sys; }
bool CRenderManager2D::Draw2DScaledGraphic(const C2DBoxF& box, CGraphicDataBank* bank, unsigned long graphic, unsigned long, unsigned long)
{ got = box; gotBank = bank; gotGraphic = graphic; refsDuring = g_block.m_references; return true; }
int main()
{
    g_rm.RenderManager2D = &g_rm2d; g_sys.RenderManager = &g_rm;
    g_block.m_references = 1; g_fe.PGraphicBank.Data = (CGraphicDataBank*)&g_block; g_fe.PGraphicBank.Info = &g_block;
    static CSprite s;
    s.centred = false; s.Size = C2DVector(10, 20); s.RelativeRenderPosition = C2DVector(1, 2);
    s.RelativeRenderZoom = C2DVector(2, 0.5f); s.Width = 0; s.Height = 0; s.GraphicIndex = 7;
    s.DrawWithRenderManager();
    bool ok = g_rm2d.got.TLX == 2 && g_rm2d.got.TLY == 8 && g_rm2d.got.BRX == 42 && g_rm2d.got.BRY == 48
           && g_rm2d.gotGraphic == 7 && g_rm2d.gotBank == (CGraphicDataBank*)&g_block && g_rm2d.refsDuring == 2
           && g_block.m_references == 1;
    s.Width = 4; s.Height = 8; s.centred = true;        // dim (8, 4), centred on pos: TL (1-4, 2-2)
    s.DrawWithRenderManager();
    ok = ok && g_rm2d.got.TLX == -6 && g_rm2d.got.TLY == 0 && g_rm2d.got.BRX == 10 && g_rm2d.got.BRY == 16;
    s.Width = 0x80000000UL; s.centred = false;          // large unsigned width converts positive
    s.DrawWithRenderManager();
    ok = ok && g_rm2d.got.BRX > 1.0e9f;
    g_fe.PGraphicBank.Info = 0;
    if (!ok) { std::printf("0041a980_TEST FAIL\n"); return 1; }
    std::printf("0041a980_TEST PASS\n");
    return 0;
}