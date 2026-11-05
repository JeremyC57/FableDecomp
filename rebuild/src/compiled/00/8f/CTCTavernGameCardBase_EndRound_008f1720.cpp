// CTCTavernGameCardBase::EndRound() -- base EndRound, clears the highlight off the highlighted card's graphic appearance, empties the card list and forgets the selected and highlighted cards
// Leaf: ?EndRound@CTCTavernGameCardBase
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008F1720, 171 bytes (plain `ret`); CTCTavernGameCardBase vtable 0x01288294
// slot 33. Ego_r.pdb `?EndRound@CTCTavernGameCardBase@@UAEXXZ`.
// Callees: CTCTavernGame::EndRound() 0x005E9AA0 (non-virtual base call; the landed SpotTheAddition override 0x007D7BA0
//          spells it `vfunc_33`, Ego_r.pdb's source order NewGame, NewRound, EndRound names it); CVectorMap<
//          ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020 (the inline GetTC, interface 0x5B = word 2 bit 27);
//          NGraphicAppearance::CTCGraphicAppearance::SetAsHighlighted(float, float, const CRGBColour&, bool, bool,
//          EAppearanceMeshEffectPriority, void*) 0x004C04C0; memmove 0x00BFEAE6 (the STLport __copy_trivial of the
//          inline clear()).
// Layout (retail, as the landed constructor 0x008F1CD0: CTCTavernGame is 0xf0 bytes and every vector 12): Cards +0x128,
//         SelectedCard +0x308, HighlightedCard +0x30c; CCard::Thing +0x00; the thing's interface bitset +0x20, map +0x44.
// Shape: `T* p = 0; if (HasTC(k)) p = GetTC(k);` preloads the `xor ecx,ecx`, and GetTC's end/hit choice written as ONE
//        ternary on the entry lands the component straight in ecx for the call (an if-reassign form routes it via eax); the CRGBColour(0, 0, 0, 0) temporary assigns
//        R, G, B, A in its body (stores +2, +1, +0, +3) and shares the lookup key's `push ecx` frame word; the two 0.0f
//        arguments ride in the shared zero register; Cards.clear() is the STLport erase(begin(), end()) through
//        __copy_trivial (`cmp eax,eax` on the twice-loaded finish pointer).
extern "C" void* __cdecl memmove(void* dest, const void* source, unsigned int count);   // 0x00BFEAE6

inline void* CopyTrivial(const void* first, const void* last, void* result)
{
    return (last == first) ? result
        : (char*)memmove(result, first, (const char*)last - (const char*)first) + ((const char*)last - (const char*)first);
}

class CTCBase;
class CCard;
class CThingObject;

struct CCardVector {                            // STLport vector<CCard*> (12 bytes)
    CCard** _M_start;
    CCard** _M_finish;
    CCard** _M_end_of_storage;
    void clear()
    {
        CCard** i = (CCard**)CopyTrivial(_M_finish, _M_finish, _M_start);
        _M_finish = i;
    }
};

class CRGBColour { public:
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};

namespace NGraphicAppearance {
enum EAppearanceMeshEffectPriority { APPEARANCE_MESH_EFFECT_PRIORITY_3 = 3 };
class CTCGraphicAppearance { public:
    void SetAsHighlighted(float a, float b, const CRGBColour& colour, bool c, bool d, EAppearanceMeshEffectPriority priority,
                          void* owner);       // 0x004C04C0
};
}

#define FABLE_RETAIL_CTHING
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CCard { public:
    CThing* Thing;                              // +0x00 CThingObject*
};

class CTCTavernGame { public:
    virtual void v00();
    unsigned char _body04[0xf0 - 0x04];         // CTCBase + CTCTavernGame members (retail 0xF0)
    void EndRound();                            // 0x005E9AA0
};

struct CTCTavernGameCardBase : CTCTavernGame {  // vtable 0x01288294
    unsigned char _pad_0xf0[0x38];
    CCardVector   Cards;                        // +0x128
    unsigned char _pad_0x134[0x1d4];
    CCard*        SelectedCard;                 // +0x308
    CCard*        HighlightedCard;              // +0x30c
    void EndRound();
};

void CTCTavernGameCardBase::EndRound()
{
    CTCTavernGame::EndRound();
    if (HighlightedCard) {
        CThing* thing = HighlightedCard->Thing;
        NGraphicAppearance::CTCGraphicAppearance* appearance = 0;
        if (thing->HasTC(TCI_GRAPHIC_APPEARANCE_NEW))
            appearance = (NGraphicAppearance::CTCGraphicAppearance*)((NGraphicAppearance::CTCGraphicAppearance*)&thing->GetTC(TCI_GRAPHIC_APPEARANCE_NEW));
        appearance->SetAsHighlighted(0.0f, 0.0f, CRGBColour(0, 0, 0, 0), false, false,
                                     NGraphicAppearance::APPEARANCE_MESH_EFFECT_PRIORITY_3, this);
    }
    Cards.clear();
    SelectedCard = 0;
    HighlightedCard = 0;
}