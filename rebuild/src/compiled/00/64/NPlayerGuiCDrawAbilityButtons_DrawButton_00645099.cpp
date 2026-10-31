// NPlayerGui::CDrawAbilityButtons::DrawButton(const CEnginePrimitivePrepareInfo& info, const C2DVector& position, float alpha, long index, long graphic, bool special, EEngine2DLayer layer) -- draw one ability button sprite into the primary or the special handle table: a negative alpha draws unscaled (fixed 160 buttonAlpha byte), the GUI alpha byte is forced to 255 while the special buttons show, and the sprite's alpha is the GUI-scaled buttonAlpha byte
// Grade: RELOCATION_MATCH (324v324)
// Leaf: ?DrawButton@CDrawAbilityButtons@NPlayerGui@@IAEXABVCEnginePrimitivePrepareInfo@@ABVC2DVector@@MJJ_NW4EEngine2DLayer@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00645099, length 324 bytes (frame pointer), `ret 0x1c`.
// Name: Ego_r.pdb ?DrawButton@CDrawAbilityButtons@NPlayerGui@@IAEXABVCEnginePrimitivePrepareInfo@@ABVC2DVector@@MJJ_NW4EEngine2DLayer@@@Z
//   (protected; 7 parameters = retail `ret 1Ch`; [ebp+1Ch] is tested as the bool, [ebp+20h] pushed as the sprite layer).
// Callees: CPlayerGui::PeekGuiDef @ 0x00438570 (static `__fastcall`, result unused), __ftol2 @ 0x00BFEA70,
//   CDrawBase::ScaleAlpha(unsigned char) const @ 0x00643D5D (Ego_r ?ScaleAlpha@CDrawBase@NPlayerGui@@QBEEE@Z; body: byte * PlayerGui->Alpha
//   through __ftol2, `ret 4`; landed under the propagated GetFadedAlpha spellings -> NAME_FIXES), the 2D sprite constructor @ 0x00493AC2
//   (Ego_r ??0CEnginePrimitive2DSprite@@QAE@_KABVC2DVector@@W4EEngine2DLayer@@KKW4EEnginePrimitiveBlendMode@@W4EEngineInterpolationSet@@MMEEEEK11@Z),
//   the renderer's virtual slot 23 (+0x5C, AddPrimitive). Float pool: 0.0f @ 0x0122DEDC, 255.0f @ 0x01230014, 160.0f @ 0x0125A29C.
// Layout (retail offsets): CDrawBase PlayerGui +4, PrimitiveHandles +0x1c (STLport vector, 8-byte CEnginePrimitiveHandle);
//   CDrawAbilityButtons SpecialPrimitiveHandles +0x3c; CPlayerGui ShowSpecialAbilityButtons +0x250, Alpha +0x2a0 (engine/CPlayerGui.h:
//   PDB +0x258 / +0x2b4, the STLport vectors are 4 bytes smaller); CEnginePrimitivePrepareInfo Renderer +0x34.
// Shape: the sprite position is a by-reference C2DVector temporary copied from `position` while the arguments are evaluated;
//   the two uv vectors are temporaries; `(unsigned char)(long)buttonAlpha` feeds ScaleAlpha.
#pragma optimize("y", off)
#pragma optimize("s", on)
class C2DVector { public:
    float X, Y;
    C2DVector() { }
    C2DVector(float x, float y) : X(x), Y(y) { }
};
class CPlayerGuiDef { public: unsigned char _pad[4]; };
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

namespace NPlayerGui {

class CDrawBase { public:
    void*                  _vtable;                        // +0x00
    CPlayerGui*            PlayerGui;                      // +0x04
    unsigned char          _pad08[0x1c - 0x08];
    CPrimitiveHandleVector PrimitiveHandles;               // +0x1c
    unsigned char ScaleAlpha(unsigned char alpha) const;   // 0x00643D5D
};

class CDrawAbilityButtons : public CDrawBase { public:
    long                   Columns;                        // +0x28
    CEnginePrimitiveHandle BackgroundPrimitiveHandle;      // +0x2c
    CEnginePrimitiveHandle InteractBorderPrimitiveHandle;  // +0x34
    CPrimitiveHandleVector SpecialPrimitiveHandles;        // +0x3c
protected:
    void DrawButton(const CEnginePrimitivePrepareInfo& info, const C2DVector& position, float alpha, long index, long graphic,
                    bool special, EEngine2DLayer layer);
};

void CDrawAbilityButtons::DrawButton(const CEnginePrimitivePrepareInfo& info, const C2DVector& position, float alpha, long index,
                                     long graphic, bool special, EEngine2DLayer layer)
{
    CPlayerGui::PeekGuiDef();
    CPrimitiveHandleVector* handles = special ? &SpecialPrimitiveHandles : &PrimitiveHandles;
    bool scaled = true;
    if (alpha < 0.0f) {
        scaled = false;
        alpha = -alpha;
    }
    unsigned char guiAlpha = (unsigned char)(long)(PlayerGui->Alpha * 255.0f);
    if (PlayerGui->ShowSpecialAbilityButtons && guiAlpha < 255)
        guiAlpha = 255;
    if (graphic > 0 && alpha > 0.0f) {
        float buttonAlpha = alpha * (float)guiAlpha;
        if (!scaled)
            buttonAlpha = 160.0f;
        else if (PlayerGui->ShowSpecialAbilityButtons && guiAlpha < 255)
            buttonAlpha = special ? 255.0f : 0.0f;
        CEnginePrimitive2DSprite sprite(0, C2DVector(position), layer, graphic, 0, ENGINE_PRIMITIVE_BLEND_MODE_2, ENGINE_INTERPOLATION_SET_0,
                                        0.0f, 0.0f, ScaleAlpha((unsigned char)(long)buttonAlpha), 0xff, 0xff, 0xff, 0,
                                        C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
        info.Renderer->AddPrimitive(&(*handles)[index], sprite, 0xc0);
    }
}

}