// CTCTavernGameCardBase::EndRound() -- base EndRound, clears the highlight off the highlighted card's graphic appearance, empties the card list and forgets the selected and highlighted cards
// Leaf: ?EndRound@CTCTavernGameCardBase
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

struct CTCBase;
struct CCard;
struct CThingObject;

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

struct CRGBColour {
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};

namespace NGraphicAppearance {
enum EAppearanceMeshEffectPriority { APPEARANCE_MESH_EFFECT_PRIORITY_3 = 3 };
struct CTCGraphicAppearance {
    void SetAsHighlighted(float a, float b, const CRGBColour& colour, bool c, bool d, EAppearanceMeshEffectPriority priority,
                          void* owner);       // 0x004C04C0
};
}

struct CInterfaceMap {                          // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned long HasTC(long interfaceType) const { return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)); }
    __forceinline CTCBase* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (it == map->last || interfaceType < it->type ? map->last : it)->component;
    }
};
struct CCard {
    CThing* Thing;                              // +0x00 CThingObject*
};

struct CTCTavernGame {
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
        if (thing->HasTC(0x5b))
            appearance = (NGraphicAppearance::CTCGraphicAppearance*)thing->GetTC(0x5b);
        appearance->SetAsHighlighted(0.0f, 0.0f, CRGBColour(0, 0, 0, 0), false, false,
                                     NGraphicAppearance::APPEARANCE_MESH_EFFECT_PRIORITY_3, this);
    }
    Cards.clear();
    SelectedCard = 0;
    HighlightedCard = 0;
}
