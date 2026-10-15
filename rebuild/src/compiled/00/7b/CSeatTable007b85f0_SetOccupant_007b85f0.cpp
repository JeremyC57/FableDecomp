// CSeatTable_007b85f0::SetOccupant(long seat, CThing* occupant) -- point the seat's intelligent pointer (element of the vector at +0x28) at the occupant
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007B85F0, length 78 bytes. `ret 8`.
// Leaf: SetOccupant
// Callees: CBaseIntelligentPointer ctor @ 0x00A01B10, SetPItem @ 0x00A01B90 (twice), GetPItem @ 0x00A01B50, dtor @ 0x00A01C10.
// The catalog label "CVillageTaskCarryCrate::AssignCreatureToTask" is propagated: `this` reads +0x28, past the 0x20-byte
// CVillageTaskCarryCrate; the callers are CAIStateGroup_ServeFood::Update (0x009236C0, three calls) and
// CAIStateGroup_EatAtTable::OnReplace (0x0096CFC0), e.g. `SetOccupant(seat, 0)` -- a table's seat list, so the class is descriptive.
// Shape: the temporary CIntelligentPointer (vtable 0x0125A858) is built first; the vector's start pointer is loaded into esi
// BEFORE GetPItem and the index scaled AFTER it (`lea ecx,[esi+ecx*8]`): STLport `operator[]` as `*(begin() + n)` with an
// inline begin() (a raw pointer member folds the address before the call).
struct CThing;

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                        // +0x04
    CBaseIntelligentPointer();              // 0x00A01B10
    CThing* GetPItem() const;               // 0x00A01B50
    void SetPItem(CThing* item);            // 0x00A01B90
    ~CBaseIntelligentPointer();             // 0x00A01C10
};

struct CIntelligentPointer : CBaseIntelligentPointer {
    virtual void v00();
    CIntelligentPointer() {}
    CIntelligentPointer(CThing* item) { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};
struct COccupantVector {                            // STLport vector<CIntelligentPointer<CThing> >
    CIntelligentPointer* _M_start;
    CIntelligentPointer* _M_finish;
    CIntelligentPointer* _M_end_of_storage;
    CIntelligentPointer* begin() { return _M_start; }
    CIntelligentPointer& operator[](long n) { return *(begin() + n); }
};
struct CSeatTable_007b85f0 {
    unsigned char        _pad00[0x28];
    COccupantVector      m_occupants;               // +0x28
    void SetOccupant(long seat, CThing* occupant);
};
void CSeatTable_007b85f0::SetOccupant(long seat, CThing* occupant)
{
    m_occupants[seat] = CIntelligentPointer(occupant);
}