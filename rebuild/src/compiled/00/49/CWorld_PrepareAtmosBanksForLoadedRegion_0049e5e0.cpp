// CWorld::PrepareAtmosBanksForLoadedRegion() -- mute or unmute the atmos sounds for the loaded region's sound themes
// Leaf: ?PrepareAtmosBanksForLoadedRegion@CWorld@@IAEXXZ
// Retail: Fable.exe @ 0x0049E5E0, 61 bytes. PDB ?PrepareAtmosBanksForLoadedRegion@CWorld@@IAEXXZ
// Callees: CWorld virtual slot 13 PeekWorldMap() const (+0x34, 0x004AE8C0), CWorldMap::GetLoadedRegion() const
// 0x004FB150 (returns +0x9C; manifest label "DrawGetEnvironment" is wrong), CWorldMap::GetRegion(long) const
// 0x004FC180, CAtmosProcess::SetAtmosSoundsAsMuted(bool, float) 0x006B1D30 (manifest label "Sound_SetThemeByName"
// is a guess; the PDB name set has ?SetAtmosSoundsAsMuted@CAtmosProcess@@QAEX_NM@Z and the body tests the bool).
// Retail CWorld: PAtmosProcessor (CCountedPointer<CAtmosProcess>::Data) +0x24 (PDB = retail). Retail CRegion is
// 0x58 bytes with SoundThemesEnabled at +0x56 (PDB +0x62: two 12-byte vectors and a 12-byte map ahead of it).
// Shape: the world map is peeked twice through the virtual (once per call), the mute flag is the negated region
// byte (`test dl,dl ; sete al`) and the fade time is the 1000.0f immediate.
struct CRegion {
    unsigned char _pad00[0x56];
    bool          SoundThemesEnabled;           // +0x56 (PDB +0x62)
    unsigned char _pad57;
};

struct CWorldMap {
    long GetLoadedRegion() const;               // 0x004FB150
    const CRegion& GetRegion(long region) const;    // 0x004FC180
};

struct CAtmosProcess {
    void SetAtmosSoundsAsMuted(bool muted, float fadeTime);    // 0x006B1D30
};

struct CWorld {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual CWorldMap& GetWorldMap();                   // slot 12 (+0x30)
    virtual const CWorldMap& PeekWorldMap() const;      // slot 13 (+0x34) 0x004AE8C0
    unsigned char  _pad04[0x20];
    CAtmosProcess* PAtmosProcessor;             // +0x24 CCountedPointer<CAtmosProcess>::Data
    void*          PAtmosProcessorInfo;         // +0x28 CCountedPointer<CAtmosProcess>::Info
protected:
    void PrepareAtmosBanksForLoadedRegion();       // PDB protected (IAE)
};

void CWorld::PrepareAtmosBanksForLoadedRegion()
{
    if (PAtmosProcessor)
    {
        const CRegion& region = PeekWorldMap().GetRegion(PeekWorldMap().GetLoadedRegion());
        PAtmosProcessor->SetAtmosSoundsAsMuted(!region.SoundThemesEnabled, 1000.0f);
    }
}