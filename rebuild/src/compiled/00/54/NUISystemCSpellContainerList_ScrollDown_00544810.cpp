// NUISystem::CSpellContainerList::ScrollDown() -- rotate the spell containers one place down: (unless at the first spell)
//   select the previous spell (clamped to the last), slide each container to the position, alpha and scale of the one
//   after it, and send the last container to above the first one's old place, faded by AlphaOffset.
// Leaf: ScrollDown
// Retail: Fable.exe (TLC, VC7.1) @ 0x00544810, 855 bytes, plain `ret` (8-byte aligned frame). Mirror of ScrollUp 0x00544520.
// Callees: the containers' virtuals ChangePosition (+0x7c), ChangeScale (+0x8c, descriptive), ChangeColour (+0x98),
//   GetPosition (+0xdc), GetColour (+0xe0), GetScale (+0xe4, descriptive), all by value through a hidden pointer;
//   __ftol2 0x00BFEA70. Pool: 255.0f 0x01230014, 0.0f 0x0122DEDC; 0.2f pushed.
// Layout: retail NUISystem::CSpellContainerList = Ego_r.pdb minus 0x30 up to SpellContainers (as the landed
//   InitialiseOffsets 0x00544210): SelectedChild +0x164, SpellContainers +0x170 (12-byte STLport vector),
//   PositionOffset +0x180, AlphaOffset +0x188.
// Shape: the last container is written `SpellContainers[SpellContainers.size() - 1]` in place (an inline back() wrapper
//   changes the CSE of _M_start); the clamp is `x < 0 ? 0 : x`.
#define MAX(a, b) ((a) < (b) ? (b) : (a))
namespace NUISystem {
struct C2DVector {
    float X, Y;
    C2DVector() {}
};
struct CRGBColour {
    unsigned char B, G, R, A;
    CRGBColour() {}
};
class CComponent { public:
    virtual void c000() const; virtual void c001() const; virtual void c002() const; virtual void c003() const; virtual void c004() const; virtual void c005() const;
    virtual void c006() const; virtual void c007() const; virtual void c008() const; virtual void c009() const; virtual void c010() const; virtual void c011() const;
    virtual void c012() const; virtual void c013() const; virtual void c014() const; virtual void c015() const; virtual void c016() const; virtual void c017() const;
    virtual void c018() const; virtual void c019() const; virtual void c020() const; virtual void c021() const; virtual void c022() const; virtual void c023() const;
    virtual void c024() const; virtual void c025() const; virtual void c026() const; virtual void c027() const; virtual void c028() const; virtual void c029() const;
    virtual void c030() const;
    virtual void ChangePosition(const C2DVector& position, float time, bool linear); // slot 31 (+0x7c)
    virtual void c032() const; virtual void c033() const; virtual void c034() const;
    virtual void ChangeScale(const C2DVector& scale, float time, bool linear); // slot 35 (+0x8c)
    virtual void c036() const; virtual void c037() const;
    virtual void ChangeColour(const CRGBColour& colour, float time, bool linear); // slot 38 (+0x98)
    virtual void c039() const; virtual void c040() const; virtual void c041() const; virtual void c042() const; virtual void c043() const; virtual void c044() const;
    virtual void c045() const; virtual void c046() const; virtual void c047() const; virtual void c048() const; virtual void c049() const; virtual void c050() const;
    virtual void c051() const; virtual void c052() const; virtual void c053() const; virtual void c054() const;
    virtual C2DVector GetPosition() const;                              // slot 55 (+0xdc)
    virtual CRGBColour GetColour() const;                               // slot 56 (+0xe0)
    virtual C2DVector GetScale() const;                                 // slot 57 (+0xe4)
};
template <class T> struct CRetailVector {        // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    typedef T* iterator;
    typedef const T* const_iterator;
    iterator begin() { return _M_start; }
    const_iterator begin() const { return _M_start; }
    iterator end() { return _M_finish; }
    const_iterator end() const { return _M_finish; }
    unsigned int size() const { return (unsigned int)(end() - begin()); }
    T& operator[](unsigned int n) { return *(begin() + n); }
};
class CSpellContainerList { public:
    unsigned char _pad00[0x164];
    unsigned long SelectedChild;                 // +0x164
    unsigned char _pad168[0x170 - 0x168];
    CRetailVector<CComponent*> SpellContainers;  // +0x170
    unsigned char _pad17c[0x180 - 0x17c];
    C2DVector     PositionOffset;                // +0x180
    float         AlphaOffset;                   // +0x188
    void ScrollDown();
};

void CSpellContainerList::ScrollDown()
{
    if (SpellContainers.size() <= 1)
        return;
    if (SelectedChild == 0)
        return;
    if (SelectedChild <= SpellContainers.size() - 1) {
        --SelectedChild;
        if (SelectedChild >= SpellContainers.size())
            SelectedChild = SpellContainers.size() - 1;
    }
    C2DVector lastPosition;
    lastPosition = SpellContainers[0]->GetPosition();
    SpellContainers[0]->GetScale();
    CRGBColour lastColour;
    lastColour = SpellContainers[0]->GetColour();
    for (unsigned int i = 0; i < SpellContainers.size() - 1; ++i) {
        SpellContainers[i]->ChangePosition(SpellContainers[i + 1]->GetPosition(), 0.2f, false);
        CRGBColour colour = SpellContainers[i]->GetColour();
        colour.A = SpellContainers[i + 1]->GetColour().A;
        SpellContainers[i]->ChangeColour(colour, 0.2f, false);
        SpellContainers[i]->ChangeScale(SpellContainers[i + 1]->GetScale(), 0.2f, false);
    }
    lastPosition.Y = SpellContainers[SpellContainers.size() - 1]->GetPosition().Y + PositionOffset.Y;
    lastColour.A = (unsigned char)MAX(SpellContainers[SpellContainers.size() - 1]->GetColour().A - AlphaOffset * 255.0f, 0.0f);
    lastColour.R = SpellContainers[SpellContainers.size() - 1]->GetColour().R;
    lastColour.G = SpellContainers[SpellContainers.size() - 1]->GetColour().G;
    lastColour.B = SpellContainers[SpellContainers.size() - 1]->GetColour().B;
    SpellContainers[SpellContainers.size() - 1]->ChangePosition(lastPosition, 0.2f, false);
    SpellContainers[SpellContainers.size() - 1]->ChangeColour(lastColour, 0.2f, false);
}
}