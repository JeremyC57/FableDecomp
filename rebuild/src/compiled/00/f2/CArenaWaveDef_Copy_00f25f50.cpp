// CArenaWaveDef::Copy(const CDefClassBase* other) -- slot 19: *this = the other def (tail call to operator=)
// Leaf: ?Copy@CArenaWaveDef@@UAEXPBVCDefClassBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F25F50, 5 bytes: e90b000000.
// Identity (naming wave 35, family def): Anniversary wave 2026-11-04 (high): vtable 0x012FB2F0 slot 19; Anniversary RTTI CArenaWaveDef (string-literal match, validated 257/258); PDB name set has the Copy member of CArenaWaveDef; slot layout from named TLC siblings; function referenced by this vtable only.
//   ?Copy@CArenaWaveDef@@UAEXPBVCDefClassBase@@@Z in Ego_r.pdb / FableWin.pdb (rebuild/candidates/manual/naming/2026-11-04_anniversary/labels_high.tsv); PDB also: ??_ECArenaWaveDef@@UAEPAXI@Z, ??1CArenaWaveDef@@UAE@XZ.
// Was: Free-function forwarder VC7.1 tail-calls (`jmp rel32`): same signature, no cleanup. (catalog stand-in _gapscan).
// Class: the class model of the landed constructor row 0x00F25C60 (rebuild/src/compiled/00/f2/CArenaWaveDef_CArenaWaveDef_00f25c60.cpp); the PDB member declaration added; only this member is graded.

class CDefClassBase { public:
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

class CArenaWaveDef : public CDefClassBase { public:                                    // vtable 0x012FB2F0 (no RTTI)
    long                    NumWaveCreatures;                             // +0x28
    CArenaCreatureDefVector Creatures;                                    // +0x2c
    bool                    ShortWave;                                    // +0x38
    virtual void v00();
    CArenaWaveDef(const CArenaWaveDef& other);
public:
    virtual void Copy(const CDefClassBase* other);   // PDB ?Copy@CArenaWaveDef@@UAEXPBVCDefClassBase@@@Z
};

CArenaWaveDef::CArenaWaveDef(const CArenaWaveDef& other)
    : CDefClassBase(other)
    , NumWaveCreatures(other.NumWaveCreatures)
    , Creatures(other.Creatures)
    , ShortWave(other.ShortWave)
{
}
void CArenaWaveDef::Copy(const CDefClassBase* other)
{
    *this = *static_cast<const CArenaWaveDef*>(other);
}
