// NPlayerGui::CDrawQuickAccessIcons::DrawChargeGraphic(const CEnginePrimitivePrepareInfo& prepareInfo, const C2DVector& position, long button) -- draw the d-pad ring charge-up graphic: the charge is 1 when fully charged, else the charging time over the button's activation time; when it is above zero (clamped to 1) a clock sprite of the charge graphic for this side sweeps half a turn from the side's start angle at the anchored ring position, black-to-white with the GUI fade, set as the persistent charge-up primitive
// Grade: DIFFER(371v388) -- register allocation and float-merge shape: retail zeroes ebx once up front and uses it for every 0 (the FullyCharged test, the zero pushes, the black colour bytes), spills `this` to [ebp-8] and the side index to [ebp-0Ch], homes the 0.0 charge at [ebp-4] and reloads it on both failed activation tests (no jump threading to the exit), and stores the white colour bytes before the first faded-alpha call; ours keeps the side index in ebx, threads the zero path straight to the exit and pushes literal zeros. Callees, arguments (all 17 of the clock-sprite constructor) and the persistent-primitive call are retail's. Tried: public member (wrong leaf), private (this). Two grades left unused: no single change covers the four residues.
// Leaf: ?DrawChargeGraphic@CDrawQuickAccessIcons@NPlayerGui@@AAEXABVCEnginePrimitivePrepareInfo@@ABVC2DVector@@J@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0064D7A6, 388 bytes, thiscall, ebp frame (`leave`), `ret 0Ch`.
// Identity (proven): Ego_r.pdb / FableWin.pdb ?DrawChargeGraphic@CDrawQuickAccessIcons@NPlayerGui@@AAEXABVCEnginePrimitivePrepareInfo@@
//   ABVC2DVector@@J@Z (three arguments, `ret 0Ch`); the body reads CDrawQuickAccessIcons::ChargingButtonIndex / ChargingButtonTime /
//   FullyCharged (+0x2C / +0x30 / +0x35) and sets ChargeUpPrimitiveHandle (+0x5C), Ego_r.pdb members 4 below the donor.
// Callees: CPlayerGui::PeekGuiDef 0x00438570, CDrawQuickAccessIcons::GetActivationTimeForButton(long) 0x0064B5C1,
//   NPlayerGui::GFAnchorToBottomLeft(const C2DVector&) 0x00643CDA (CPlayerGuiDef::DPadRingsPos +0x174), the faded-alpha helper
//   0x00643D5D (twice), CEnginePrimitive2DClockSprite::CEnginePrimitive2DClockSprite 0x0065595C (Ego_r.pdb ??0CEnginePrimitive2DClockSprite
//   @@QAE@_KABVC2DVector@@W4EEngine2DLayer@@KKABVCRGBColour@@KK3W4EEnginePrimitiveBlendMode@@MM11KW4EEngineInterpolationSet@@@Z: the
//   17 pushes match it argument for argument), CIEngine::SetPersistentPrimitive (slot 23, +0x5C, on CEnginePrimitivePrepareInfo::Engine
//   +0x34). CPlayerGuiDef::DPadRingsChargeGraphics / DPadRingsChargeOffsets / DPadRingsChargeAngles (+0x18C / +0x198 / +0x1A4).
// Names: 0x00643CDA is the third of the four PDB GFAnchorTo* helpers in address order (TopLeft, TopRight 0x00643CA1, BottomLeft,
//   BottomRight 0x00643D0C, the donor order) and adds the render-target height to Y only -> GFAnchorToBottomLeft (probable);
//   0x00643D5D (PlayerGui's GUI fade at +0x2A0 times the alpha, a CDrawBase member) has no PDB name -> descriptive
//   GetFadedAlpha_00643d5d; CIEngine slot 23 is SetPersistentPrimitive(CEnginePrimitiveHandle&, const CEnginePrimitive&, unsigned
//   long) (the CEngine / CEditEngine2DBase overrides; probable).
// Label correction: catalogued as "CDrawQuickAccessIcons::DrawChargeGraphic" without the NPlayerGui namespace.
// Layout: local view (no engine/ header for CDrawQuickAccessIcons, CDrawBase, CPlayerGuiDef or CEnginePrimitivePrepareInfo).
#pragma optimize("y", off)
#pragma optimize("s", on)
class C2DVector {
public:
    float X;
    float Y;
    C2DVector() { }
    C2DVector(float x, float y) : X(x), Y(y) { }
    C2DVector operator+(const C2DVector& other) const { return C2DVector(X + other.X, Y + other.Y); }
};
class CRGBColour {
public:
    unsigned char B;
    unsigned char G;
    unsigned char R;
    unsigned char A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
namespace std {
template <class _Tp> class allocator { };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    typedef unsigned int size_type;
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    const _Tp& operator[](size_type __n) const { return *(_M_start + __n); }
};
}
class CPlayerGuiDef {
public:
    unsigned char _pad_0x000[0x174];
    C2DVector               DPadRingsPos;                   // +0x174
    long                    DPadRingsTopGraphic;            // +0x17C
    C2DVector               DPadRingsTopOffset;             // +0x180
    long                    DPadRingsExtendGraphic;         // +0x188
    std::vector<long>       DPadRingsChargeGraphics;        // +0x18C
    std::vector<C2DVector>  DPadRingsChargeOffsets;         // +0x198
    std::vector<float>      DPadRingsChargeAngles;          // +0x1A4
};
class CPlayerGui {
public:
    static const CPlayerGuiDef& __fastcall PeekGuiDef();     // 0x00438570
};
class CEnginePrimitive {                                    // 0x0C bytes
public:
    unsigned long Type;
    unsigned long Data[2];
};
enum EEngine2DLayer { };
enum EEnginePrimitiveBlendMode { };
enum EEngineInterpolationSet { };
class CEnginePrimitive2DClockSprite : public CEnginePrimitive {   // 0x60 bytes
public:
    CEnginePrimitive2DClockSprite(unsigned __int64 id, const C2DVector& position, EEngine2DLayer layer, unsigned long graphic,
                                  unsigned long frame, const CRGBColour& colour, unsigned long graphic2, unsigned long frame2,
                                  const CRGBColour& colour2, EEnginePrimitiveBlendMode blendMode, float angle, float angle2,
                                  const C2DVector& scale, const C2DVector& scale2, unsigned long flags,
                                  EEngineInterpolationSet interpolationSet);   // 0x0065595C
    unsigned char _body[0x60 - 0x0c];
};
class CEnginePrimitiveHandle {
public:
    void* _vfptr;
    void* Data;
};
class CIEngine {
public:
    virtual void Slot00(); virtual void Slot01(); virtual void Slot02(); virtual void Slot03(); virtual void Slot04();
    virtual void Slot05(); virtual void Slot06(); virtual void Slot07(); virtual void Slot08(); virtual void Slot09();
    virtual void Slot10(); virtual void Slot11(); virtual void Slot12(); virtual void Slot13(); virtual void Slot14();
    virtual void Slot15(); virtual void Slot16(); virtual void Slot17(); virtual void Slot18(); virtual void Slot19();
    virtual void Slot20(); virtual void Slot21(); virtual void Slot22();
    virtual void SetPersistentPrimitive(CEnginePrimitiveHandle& handle, const CEnginePrimitive& primitive,
                                        unsigned long flags);   // slot 23 (+0x5C)
};
class CEnginePrimitivePrepareInfo {
public:
    unsigned char _pad_0x00[0x34];
    CIEngine*     Engine;                                   // +0x34
};
class CPlayerManager;
class CWorld;
class CMainGameComponent;
class CGraphicDataBank;
namespace NPlayerGui {
C2DVector __fastcall GFAnchorToBottomLeft(const C2DVector& position);   // 0x00643CDA
class CDrawBase {
public:
    virtual ~CDrawBase();
    unsigned char GetFadedAlpha_00643d5d(unsigned char alpha);   // 0x00643D5D
    CPlayerGui*         PlayerGui;                          // +0x04
    CPlayerManager*     PlayerManager;                      // +0x08
    CWorld*             World;                              // +0x0C
    CMainGameComponent* Component;                          // +0x10
    CGraphicDataBank*   Bank;                               // +0x14
    bool                Drawing;                            // +0x18
    std::vector<CEnginePrimitiveHandle> PrimitiveHandles;   // +0x1C
};
class CDrawQuickAccessIcons : public CDrawBase {
private:
    float GetActivationTimeForButton(long button);          // 0x0064B5C1
    void DrawChargeGraphic(const CEnginePrimitivePrepareInfo& prepareInfo, const C2DVector& position, long button);
public:
    unsigned long          ChargingSoundID;                 // +0x28
    long                   ChargingButtonIndex;             // +0x2C
    float                  ChargingButtonTime;              // +0x30
    bool                   Charging;                        // +0x34
    bool                   FullyCharged;                    // +0x35
    float                  FullyChargedTime;                // +0x38
    long                   FlashButton;                     // +0x3C
    unsigned char          FlashFade[0x5c - 0x40];          // +0x40 CFadeInFadeOutBase
    CEnginePrimitiveHandle ChargeUpPrimitiveHandle;         // +0x5C
};

void CDrawQuickAccessIcons::DrawChargeGraphic(const CEnginePrimitivePrepareInfo& prepareInfo, const C2DVector& position, long button)
{
    const CPlayerGuiDef& def = CPlayerGui::PeekGuiDef();
    float charge = 0.0f;
    if (FullyCharged)
        charge = 1.0f;
    else {
        float activationTime = GetActivationTimeForButton(ChargingButtonIndex);
        if (activationTime > 0.0001f && activationTime > ChargingButtonTime)
            charge = ChargingButtonTime / activationTime;
    }
    if (charge > 0.0001f) {
        long side = (button != -2);
        if (charge > 1.0f)
            charge = 1.0f;
        float angle = charge * 0.5f + def.DPadRingsChargeAngles[side];
        C2DVector pos = GFAnchorToBottomLeft(def.DPadRingsPos) + position + def.DPadRingsChargeOffsets[side];
        CEnginePrimitive2DClockSprite sprite(0, pos, (EEngine2DLayer)0x44, def.DPadRingsChargeGraphics[side], 0,
                                             CRGBColour(0, 0, 0, GetFadedAlpha_00643d5d(255)),
                                             def.DPadRingsChargeGraphics[side], 0,
                                             CRGBColour(255, 255, 255, GetFadedAlpha_00643d5d(255)),
                                             (EEnginePrimitiveBlendMode)2, angle, angle, C2DVector(0.0f, 0.0f),
                                             C2DVector(0.0f, 0.0f), 2, (EEngineInterpolationSet)0);
        prepareInfo.Engine->SetPersistentPrimitive(ChargeUpPrimitiveHandle, sprite, 0xc0);
    }
}
}   // namespace NPlayerGui
