// CTCMultiArrow::ApplyEffectOnWeapon(CThing* weapon) -- highlight and tint a living weapon's graphic appearance (interface 0x5b) with the def's bow-highlight width and colour, owned by this ability; mark the effect as not removed
// Leaf: ?ApplyEffectOnWeapon@CTCMultiArrow@@QAEXPAVCThing@@@Z
// Plan phase 2 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). By-value members re-padded to the header layout (re-padded by-value maps +8).
// Retail: Fable.exe (TLC, VC7.1) @ 0x007CC060, 182 bytes (`ret 4`; PDB ?ApplyEffectOnWeapon@CTCMultiArrow@@QAEXPAVCThing@@@Z).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020 (interface 0x5b = bit 27 of CThing+0x28),
//   NGraphicAppearance::CTCGraphicAppearance::SetAsHighlighted(float, float, const CRGBColour&, bool, bool,
//   EAppearanceMeshEffectPriority, void*) @ 0x004C04C0 and SetTintColour(const CRGBColour&, void*) @ 0x004C00B0.
// Layout: Ego_r.pdb CTCMultiArrow (as the landed RemoveEffectOnWeapon 0x007CC120: PDef +0x0c, RemovedEffect +0x20);
//   CSpecialAbilitiesMultiArrowDef at PDB - 4: BowHighlightWidth +0x64, BowHighlightColor +0x68 (vector<long>, the four
//   channels narrowed to bytes). CThing +0x91 bit 0 = dead.
// Shape: the mirror of RemoveEffectOnWeapon (`weapon && alive && GetInterface`, the store after the `if`); the colour is one
//   named CRGBColour (R,G,B,A assigned in the ctor body, as the landed CTCFlashOnHit 0x007E33B0) handed to both calls -- the
//   blue channel's byte passes through the dead `weapon` slot while the other three load.
class CTCBase;
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CBaseIntelligentPointer {
public:
    CBaseIntelligentPointer();                          // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                           // 0x00A01B50
    void SetPItem(CThing* item);                        // 0x00A01B90
    void* PData;
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // vtable 0x01238C6C
public:
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};
inline bool IsThingAlive(const CBaseIntelligentPointer& p) { return p.GetPItem() && !p.GetPItem()->IsDead(); }
template <class T> struct vector {                      // STLport vector, 12 bytes
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    T* begin() const { return _M_start; }
    T* end() const { return _M_finish; }
    T& operator[](unsigned long n) const { return *(begin() + n); }
    T& back() const { return *(end() - 1); }
    void pop_back() { --_M_finish; _M_finish->~T(); }
};
class CRGBColour { public:
    unsigned char B;                                    // +0
    unsigned char G;                                    // +1
    unsigned char R;                                    // +2
    unsigned char A;                                    // +3
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
namespace NGraphicAppearance {
    enum EAppearanceMeshEffectPriority { PRIORITY_1 = 1 };
    class CTCGraphicAppearance { public:
        void SetAsHighlighted(float start, float width, const CRGBColour& colour, bool includeCarried, bool flag,
                              EAppearanceMeshEffectPriority priority, void* owner);   // 0x004C04C0
        void SetTintColour(const CRGBColour& colour, void* owner);   // 0x004C00B0
    };
}
class CSpecialAbilitiesMultiArrowDef { public:                 // retail = PDB - 4
    unsigned char _pad00[0x64];
    float         BowHighlightWidth;                    // +0x64
    vector<long>  BowHighlightColor;                    // +0x68
};
class CTCMultiArrow { public:
    virtual void v00();
    CThing*       ParentThing;                          // +0x04 (CTCBase)
    unsigned char _base08[4];
    const CSpecialAbilitiesMultiArrowDef* PDef;         // +0x0c CDefPointer
    long          NumberOfShots;                        // +0x10
    long          NumberInVolley;                       // +0x14
    long          NumOfVolleys;                         // +0x18
    long          SpellLevel;                           // +0x1c
    bool          RemovedEffect;                        // +0x20
    long          CreateAmmoEffectCount;                // +0x24
    long          CreateAmmoEffectDelay;                // +0x28
    long          CreateAmmoEffectTimer;                // +0x2c
    vector< CIntelligentPointer<CThing> > AmmoEffects;  // +0x30
    void FinalCleanup();                                // 0x007CC410
    void ApplyEffectOnWeapon(CThing* weapon);
    void ReleaseShot();
};

void CTCMultiArrow::ApplyEffectOnWeapon(CThing* weapon)
{
    NGraphicAppearance::CTCGraphicAppearance* appearance;
    if (weapon && !weapon->IsDead() && weapon->GetTC(TCI_GRAPHIC_APPEARANCE_NEW, &appearance)) {
        const vector<long>& c = PDef->BowHighlightColor;
        CRGBColour colour((unsigned char)c[0], (unsigned char)c[1], (unsigned char)c[2], (unsigned char)c[3]);
        appearance->SetAsHighlighted(0.0f, PDef->BowHighlightWidth, colour, false, true, NGraphicAppearance::PRIORITY_1, this);
        appearance->SetTintColour(colour, this);
    }
    RemovedEffect = false;
}