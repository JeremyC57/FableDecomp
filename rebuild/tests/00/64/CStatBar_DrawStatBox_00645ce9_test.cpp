#ifndef _MSC_VER
#define __fastcall
#endif
#include <stdio.h>
#include <string.h>
// NPlayerGui::CStatBar::DrawStatBox(const CEnginePrimitivePrepareInfo& info, const CGraphicDataBank& bank, const C2DVector& pos, long graphic, long value, long effect, const CWideString& text, unsigned char alpha, bool withText, bool flag) -- draw the box through the static helper, place the change text (right-aligned under the box when shown) and keep the box's particle effect alive at the text position
// Leaf: DrawStatBox@CStatBar
// Retail: Fable.exe (TLC, VC7.1) @ 0x00645CE9, 578 bytes, `ret 28h`, frame-pointer size TU (`leave`).
// Callees: the static DrawStatBox 0x00645853 (__fastcall: info in ecx, handles in edx), PeekGuiDef 0x00438570 (__fastcall),
//   CDisplayManager::GetRenderTargetDimensions 0x009BEDC0 (display @ 0x013B8390), CFontBank::GetStringWidth 0x0048DA62
//   (result unused), the bank's virtual slots +0x5c / +0x60, CBaseIntelligentPointer::GetPItem 0x00A01B50, CParticleEmitter
//   Kill 0x00AEE8C0 / Set2DPosition 0x00AED090, CIntelligentPointer<T>(T*) 0x00655318 / operator= 0x0065533C,
//   ~CBaseIntelligentPointer 0x00A01C10, CRightHandedSet(up, forward) 0x00487050, CParticleEmitterManager::Get 0x00AEAAF0 /
//   Create2DEmitter 0x00AEB790. Float 0.5f 0x0122F59C, 2^32 0x0122DCB4.
// Layout: as the landed ClearGraphics 0x00645F8D (PrimitiveHandles +0x30, Effects +0x3c) and DrawMoney: def pointer +4
//   (its effect offset C2DVector at +0x528), last effect +0x5c; CPlayerGuiDef (retail) +0x2c8 float, +0x2cc / +0x2e4 graphic
//   indices, +0x2d0 / +0x2d8 C2DVector offsets; the game UI's font bank at +0x138.
#pragma optimize("y", off)
#pragma optimize("s", on)
class C2DVector { public:
    float X, Y;
    C2DVector() {}
    C2DVector(float x, float y) : X(x), Y(y) {}
    C2DVector& operator+=(const C2DVector& o) { X += o.X; Y += o.Y; return *this; }
};
inline C2DVector operator+(const C2DVector& a, const C2DVector& b) { return C2DVector(a.X + b.X, a.Y + b.Y); }
class C3DVector { public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};
class CRightHandedSet { public:
    C3DVector Up, Forward;
    CRightHandedSet(const C3DVector& up, const C3DVector& forward);   // 0x00487050
};
class CWideString;
class CEnginePrimitivePrepareInfo;
class CEnginePrimitiveHandle { public: unsigned long m_handle; };
class C2DExtentsI { public: long Width, Height; };
class CDisplayManager { public: C2DExtentsI GetRenderTargetDimensions(); };   // 0x009BEDC0
extern CDisplayManager* g_pDisplayManager_013b8390;                    // 0x013B8390
class CFontBank { public: float GetStringWidth(const CWideString& text); };    // 0x0048DA62
struct CGameUI { unsigned char _pad[0x138]; CFontBank* Font; };        // +0x138
extern CGameUI* g_pGameUI;                                              // 0x013B8790
class CPlayerGuiDef { public:
    unsigned char _pad[0x2c8];
    float     StatBoxTextWidth;          // +0x2c8
    long      StatBoxTextGraphic;        // +0x2cc
    C2DVector StatBoxOffset;             // +0x2d0
    C2DVector StatBoxTextOffset;         // +0x2d8
    long      StatBoxEdgeGraphic;        // +0x2e0 (unused here)
    long      StatBoxTextEdgeGraphic;    // +0x2e4
};
extern CPlayerGuiDef* __fastcall PeekGuiDef();                          // 0x00438570
class CGraphicDataBank { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22();
    virtual unsigned long GetGraphicWidth(long graphic) const;          // slot 23 (+0x5c)
    virtual unsigned long GetGraphicHeight(long graphic) const;         // slot 24 (+0x60)
};
enum EEngine2DLayer { ENGINE_2D_LAYER_STAT_BOX = 0x44 };
namespace NParticleEngine {
class CParticleEmitter { public:
    void Kill();                                                        // 0x00AEE8C0
    void Set2DPosition(const C2DVector* position, EEngine2DLayer layer);   // 0x00AED090
};
class CParticleEmitterManager { public:
    static CParticleEmitterManager* Get();                              // 0x00AEAAF0
    CParticleEmitter* Create2DEmitter(long effect, const C2DVector* position, EEngine2DLayer layer, bool start,
                                      const CRightHandedSet& axes);     // 0x00AEB790
};
}
class CBaseObject;
struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;
    CBaseObject* GetPItem() const;                                      // 0x00A01B50
    ~CBaseIntelligentPointer();                                         // 0x00A01C10
};
template<class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    CIntelligentPointer(T* item);                                       // 0x00655318
    CIntelligentPointer& operator=(const CIntelligentPointer& other);  // 0x0065533C
};
template<class T> struct CRetailVector {
    T* m_pFirst; T* m_pLast; T* m_pEnd;
    T* begin() const { return m_pFirst; }
};
struct CStatBarDef { unsigned char _pad[0x528]; C2DVector EffectOffset; };   // +0x528
namespace NPlayerGui {
class CStatBar { public:
    virtual void v00();
    CStatBarDef* Def;                                                   // +0x04
    unsigned char _pad08[0x28];
    CRetailVector<CEnginePrimitiveHandle> PrimitiveHandles;             // +0x30
    CRetailVector<CIntelligentPointer<NParticleEngine::CParticleEmitter> > Effects;   // +0x3c
    unsigned char _pad48[0x14];
    long LastEffect;                                                    // +0x5c
    static void __fastcall DrawStatBox(const CEnginePrimitivePrepareInfo& info, CRetailVector<CEnginePrimitiveHandle>& handles,
        const C2DVector& pos, long graphic, const CWideString& text, long value, unsigned char alpha, bool withText, bool flag,
        bool extra);                                                    // 0x00645853
    void DrawStatBox(const CEnginePrimitivePrepareInfo& info, const CGraphicDataBank& bank, const C2DVector& pos, long graphic,
        long value, long effect, const CWideString& text, unsigned char alpha, bool withText, bool flag);
};

void CStatBar::DrawStatBox(const CEnginePrimitivePrepareInfo& info, const CGraphicDataBank& bank, const C2DVector& pos,
    long graphic, long value, long effect, const CWideString& text, unsigned char alpha, bool withText, bool flag)
{
    DrawStatBox(info, PrimitiveHandles, pos, graphic, text, value, alpha, withText, flag, false);
    C2DVector textPos(0.0f, 0.0f);
    if (graphic > 0) {
        if (withText) {
            CPlayerGuiDef* def = PeekGuiDef();
            CFontBank* font = g_pGameUI->Font;
            C2DVector corner = pos;
            corner.X = (float)g_pDisplayManager_013b8390->GetRenderTargetDimensions().Width - corner.X;
            corner += def->StatBoxOffset;
            float boxWidth = (float)bank.GetGraphicWidth(graphic);
            float textWidth = def->StatBoxTextWidth;
            font->GetStringWidth(text);
            bank.GetGraphicHeight(def->StatBoxTextGraphic);
            float edge = (float)bank.GetGraphicWidth(def->StatBoxTextEdgeGraphic);
            textPos = C2DVector((textWidth - edge) + (corner.X + boxWidth * 0.5f), corner.Y) + def->StatBoxTextOffset;
        } else {
            textPos = pos;
        }
    }
    if (!effect || effect != LastEffect) {
        if ((*Effects.begin()).GetPItem()) {
            ((NParticleEngine::CParticleEmitter*)(*Effects.begin()).GetPItem())->Kill();
            CIntelligentPointer<NParticleEngine::CParticleEmitter> none(0);
            *Effects.begin() = none;
        }
    }
    if (effect > 0) {
        if (!(*Effects.begin()).GetPItem()) {
            CIntelligentPointer<NParticleEngine::CParticleEmitter> emitter(
                NParticleEngine::CParticleEmitterManager::Get()->Create2DEmitter(effect, &(textPos + Def->EffectOffset),
                    ENGINE_2D_LAYER_STAT_BOX, true, CRightHandedSet(C3DVector(0.0f, 0.0f, 1.0f), C3DVector(0.0f, 1.0f, 0.0f))));
            *Effects.begin() = emitter;
        }
        if ((*Effects.begin()).GetPItem())
            ((NParticleEngine::CParticleEmitter*)(*Effects.begin()).GetPItem())->Set2DPosition(&(textPos + Def->EffectOffset),
                ENGINE_2D_LAYER_STAT_BOX);
    }
    LastEffect = effect;
}
}

