// CArenaWaveDef::CArenaWaveDef(const CArenaWaveDef&) -- copy constructor: base CDefClassBase(other), own vtable, NumWaveCreatures, the Creatures vector by its copy constructor, ShortWave
// Leaf: ??0CArenaWaveDef
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F25C60, length 51 bytes.
// Callees: CDefClassBase::CDefClassBase(const CDefClassBase&) @ 0x0046644C,
//          vector<CArenaCreatureDef> copy constructor @ 0x00F25D20.
// Identity: the manifest label "CParentDefClassBase::CParentDefClassBase" is propagated. Vtable
// 0x012FB2F0 has no complete-object locator (the TLC-added no-RTTI module), but its slot 18
// (0x00F261F0, the def Transfer) serialises "NumWaveCreatures" at +0x28, "Creatures" at +0x2c and
// "ShortWave" at +0x38, slot 20 (0x00F25CA0) returns sizeof = 0x3c, and slots 10..16 are the plain
// CDefClassBase ones (not a parent def). PDB CArenaWaveDef: NumWaveCreatures +0x28, Creatures
// (16-byte PDB vector) +0x2c, ShortWave +0x3c -- with the 12-byte retail vector ShortWave lands at
// +0x38, as the bytes show. The base is CDefClassBase directly (the scalar deleting destructor
// 0x00F25CB0 stores the CDefClassBase vtable 0x01230BA0 before the base destructor 0x009FC550).
// Shape: user-style memberwise copy constructor: base call, vtable store (scheduled after the
// first `lea`), the long through eax, the vector out of line, the bool through dl.

struct CDefClassBase {
    virtual void v00();
    unsigned char _body[0x24];                                            // +0x04..+0x28 (0x25 bytes + alignment)
    CDefClassBase(const CDefClassBase& other);                            // 0x0046644C
};

struct CArenaCreatureDefVector {                                          // retail std::vector<CArenaCreatureDef> (12 bytes)
    void* m_first;
    void* m_last;
    void* m_end;
    CArenaCreatureDefVector(const CArenaCreatureDefVector& other);        // 0x00F25D20
};

struct CArenaWaveDef : CDefClassBase {                                    // vtable 0x012FB2F0 (no RTTI)
    long                    NumWaveCreatures;                             // +0x28
    CArenaCreatureDefVector Creatures;                                    // +0x2c
    bool                    ShortWave;                                    // +0x38
    virtual void v00();
    CArenaWaveDef(const CArenaWaveDef& other);
};

CArenaWaveDef::CArenaWaveDef(const CArenaWaveDef& other)
    : CDefClassBase(other)
    , NumWaveCreatures(other.NumWaveCreatures)
    , Creatures(other.Creatures)
    , ShortWave(other.ShortWave)
{
}
