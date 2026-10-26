// CBossDef::operator=(const CBossDef&) -- def copy: CDefClassBase part, three phase/shot vectors, two twin-blade scalars
// Leaf: ??4CBossDef
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E2C08, 69 bytes, size-optimised TU (/Os def region, like the landed
//         0x004663AE / 0x004E05AD def assignments).
// Callees: CDefClassBase::operator= @ 0x00431F10 (the manifest label "CThingBuildingDef::operator=" is propagated
//          over every def base assignment), std::vector<long>::operator= @ 0x00454886.
// Identification: PDB CBossDef = CDefClassBase + PhaseHealthPercent, Shots, TwinBladeMissesBeforeLunge (std::vector<long>)
//   +0x28/+0x34/+0x40, TwinBladeStuckLoops +0x4c, TwinBladeTimeBeforeLunge (float) +0x50 (retail offsets).
// Shape: memberwise; each vector goes through the out-of-line vector<long>::operator= 0x00454886.
// The generated engine headers flatten the bases (no base operator= to call), so the classes are modelled locally.
#pragma optimize("s", on)

class CDefClassBase { public:
    unsigned char _body[0x25];          // vptr, resource links, PDefManager +0x1c, GlobalIndex +0x20, flag bits +0x24
    CDefClassBase& operator=(const CDefClassBase& other);   // 0x00431F10
};

struct CLongVector {                    // std::vector<long> (retail STLport, 12 bytes)
    long* m_pFirst;
    long* m_pLast;
    long* m_pEndOfStorage;
    CLongVector& operator=(const CLongVector& other);   // 0x00454886
};

class CBossDef : public CDefClassBase { public:
    CLongVector PhaseHealthPercent;     // +0x28
    CLongVector Shots;                  // +0x34
    CLongVector TwinBladeMissesBeforeLunge;// +0x40
    long TwinBladeStuckLoops;           // +0x4c
    float TwinBladeTimeBeforeLunge;     // +0x50
    CBossDef& operator=(const CBossDef& other);
};

CBossDef& CBossDef::operator=(const CBossDef& other)
{
    CDefClassBase::operator=(other);
    PhaseHealthPercent = other.PhaseHealthPercent;
    Shots = other.Shots;
    TwinBladeMissesBeforeLunge = other.TwinBladeMissesBeforeLunge;
    TwinBladeStuckLoops = other.TwinBladeStuckLoops;
    TwinBladeTimeBeforeLunge = other.TwinBladeTimeBeforeLunge;
    return *this;
}