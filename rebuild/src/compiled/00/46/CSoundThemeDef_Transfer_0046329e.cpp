// CSoundThemeDef::Transfer(CPersistContext& context) -- persist the sound theme: atmos index, music set, edit colour, gain/range/granularity, the "AtmosGroupMap" multimap (default 0) and the four inside/outside play flags
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0046329E, length 150 bytes (virtual, `?Transfer@CSoundThemeDef@@UAEXAAVCPersistContext@@@Z`).
// Callees (per-type CPersistContext transfers of the definition module; the catalog's "CEngineLightingManager::UpdateShadowScene"
// labels are propagated): long 0x00431102, CRGBColour 0x004595A3, float 0x00431061, bool 0x0043314A, and the map transfer 0x00463334
// (catalog TransferMap<EGameAction,CCharString>: same body shape) with "AtmosGroupMap" @ 0x012353A8.
// Layout: Ego_r.pdb CSoundThemeDef (0x6c) less 4 in retail: EditColour +0x3c, AtmosIndex +0x40, MusicSet +0x44, Gain +0x48,
// Range +0x4c, Granularity +0x50, AtmosGroupMap +0x54 (12-byte STLport tree), flags +0x60..+0x63.
// TU: `optimize("y", off)` + `("s", on)`; the zero default `long` lives in the dead `context` slot (`and [ebp+8],0`).
// Shape: the flags are transferred inside, outside, quietly-inside, quietly-outside (+0x60, +0x62, +0x61, +0x63).
// Leaf: Transfer
#pragma optimize("y", off)
#pragma optimize("s", on)
struct CRGBColour { unsigned char B, G, R, A; };
struct CAtmosGroupMap { void* _M_header[3]; };       // STLport multimap<CCharString, long>, 12 bytes

struct CPersistContext {
    void TransferBinary(long* value);                // 0x00431102
    void TransferBinary(CRGBColour* value);          // 0x004595A3
    void TransferBinary(float* value);               // 0x00431061
    void TransferBinary(bool* value);                // 0x0043314A
    void TransferMap(const char* name, CAtmosGroupMap& map, const long& defaultValue);   // 0x00463334
};

struct CSoundThemeDef {
    virtual void Transfer(CPersistContext& context);
    unsigned char  _pad04[0x38];                     // definition base (retail 4 bytes below the PDB)
    CRGBColour     EditColour;                       // +0x3c
    long           AtmosIndex;                       // +0x40
    long           MusicSet;                         // +0x44
    float          Gain;                             // +0x48
    float          Range;                            // +0x4c
    float          Granularity;                      // +0x50
    CAtmosGroupMap AtmosGroupMap;                    // +0x54
    bool           PlayOnlyWhenInside;               // +0x60
    bool           PlayQuietlyWhenInside;            // +0x61
    bool           PlayOnlyWhenOutside;              // +0x62
    bool           PlayQuietlyWhenOutside;           // +0x63
};

void CSoundThemeDef::Transfer(CPersistContext& context)
{
    context.TransferBinary(&AtmosIndex);
    context.TransferBinary(&MusicSet);
    context.TransferBinary(&EditColour);
    context.TransferBinary(&Gain);
    context.TransferBinary(&Range);
    context.TransferBinary(&Granularity);
    context.TransferMap("AtmosGroupMap", AtmosGroupMap, 0);
    context.TransferBinary(&PlayOnlyWhenInside);
    context.TransferBinary(&PlayOnlyWhenOutside);
    context.TransferBinary(&PlayQuietlyWhenInside);
    context.TransferBinary(&PlayQuietlyWhenOutside);
}