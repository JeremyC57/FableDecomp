// CDefendingCombatantInfo::CProng::CProng(const CProng&) -- out-of-line copy constructor of a combat-ring prong: Blocked flag, the byte-packed prong vector, the blockage timer and the occupier pointer
// Leaf: ??0CProng
// Retail: Fable.exe @ 0x006EFF60, 77 bytes, `ret 4`, returns this; called by CCombatRing::CCombatRing 0x006EC380 (landed, which names
//   this address as CProng's copy ctor) and the vector<CProng> growth paths.
// Callees: CBaseIntelligentPointer ctor 0x00A01B10, GetPItem 0x00A01B50, SetPItem 0x00A01B90 (inline CIntelligentPointer<CThingCreatureBase>
//   copy constructor, vtable 0x0123E7D8).
// Layout (Ego_r.pdb, as in the landed CCombatRing model): Blocked +0, C3DVector Prong +1 (byte-packed head), ProngBlockageTimer +0x10,
//   POccupier +0x14; sizeof 0x1C.
// Shape: memberwise copy -- the head's byte then three unaligned dwords at +1/+5/+9 (C3DVector has a user memberwise copy, so the
//   head is not block-copied), the timer, then the intelligent-pointer copy built in ebx.
struct C3DVector {
    float X, Y, Z;
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
struct CBaseObject;
struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;                             // +4
    CBaseIntelligentPointer();                        // 0x00A01B10
    CBaseObject* GetPItem() const;                    // 0x00A01B50
    void SetPItem(CBaseObject* item);                 // 0x00A01B90
};
struct CIntelligentPointer : CBaseIntelligentPointer {   // CIntelligentPointer<CThingCreatureBase>, vtable 0x0123E7D8
    virtual void v00();
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};
#pragma pack(push, 1)
struct CProngHead {                                   // the PDB's byte-packed head of CProng
    bool      Blocked;                                // +0x00
    C3DVector Prong;                                  // +0x01
};
#pragma pack(pop)
struct CDefendingCombatantInfo {
    struct CProng : CProngHead {                      // 28 bytes
        long                ProngBlockageTimer;       // +0x10
        CIntelligentPointer POccupier;                // +0x14
        CProng(const CProng& other);
    };
};
CDefendingCombatantInfo::CProng::CProng(const CProng& other)
    : CProngHead(other), ProngBlockageTimer(other.ProngBlockageTimer), POccupier(other.POccupier)
{
}