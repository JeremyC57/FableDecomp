// Grade: DIFFER(387v393) 27 of 139 lines (was 139): rebuilt on the matched sibling 0x00645099 models -- PDB ?Draw@CDrawMeleeActionButtonEffects@NPlayerGui@@QAEXABVCEnginePrimitivePrepareInfo@@@Z, Ego_r layout (CDrawBase Drawing +0x18 / PrimitiveHandles +0x1c, Effects vector<CIntelligentPointer<CParticleEmitter> > +0x28, FlashTimer +0x34, ComboLoadingIconTimer +0x38, FlourishEffect +0x40), PDB Create2DEmitter(long, const C2DVector&, EEngine2DLayer, bool, const CRightHandedSet&) with CRightHandedSet(C3DVector(0,0,1), C3DVector(0,1,0)) (0x00487050), the 16-argument sprite ctor with ScaleAlpha(0xff) (0x00643D5D), the emitter as a named intelligent-pointer local assigned to Effects[0]; residue: the second screen position should share [ebp-8] with the emitter temporary and the sprite position sum stays on the FPU until the arguments are built (ours stores it at once; a named sum + C2DVector copy gets 25 lines but 399 bytes)
// NPlayerGui::CDrawMeleeActionButtonEffects::Draw(const CEnginePrimitivePrepareInfo& info) -- melee flourish effect: while a flash or combo-loading timer runs and a flourish effect is pending, create the 2D particle emitter once and draw the effect sprite at the GUI-def emitter position; a hidden GUI zeroes the effect, and a zero effect clears the emitters
// Leaf: Draw
#pragma optimize("y", off)
#pragma optimize("s", on)
class C2DVector { public:
    float X, Y;
    C2DVector() { }
    C2DVector(float x, float y) : X(x), Y(y) { }
};
inline C2DVector operator+(const C2DVector& a, const C2DVector& b) { return C2DVector(a.X + b.X, a.Y + b.Y); }
class C3DVector { public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};
class CRightHandedSet { public:
    unsigned char _body[0x18];
    CRightHandedSet(const C3DVector& forward, const C3DVector& up);   // 0x00487050
};
void __fastcall GFAPositionToScreen(C2DVector* out, const C2DVector* in);   // 0x00643D0C
class CPlayerGuiDef { public:
    unsigned char _pad000[0x788];
    C2DVector     MeleeEffectsEmitterPos;                  // +0x788 (name descriptive)
    unsigned long MeleeEffectsGraphic;                     // +0x790 (name descriptive)
    C2DVector     MeleeEffectsSpriteOffset;                // +0x794 (name descriptive)
};
class CPlayerGui { public:                                 // retail offsets (engine/CPlayerGui.h)
    static const CPlayerGuiDef& __fastcall PeekGuiDef();     // 0x00438570
    unsigned char _pad000[0x250];
    bool          ShowSpecialAbilityButtons;               // +0x250
    unsigned char _pad251[0x2a0 - 0x251];
    float         Alpha;                                   // +0x2a0
};
class CEnginePrimitiveHandle { public: unsigned long Data[2]; };
enum EEngine2DLayer { ENGINE_2D_LAYER_DUMMY = 0 };
enum EEnginePrimitiveBlendMode { ENGINE_PRIMITIVE_BLEND_MODE_2 = 2 };
enum EEngineInterpolationSet { ENGINE_INTERPOLATION_SET_0 = 0 };
class CEnginePrimitive2DSprite { public:
    unsigned char _body[0x54];
    CEnginePrimitive2DSprite(unsigned __int64 id, const C2DVector& pos, EEngine2DLayer layer, unsigned long graphic, unsigned long frame,
                             EEnginePrimitiveBlendMode blend, EEngineInterpolationSet interpolation, float scaleX, float scaleY,
                             unsigned char alpha, unsigned char red, unsigned char green, unsigned char blue, unsigned long flags,
                             const C2DVector& uv0, const C2DVector& uv1);   // 0x00493AC2
};
class CEnginePrimitiveRenderer { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22();
    virtual void AddPrimitive(CEnginePrimitiveHandle* handle, const CEnginePrimitive2DSprite& primitive, long flags);   // slot 23 (+0x5C)
};
class CEnginePrimitivePrepareInfo { public:
    unsigned char             _pad00[0x34];
    CEnginePrimitiveRenderer* Renderer;                    // +0x34
};
struct CPrimitiveHandleVector {                            // STLport vector<CEnginePrimitiveHandle> (12 bytes)
    CEnginePrimitiveHandle* _M_start;
    CEnginePrimitiveHandle* _M_finish;
    CEnginePrimitiveHandle* _M_end_of_storage;
    CEnginePrimitiveHandle& operator[](unsigned long n) { return _M_start[n]; }
};

