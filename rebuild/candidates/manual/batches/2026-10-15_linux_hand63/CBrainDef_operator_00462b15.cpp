// CBrainDef::operator=(const CBrainDef&) -- memberwise assignment: base CParentDefClassBase::operator=, then the Behaviours, DisabledBehaviours and UpdateZones vectors by their operator=; returns *this
// Leaf: ??4CBrainDef
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00462B15, length 57 bytes.
// Pragma: #pragma optimize("s", on) (the def module; one register per address pair).
// Callees: CParentDefClassBase::operator= @ 0x00431ED0,
//          vector<CBrainBehaviour>::operator= @ 0x00462B4E,
//          vector<CDefString>::operator= @ 0x0046078C,
//          vector<CBrainUpdateZone>::operator= @ 0x00462CB0.
// Identity: the manifest label "CParentDefClassBase::operator=" is propagated. The body is reached
// through a `jmp` thunk at 0x00462B10 in slot 19 of vtable 0x0123520C = CBrainDef
// (rtti-vtables.tsv); PDB CBrainDef holds exactly three vectors (Behaviours, DisabledBehaviours,
// UpdateZones), here at +0x3c/+0x48/+0x54 (retail base 0x3c, retail vectors 12 bytes).
// Shape: compiler-generated memberwise assignment; the last member's source address is formed with
// `add edi,54h` because `other` is dead after it (the natural `&other.UpdateZones` of the final
// call). The PDB header (engine/CParentDefClassBase.h) describes the Ego layout, so the retail
// base is modelled locally.
#pragma optimize("s", on)   // the def module is size-optimised

struct CParentDefClassBase {
    virtual void v00();
    unsigned char _body[0x38];                                            // +0x04..+0x3c
    CParentDefClassBase& operator=(const CParentDefClassBase& other);     // 0x00431ED0
};

struct CBrainBehaviourVector {                                            // retail std::vector (12 bytes)
    void* m_first;
    void* m_last;
    void* m_end;
    CBrainBehaviourVector& operator=(const CBrainBehaviourVector& other);   // 0x00462B4E
};

struct CDefStringVector {                                                 // retail std::vector (12 bytes)
    void* m_first;
    void* m_last;
    void* m_end;
    CDefStringVector& operator=(const CDefStringVector& other);           // 0x0046078C
};

struct CBrainUpdateZoneVector {                                           // retail std::vector (12 bytes)
    void* m_first;
    void* m_last;
    void* m_end;
    CBrainUpdateZoneVector& operator=(const CBrainUpdateZoneVector& other);   // 0x00462CB0
};

struct CBrainDef : CParentDefClassBase {                                  // vtable 0x0123520C
    CBrainBehaviourVector  Behaviours;                                    // +0x3c
    CDefStringVector       DisabledBehaviours;                            // +0x48
    CBrainUpdateZoneVector UpdateZones;                                   // +0x54
    virtual void v00();
    CBrainDef& operator=(const CBrainDef& other);
};

CBrainDef& CBrainDef::operator=(const CBrainDef& other)
{
    CParentDefClassBase::operator=(other);
    Behaviours = other.Behaviours;
    DisabledBehaviours = other.DisabledBehaviours;
    UpdateZones = other.UpdateZones;
    return *this;
}
