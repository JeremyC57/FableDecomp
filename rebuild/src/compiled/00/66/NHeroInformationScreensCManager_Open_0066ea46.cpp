// NHeroInformationScreens::CManager::Open() -- start the screen-effect fade if not already open, mark open, collect the inventory screens
// Leaf: ?Open@CManager@NHeroInformationScreens@@QAEXXZ
// Retail: Fable.exe @ 0x0066EA46, 63 bytes, `ret`; one caller (0x0049F481); Ego_r.pdb ?Open@CManager@NHeroInformationScreens@@QAEXXZ.
// Body: if (!OpenFlag) ScreenEffectFade.Open(1.0f, 0.0f, CRGBColour(0, 0, 0, 255)) (CFadeInFadeOutBase::Open 0x00434840,
// Ego_r.pdb ?Open@CFadeInFadeOutBase@@QAEXMMVCRGBColour@@@Z; the colour built in its argument slot, R +2, G +1, B +0,
// A +3); OpenFlag = true; GetAllRegisteredInventoryScreens(InventoryScreens) (0x0066E9E9, private const).
// Layout: retail NHeroInformationScreens::CManager -- InventoryScreens +0x10, OpenFlag +0x34, ScreenEffectFade +0x38
// (Ego_r.pdb +0x14 / +0x40 / +0x44 with 16-byte vectors), as modelled at 0x0066EA85; CFadeInFadeOutBase from
// engine/CFadeInFadeOutBase.h.
// Shape: CRGBColour has a user copy constructor, so the by-value colour is built straight in its argument slot.
// Opt: size TU (unaligned entry).
#pragma optimize("s", on)
#include "engine/CFadeInFadeOutBase.h"
class CRGBColour {                                      // Ego_r.pdb: 4 bytes, one anonymous union (CRGBColour_u_0)
public:
    union {
        struct {
            unsigned char B;                             // +0
            unsigned char G;                             // +1
            unsigned char R;                             // +2
            unsigned char A;                             // +3
        };
        unsigned long Colour;
    };
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
    CRGBColour(const CRGBColour& c) : Colour(c.Colour) {}
};
struct CFadeInFadeOutBase_Methods : CFadeInFadeOutBase {
    void Open(float fade_in_time, float fade_out_time, CRGBColour to_colour);   // 0x00434840
};
class CWorld;
namespace NHeroInformationScreens {
class CBase;
template <class T> class CVector {                       // STLport vector, 12 bytes
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
};
class CManager {
public:
    CWorld*                       World;                 // +0x00
    CVector<const CBase*>         Screens;               // +0x04
    CVector<const CBase*>         InventoryScreens;      // +0x10
    CVector<long>                 ScrollBarIndices;      // +0x1c
    long                          CurrentScreen;         // +0x28
    long                          LastScreen;            // +0x2c
    long                          HighlightedScreen;     // +0x30
    bool                          OpenFlag;              // +0x34
    unsigned char                 _pad35[3];             // +0x35 (engine/CFadeInFadeOutBase.h is pack(1))
    CFadeInFadeOutBase_Methods    ScreenEffectFade;      // +0x38
    void Open();
private:
    void GetAllRegisteredInventoryScreens(CVector<const CBase*>& screens) const;   // 0x0066E9E9
    friend struct CManager_Test;                         // behaviour-test hook
};
void CManager::Open()
{
    if (!OpenFlag)
        ScreenEffectFade.Open(1.0f, 0.0f, CRGBColour(0, 0, 0, 255));
    OpenFlag = true;
    GetAllRegisteredInventoryScreens(InventoryScreens);
}
}