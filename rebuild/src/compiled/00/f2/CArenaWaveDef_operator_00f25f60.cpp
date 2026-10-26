// CArenaWaveDef::operator=(const CArenaWaveDef&) -- memberwise assignment: base CDefClassBase::operator=, NumWaveCreatures, the Creatures vector by its operator=, ShortWave; returns *this
// Leaf: ??4CArenaWaveDef
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F25F60, length 45 bytes.
// Callees: CDefClassBase::operator= @ 0x00431F10 (manifest label "CThingBuildingDef::operator=" is
//          propagated across byte-identical bodies), vector<CArenaCreatureDef>::operator= @ 0x00F25F90.
// Identity: reached through the `jmp` thunk 0x00F25F50 in slot 19 of vtable 0x012FB2F0, the
// no-RTTI CArenaWaveDef vtable (see 0x00F25C60: its Transfer slot names NumWaveCreatures +0x28,
// Creatures +0x2c, ShortWave +0x38; sizeof 0x3c). The manifest name "CParentDefClassBase::operator="
// is propagated.
// Shape: compiler-generated memberwise assignment in declaration order: base out of line, the
// long through eax, the vector out of line, the bool through dl.

class CDefClassBase { public:
    virtual void v00();
    unsigned char _body[0x24];                                            // +0x04..+0x28 (0x25 bytes + alignment)
    CDefClassBase& operator=(const CDefClassBase& other);                 // 0x00431F10
};

struct CArenaCreatureDefVector {                                          // retail std::vector<CArenaCreatureDef> (12 bytes)
    void* m_first;
    void* m_last;
    void* m_end;
    CArenaCreatureDefVector& operator=(const CArenaCreatureDefVector& other);   // 0x00F25F90
};

class CArenaWaveDef : public CDefClassBase { public:                                    // vtable 0x012FB2F0 (no RTTI)
    long                    NumWaveCreatures;                             // +0x28
    CArenaCreatureDefVector Creatures;                                    // +0x2c
    bool                    ShortWave;                                    // +0x38
    virtual void v00();
    CArenaWaveDef& operator=(const CArenaWaveDef& other);
};

CArenaWaveDef& CArenaWaveDef::operator=(const CArenaWaveDef& other)
{
    CDefClassBase::operator=(other);
    NumWaveCreatures = other.NumWaveCreatures;
    Creatures = other.Creatures;
    ShortWave = other.ShortWave;
    return *this;
}