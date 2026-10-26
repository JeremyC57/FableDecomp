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
class CPlayerGui {
public:
    static const CPlayerGuiDef& __fastcall PeekGuiDef();   // 0x00438570
};                          // 0x00438570
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
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    CIntelligentPointer(T* item);                                       // 0x00655318
    CIntelligentPointer& operator=(const CIntelligentPointer& other);  // 0x0065533C

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
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
            const CPlayerGuiDef* def = (&CPlayerGui::PeekGuiDef());
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