// CSoundDef::Transfer(CPersistContext& context) -- persist the sound definition: the five entry vectors (sound banks, atmos banks, music, music sets, reverb environments) and three floats with zero defaults
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0046A9CE, length 149 bytes (virtual, `?Transfer@CSoundDef@@UAEXAAVCPersistContext@@@Z`).
// Callees: the per-vector CPersistContext transfer instantiations 0x0046AA63 / 0x0046AFC6 / 0x0046B3F4 / 0x0046B92A / 0x0046BE1F
// (each CheckCRC("") + load/save; catalog "CCreatureAction_FireMissileWeapon::FrameUpdate" is a propagated label),
// CPersistContext::Transfer<float>(name, value, default) @ 0x00410620 with "AtmosListenerDampingMin" / "...Range" / "DistanceModelCurve".
// Layout: Ego_r.pdb CSoundDef vectors at +0x40.. (16-byte); retail is 4 bytes lower with 12-byte vectors: +0x3c..+0x6c, floats +0x78/+0x7c/+0x80.
// TU: the 0x0046xxxx definition module (`optimize("y", off)` + `("s", on)`); the zero default lives in the dead `context` slot.
// Leaf: Transfer
#pragma optimize("y", off)
#pragma optimize("s", on)
struct CSoundBankEntry; struct CAtmosBankEntry; struct CMusicEntry; struct CMusicSetEntry; struct CReverbEnvironmentEntry;
template <class T> struct CRetailVector { T* _M_start; T* _M_finish; T* _M_end_of_storage; };   // 12-byte retail vector

struct CPersistContext {
    void Transfer(const char* name, float& value, const float& defaultValue);   // 0x00410620
    void TransferBinary(CRetailVector<CSoundBankEntry>* v);            // 0x0046AA63
    void TransferBinary(CRetailVector<CAtmosBankEntry>* v);            // 0x0046AFC6
    void TransferBinary(CRetailVector<CMusicEntry>* v);                // 0x0046B3F4
    void TransferBinary(CRetailVector<CMusicSetEntry>* v);             // 0x0046B92A
    void TransferBinary(CRetailVector<CReverbEnvironmentEntry>* v);    // 0x0046BE1F
};

struct CSoundDef {
    virtual void Transfer(CPersistContext& context);
    unsigned char _pad04[0x38];                                        // definition base (retail 4 bytes below the PDB)
    CRetailVector<CSoundBankEntry>         SoundBankEntries;           // +0x3c
    CRetailVector<CAtmosBankEntry>         AtmosBankEntries;           // +0x48
    CRetailVector<CMusicEntry>             MusicEntries;               // +0x54
    CRetailVector<CMusicSetEntry>          MusicSetEntries;            // +0x60
    CRetailVector<CReverbEnvironmentEntry> ReverbEnvironmentEntries;   // +0x6c
    float AtmosListenerDampingMin;                                     // +0x78
    float AtmosListenerDampingRange;                                   // +0x7c
    float DistanceModelCurve;                                          // +0x80
};

void CSoundDef::Transfer(CPersistContext& context)
{
    context.TransferBinary(&SoundBankEntries);
    context.TransferBinary(&AtmosBankEntries);
    context.TransferBinary(&MusicEntries);
    context.TransferBinary(&MusicSetEntries);
    context.TransferBinary(&ReverbEnvironmentEntries);
    context.Transfer("AtmosListenerDampingMin", AtmosListenerDampingMin, 0.0f);
    context.Transfer("AtmosListenerDampingRange", AtmosListenerDampingRange, 0.0f);
    context.Transfer("DistanceModelCurve", DistanceModelCurve, 0.0f);
}