namespace NParticleEngine {
class CParticleEmitter;
class CParticleEmitterManager { public:
    static CParticleEmitterManager* Get();                 // 0x00AEAAF0
    CParticleEmitter* Create2DEmitter(long effect, const C2DVector& position, EEngine2DLayer layer, bool flag,
                                      const CRightHandedSet& orientation);   // 0x00AEB790
};
}
class CBaseObject;
class CBaseIntelligentPointer { public:
    virtual ~CBaseIntelligentPointer();                    // 0x00A01C10
    void* PData;
    CBaseObject* GetPItem() const;                         // 0x00A01B50
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    CIntelligentPointer(T* item);                          // 0x00655318 (CParticleEmitter)
    CIntelligentPointer& operator=(const CIntelligentPointer& other);   // 0x0065533C
};
struct CEffectVector {                                     // STLport vector<CIntelligentPointer<CParticleEmitter> >
    CIntelligentPointer<NParticleEngine::CParticleEmitter>* _M_start;
    CIntelligentPointer<NParticleEngine::CParticleEmitter>* _M_finish;
    CIntelligentPointer<NParticleEngine::CParticleEmitter>* _M_end_of_storage;
    CIntelligentPointer<NParticleEngine::CParticleEmitter>& operator[](unsigned long n) { return _M_start[n]; }
    void clear();                                          // 0x006445BA (out of line)
};

namespace NPlayerGui {

class CDrawBase { public:
    void*                  _vtable;                        // +0x00
    CPlayerGui*            PlayerGui;                      // +0x04
    unsigned char          _pad08[0x18 - 0x08];
    bool                   Drawing;                        // +0x18
    CPrimitiveHandleVector PrimitiveHandles;               // +0x1c
    unsigned char ScaleAlpha(unsigned char alpha) const;   // 0x00643D5D
};

class CDrawMeleeActionButtonEffects : public CDrawBase { public:
    CEffectVector Effects;                                 // +0x28
    long          FlashTimer;                              // +0x34
    long          ComboLoadingIconTimer;                   // +0x38
    long          FlourishCounter;                         // +0x3c
    long          FlourishEffect;                          // +0x40
    void ClearEffects() { Effects.clear(); }
    void Draw(const CEnginePrimitivePrepareInfo& info);
};

void CDrawMeleeActionButtonEffects::Draw(const CEnginePrimitivePrepareInfo& info)
{
    if (!PlayerGui->ShowSpecialAbilityButtons) {
        if ((FlashTimer >= 0 || ComboLoadingIconTimer >= 0) && FlourishEffect > 0) {
            Drawing = true;
            const CPlayerGuiDef& def = CPlayerGui::PeekGuiDef();
            if (FlashTimer >= 0 && !Effects[0].GetPItem()) {
                C2DVector position;
                GFAPositionToScreen(&position, &def.MeleeEffectsEmitterPos);
                CIntelligentPointer<NParticleEngine::CParticleEmitter> emitter(
                    NParticleEngine::CParticleEmitterManager::Get()->Create2DEmitter(FlourishEffect, position, (EEngine2DLayer)0x44, true,
                        CRightHandedSet(C3DVector(0.0f, 0.0f, 1.0f), C3DVector(0.0f, 1.0f, 0.0f))));
                Effects[0] = emitter;
            }
            if (Effects[0].GetPItem()) {
                C2DVector position;
                GFAPositionToScreen(&position, &def.MeleeEffectsEmitterPos);
                CEnginePrimitive2DSprite sprite(0, position + def.MeleeEffectsSpriteOffset, (EEngine2DLayer)0x43, def.MeleeEffectsGraphic, 0,
                                                ENGINE_PRIMITIVE_BLEND_MODE_2, ENGINE_INTERPOLATION_SET_0, 0.0f, 0.0f, ScaleAlpha(0xff),
                                                0xff, 0xff, 0xff, 0, C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
                info.Renderer->AddPrimitive(&PrimitiveHandles[0], sprite, 0xc0);
            }
        }
    } else
        FlourishEffect = 0;
    if (FlourishEffect == 0)
        ClearEffects();
}

}
