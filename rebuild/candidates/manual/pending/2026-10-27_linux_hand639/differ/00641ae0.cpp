// CTCLight::DrawFlare(long index, const C3DVector& position, CTCGraphicAppearance& appearance, const CGraphicAppearanceDrawInfo& info) -- make sure flare primitive slot `index` exists (grow the flare list and add a type-3 primitive to the appearance), then, unless overridden and when the light def names a halo graphic, give that primitive a flare sprite at the position with the appearance's fade distance as alpha
// Grade: DIFFER(331v331) -- only the scratch registers of the flare block differ: retail holds the halo graphic in edx, the thing in eax and the unique id low/high in ecx/eax, ours ecx/edx/eax/edx (same instructions and sizes)
// Leaf: ?DrawFlare@CTCLight@@IAEXJABVC3DVector@@AAVCTCGraphicAppearance@NGraphicAppearance@@ABVCGraphicAppearanceDrawInfo@4@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00641AE0, 331 bytes, `ret 10h`. Speed TU (esp frame, no pragma).
//   Ego_r.pdb: `?DrawFlare@CTCLight@@IAEXJABVC3DVector@@AAVCTCGraphicAppearance@NGraphicAppearance@@ABVCGraphicAppearanceDrawInfo@4@@Z`.
// Callees: CTCGraphicAppearance::GetFadeDistance @ 0x004BC180, vector<CPrimitive*>::resize(size_type, const T&)
//   @ 0x006426A0 (label "vector<unsigned int>::resize": byte-identical instantiation), CBaseClass() @ 0x0099A2D0
//   (label "NHeroInformationScreens::CBase::CBase" is propagated), CTCGraphicAppearance::AddPrimitive(const
//   CPrimitive&) @ 0x004C0050 (unnamed; forge-autoname "CArray_InsertAppearanceNode"), ~CPrimitive @ 0x004C4160
//   (manifest "CObject::~CObject", as the landed CTCGraphicAppearance rows), __ftol2 @ 0x00BFEA70,
//   CTCGraphicAppearance::SetPrimitiveGraphics(CPrimitive*, const CEnginePrimitive&, unsigned short) @ 0x004BD620.
// Shape: the vector start is read through a named pointer after the named CPrimitive(3) (retail caches the start, not the
//   element address, in the dead index slot); the flare sprite is built by an inline constructor in member order; the thing's 64-bit unique id is copied
//   whole (its high word lands in a dead frame slot) and narrowed to the 32-bit Identifier; the resize fill value
//   lives in the dead `index` argument slot, which then caches the vector start for the `[index] =` store.
// Layout: Ego_r.pdb CTCLight: PLightDef +0x18, Overridden +0x1D, FlareSprites +0x2C (retail 12-byte vector);
//   CLightDef HaloGraphic +0x2C; CThing UniqueID +0x18 (ulong64); CPrimitive 16 bytes (Type, CEnginePrimitiveHandle
//   {CBaseClass vptr -> 0x0122F598, Data}, BankIndex); CEnginePrimitiveFlareSprite 0x44 bytes (Ego_r.pdb).
#define CTCBase CTCBaseLayout
#include "engine/CTCBase.h"
#undef CTCBase
class CThing;
class CTCBase { public:                                     // engine/CTCBase.h flattens the vptr
    virtual ~CTCBase();
    CThing* ParentThing;                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
    CThing& GetParentThing() { return *ParentThing; }       // Ego_r.pdb ?GetParentThing@CTCBase@@QAEAAVCThing@@XZ
};

class C3DVector { public: float X, Y, Z; };
typedef unsigned __int64 ulong64;

class CThing { public:
    unsigned char _pad00[0x18];
    ulong64 UniqueID;                                       // +0x18
    ulong64 GetUniqueID() const { return UniqueID; }
};

#include "engine/CBaseClass.h"
struct CEnginePersistentPrimitiveBase;
class CEnginePrimitiveHandle : public CBaseClass {          // vtable 0x0122F598, 8 bytes
public:
    CEnginePersistentPrimitiveBase* Data;                   // +0x04
    CEnginePrimitiveHandle() : Data(0) { }
};