// ---- test stubs ----
CRightHandedSet::CRightHandedSet(const C3DVector& u, const C3DVector& f) : Up(u), Forward(f) {}
C2DExtentsI CDisplayManager::GetRenderTargetDimensions() { C2DExtentsI e; e.Width = 640; e.Height = 480; return e; }
static CDisplayManager g_dm;
CDisplayManager* g_pDisplayManager_013b8390 = &g_dm;
float CFontBank::GetStringWidth(const CWideString&) { return 0.0f; }
static CGameUI g_ui;
CGameUI* g_pGameUI = &g_ui;
static CPlayerGuiDef g_def;
CPlayerGuiDef* __fastcall PeekGuiDef() { return &g_def; }
void CGraphicDataBank::v00() {} void CGraphicDataBank::v01() {} void CGraphicDataBank::v02() {} void CGraphicDataBank::v03() {}
void CGraphicDataBank::v04() {} void CGraphicDataBank::v05() {} void CGraphicDataBank::v06() {} void CGraphicDataBank::v07() {}
void CGraphicDataBank::v08() {} void CGraphicDataBank::v09() {} void CGraphicDataBank::v10() {} void CGraphicDataBank::v11() {}
void CGraphicDataBank::v12() {} void CGraphicDataBank::v13() {} void CGraphicDataBank::v14() {} void CGraphicDataBank::v15() {}
void CGraphicDataBank::v16() {} void CGraphicDataBank::v17() {} void CGraphicDataBank::v18() {} void CGraphicDataBank::v19() {}
void CGraphicDataBank::v20() {} void CGraphicDataBank::v21() {} void CGraphicDataBank::v22() {}
unsigned long CGraphicDataBank::GetGraphicWidth(long g) const { return (unsigned long)(g * 2); }
unsigned long CGraphicDataBank::GetGraphicHeight(long g) const { return (unsigned long)g; }
static int g_kills, g_creates, g_sets; static C2DVector g_createPos, g_setPos;
static NParticleEngine::CParticleEmitter g_particleSink;
void NParticleEngine::CParticleEmitter::Kill() { ++g_kills; }
void NParticleEngine::CParticleEmitter::Set2DPosition(const C2DVector* p, EEngine2DLayer) { ++g_sets; g_setPos = *p; }
static NParticleEngine::CParticleEmitterManager g_mgr;
NParticleEngine::CParticleEmitterManager* NParticleEngine::CParticleEmitterManager::Get() { return &g_mgr; }
NParticleEngine::CParticleEmitter* NParticleEngine::CParticleEmitterManager::Create2DEmitter(long, const C2DVector* p, EEngine2DLayer, bool, const CRightHandedSet&) { ++g_creates; g_createPos = *p; return &g_particleSink; }
void CBaseIntelligentPointer::v00() {}
CBaseObject* CBaseIntelligentPointer::GetPItem() const { return m_pItem; }
CBaseIntelligentPointer::~CBaseIntelligentPointer() {}
template<class T> CIntelligentPointer<T>::CIntelligentPointer(T* item) { m_pItem = (CBaseObject*)item; }
template<class T> CIntelligentPointer<T>& CIntelligentPointer<T>::operator=(const CIntelligentPointer& o) { m_pItem = o.m_pItem; return *this; }
static int g_inner;
void __fastcall NPlayerGui::CStatBar::DrawStatBox(const CEnginePrimitivePrepareInfo&, CRetailVector<CEnginePrimitiveHandle>&, const C2DVector&, long, const CWideString&, long, unsigned char, bool, bool, bool) { ++g_inner; }
void NPlayerGui::CStatBar::v00() {}
static bool near(float a, float b) { float d = a - b; return d < 0.001f && d > -0.001f; }
int main()
{
    int ok = 1;
    g_def.StatBoxTextWidth = 30.0f; g_def.StatBoxTextGraphic = 7; g_def.StatBoxTextEdgeGraphic = 4;
    g_def.StatBoxOffset = C2DVector(1.0f, 2.0f); g_def.StatBoxTextOffset = C2DVector(3.0f, 4.0f);
    static CGraphicDataBank bank;
    static CStatBarDef sdef; sdef.EffectOffset = C2DVector(100.0f, 200.0f);
    static NPlayerGui::CStatBar bar; memset((char*)&bar + sizeof(void*), 0, sizeof(bar) - sizeof(void*));
    static CIntelligentPointer<NParticleEngine::CParticleEmitter>* slot;
    static unsigned char slotRaw[sizeof(CIntelligentPointer<NParticleEngine::CParticleEmitter>)];
    slot = (CIntelligentPointer<NParticleEngine::CParticleEmitter>*)slotRaw;
    bar.Def = &sdef; bar.Effects.m_pFirst = slot; bar.LastEffect = 0;
    int dummy = 0;
    const CEnginePrimitivePrepareInfo& info = *(const CEnginePrimitivePrepareInfo*)&dummy;
    const CWideString& text = *(const CWideString*)&dummy;
    C2DVector pos(10.0f, 20.0f);
    bar.DrawStatBox(info, bank, pos, 5, 1, 9, text, 255, true, false);
    // textPos = ((30 - 8) + ((640 - 10 + 1) + 10 * 0.5), 20 + 2) + (3, 4) = (661, 26); + effect offset = (761, 226)
    if (g_inner != 1 || g_creates != 1 || g_sets != 1 || !near(g_createPos.X, 761.0f) || !near(g_createPos.Y, 226.0f)) ok = 0;
    if (!near(g_setPos.X, 761.0f) || !near(g_setPos.Y, 226.0f) || bar.LastEffect != 9) ok = 0;
    // same effect again, no text: position from pos, no new emitter
    bar.DrawStatBox(info, bank, pos, 5, 1, 9, text, 255, false, false);
    if (g_creates != 1 || g_kills != 0 || g_sets != 2 || !near(g_setPos.X, 110.0f) || !near(g_setPos.Y, 220.0f)) ok = 0;
    // a different effect kills the old one and creates a new one
    bar.DrawStatBox(info, bank, pos, 0, 1, 3, text, 255, true, false);
    if (g_kills != 1 || g_creates != 2 || !near(g_createPos.X, 100.0f) || bar.LastEffect != 3) ok = 0;
    if (ok) printf("00645ce9_TEST PASS\n"); else printf("fail k%d c%d s%d %f %f\n", g_kills, g_creates, g_sets, g_createPos.X, g_createPos.Y);
    return ok ? 0 : 1;
}