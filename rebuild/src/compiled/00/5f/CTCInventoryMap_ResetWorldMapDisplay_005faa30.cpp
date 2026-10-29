// CTCInventoryMap::ResetWorldMapDisplay() -- show the world map with the hero's current region selected
// Retail 0x005FAA30, 37 bytes (+11 int3). Caller: CWorld::UpdateRegionLoad (0x004A5628).
// Shape: WorldMapActive (+0x234) = true; CTCBase::PeekWorld() const 0x00686A80 (the landed accessor,
// manifest label "CViewBase::GetDisplayEngine" propagated), the world's virtual slot 13 PeekWorldMap (`call
// [edx+34h]`) returns the world map, whose GetLoadedRegion() const 0x004FB150 (`mov eax,[ecx+9Ch]`,
// manifest label "CWorld::DrawGetEnvironment" propagated) is stored to ActiveRegionIndex (+0x238).
// The chained calls go through eax/ecx with `this` kept in esi. Retail offsets as 0x005FA910, local model.

class CWorldMap { public:
    long GetLoadedRegion() const;                  // 0x004FB150 (Ego_r.pdb)
};

class CWorld { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12();
    virtual const CWorldMap& PeekWorldMap() const; // slot 13 (+0x34) (FableWin.pdb)
};

class CTCBase { public:
    virtual ~CTCBase();                                           // slot 0 (0x00686830)
protected:
    const CWorld& PeekWorld() const;   // 0x00686A80 (Ego_r.pdb CTCBase, protected)
};
struct CTCInventoryBase : public CTCBase {
    unsigned char _base_04[0x12C];                 // .. +0x130
};

class CTCInventoryMap : public CTCInventoryBase { public:
    unsigned char _pad_130[0x104];
    bool WorldMapActive;                           // +0x234
    long ActiveRegionIndex;                        // +0x238
    long NumRegions;                               // +0x23C
    void ResetWorldMapDisplay();
};

void CTCInventoryMap::ResetWorldMapDisplay()
{
    WorldMapActive = true;
    ActiveRegionIndex = PeekWorld().PeekWorldMap().GetLoadedRegion();
}