struct CEnginePrimitiveThemeChannelDesc {                   // 3 bytes
    unsigned char A, B, C;
    CEnginePrimitiveThemeChannelDesc() : A(0), B(0), C(0) { }
};
enum EEnginePrimitiveBlendMode { };
enum EEngineInterpolationSet { };
struct CEnginePrimitive {                                   // 12 bytes
    long Type;                                              // +0x00 EEnginePrimitiveType
    EEngineInterpolationSet InterpolationSet;               // +0x04
    CEnginePrimitiveThemeChannelDesc ThemeChannelDesc;      // +0x08
    CEnginePrimitive(long type, ulong64 id, EEngineInterpolationSet interp) : Type(type), InterpolationSet(interp) { }
};
struct CEnginePrimitiveFlareSprite : CEnginePrimitive {     // 0x44 bytes
    C3DVector     WorldPos;                                 // +0x0C
    unsigned long BankIndex;                                // +0x18
    unsigned long Frame;                                    // +0x1C
    float         LightSourceRadius;                        // +0x20
    float         SizeX;                                    // +0x24
    float         SizeY;                                    // +0x28
    float         MinScale;                                 // +0x2C
    float         FadeStart;                                // +0x30
    float         Angle;                                    // +0x34
    EEnginePrimitiveBlendMode BlendMode;                    // +0x38
    long          Identifier;                               // +0x3C
    unsigned char Alpha;                                    // +0x40
    __forceinline CEnginePrimitiveFlareSprite(ulong64 id, const C3DVector& pos, unsigned long bank, unsigned long frame, float radius,
                                float sizeX, float sizeY, float minScale, float fadeStart, EEnginePrimitiveBlendMode blend,
                                unsigned long unknown, unsigned char alpha, float angle, EEngineInterpolationSet interp)
        : CEnginePrimitive(0x11, id, interp), WorldPos(pos), BankIndex(bank), Frame(frame), LightSourceRadius(radius), SizeX(sizeX),
          SizeY(sizeY), MinScale(minScale), FadeStart(fadeStart), Angle(angle), BlendMode(blend), Identifier((long)id),
          Alpha(alpha) { }
};

namespace NGraphicAppearance {
class CPrimitive { public:                                  // 16 bytes
    long                   Type;                            // +0x00 EPrimitiveType
    CEnginePrimitiveHandle PrimitiveHandle;                 // +0x04
    long                   BankIndex;                       // +0x0C
    CPrimitive(long type) : Type(type), BankIndex(0) { }
    ~CPrimitive();                                          // 0x004C4160
};
class CGraphicAppearanceDrawInfo;
class CTCGraphicAppearance { public:
    float GetFadeDistance() const;                          // 0x004BC180
    CPrimitive* AddPrimitive(const CPrimitive& primitive);  // 0x004C0050
    void SetPrimitiveGraphics(CPrimitive* primitive, const CEnginePrimitive& graphics, unsigned short flags);   // 0x004BD620
};
}
using NGraphicAppearance::CPrimitive;
using NGraphicAppearance::CTCGraphicAppearance;
using NGraphicAppearance::CGraphicAppearanceDrawInfo;

struct CPrimitiveVector {                                   // STLport vector<CPrimitive*> (12 bytes)
    CPrimitive** _M_start;
    CPrimitive** _M_finish;
    CPrimitive** _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    CPrimitive*& operator[](unsigned long n) { return _M_start[n]; }
    void resize(unsigned long n, CPrimitive* const& x);     // 0x006426A0
};

#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"
#include "engine/CDefPointer.h"
class CLightDef : public CDefClassBase { public:           // engine/CLightDef.h flattens the vptr
    unsigned long Colour;                                   // +0x28 CRGBColour
    long          HaloGraphic;                              // +0x2C
};

class CTCLight : public CTCBase { public:
    unsigned char    _pad0c[0x18 - 0x0c];
    CDefPointer<const CLightDef> PLightDef;                 // +0x18
    bool             Active;                                // +0x1C
    bool             Overridden;                            // +0x1D
    unsigned char    _pad1e[0x2c - 0x1e];
    CPrimitiveVector FlareSprites;                          // +0x2C
    bool IsOverridden() const { return Overridden; }                  // Ego_r.pdb QBE_NXZ
protected:
    long GetHaloGraphic() const { return PLightDef->HaloGraphic; }    // Ego_r.pdb IBEJXZ
    void DrawFlare(long index, const C3DVector& position, CTCGraphicAppearance& appearance,
                   const CGraphicAppearanceDrawInfo& info);
};

void CTCLight::DrawFlare(long index, const C3DVector& position, CTCGraphicAppearance& appearance,
                         const CGraphicAppearanceDrawInfo& info)
{
    float fade = appearance.GetFadeDistance();
    if ((unsigned long)index >= FlareSprites.size()) {
        FlareSprites.resize(index + 1, 0);
        CPrimitive primitive(3);
        CPrimitive** sprites = FlareSprites._M_start;
        sprites[index] = appearance.AddPrimitive(primitive);
    }
    long halo;
    if (!IsOverridden() && (halo = GetHaloGraphic()) > 0) {
        CEnginePrimitiveFlareSprite flare(GetParentThing().GetUniqueID(), position, halo, 0, 1.0f, 2.5f, 2.5f, 0.33f, 0.0f,
                                          (EEnginePrimitiveBlendMode)4, 0, (unsigned char)fade, 0.0f, (EEngineInterpolationSet)0);
        appearance.SetPrimitiveGraphics(FlareSprites[index], flare, 0);
    }
}
