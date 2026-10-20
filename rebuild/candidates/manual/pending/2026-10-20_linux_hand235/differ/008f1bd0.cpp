// CTCTavernGameCardBase::UpdatePointerScale() -- grows the pointer piece towards full size while the game is in the pointer state (0x17), shrinks it to nothing otherwise, and applies the scale to the pointer's graphic appearance
// Grade: DIFFER(ours v retail) -- 254v256: scheduling only, retail loads the component into eax above the scale push (`mov eax,[eax+4] ; push ecx ; mov ecx,eax`), ours loads it straight into ecx after the push
// Leaf: UpdatePointerScale
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008F1BD0, 256 bytes (plain `ret`; Ego_r.pdb `?UpdatePointerScale@CTCTavernGameCardBase@@QAEXXZ`).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020 (the inline GetTC, interface 0x5B);
//          NGraphicAppearance::CTCGraphicAppearance::SetScale(float) 0x004BFA50.
// Constants: 1.0f (as an integer, 0x3F800000), 0.1f 0x0122EE58 (and its bit pattern 0x3DCCCCCD as a store), 0.99
//          (double) 0x01288288, 1.5f 0x0125AADC, 0.0f 0x0122DEDC, 0.5f 0x0122F59C.
// Layout (retail): CTCTavernGame::TavernGameState +0xc0 (PDB +0xdc); CTCTavernGameCardBase::Pointer +0x140 (PDB +0x160),
//          PointerScale +0x144 (PDB +0x164); CThing interface-present bitset +0x20, interface map +0x44.
// Shape: the pointer is read once into ecx (kept across the scale update for the HasTC test); `PointerScale != 1.0f`
//        against the integer constant held in edx (also the 1.0f store); each arm stores and joins the shared
//        `fstp PointerScale`; the GetTC entry choice is the one-ternary form.
struct CTCBase;

namespace NGraphicAppearance {
struct CTCGraphicAppearance {
    void SetScale(float scale);                 // 0x004BFA50
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

struct CTCTavernGameCardBase {
    virtual void v00();
    unsigned char _pad_0x04[0xbc];
    long          TavernGameState;              // +0xc0
    unsigned char _pad_0xc4[0x7c];
    CThing*       Pointer;                      // +0x140 (CThingObject*)
    float         PointerScale;                 // +0x144
    void UpdatePointerScale();
};

void CTCTavernGameCardBase::UpdatePointerScale()
{
    CThing* pointer = Pointer;
    if (!pointer)
        return;
    if (TavernGameState == 0x17) {
        if (PointerScale != 1.0f) {
            if (PointerScale < 0.1f)
                PointerScale = 0.1f;
            else if (PointerScale > 0.99)
                PointerScale = 1.0f;
            else
                PointerScale = PointerScale * 1.5f;
        }
    } else if (PointerScale != 0.0f) {
        if (PointerScale < 0.1f)
            PointerScale = 0.0f;
        else
            PointerScale = PointerScale * 0.5f;
    }
    if (pointer->HasTC(0x5b))
        ((NGraphicAppearance::CTCGraphicAppearance*)pointer->GetTC(0x5b))->SetScale(PointerScale);
}
