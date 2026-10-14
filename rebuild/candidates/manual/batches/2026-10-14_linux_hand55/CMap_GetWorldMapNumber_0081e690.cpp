// CMap::GetWorldMapNumber() -- the world-map number of this map's position record (+0x1d24): the world (+0x20) gives its map table (slot 13) which is asked (slot 17) for the record
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0081E690, length 27 bytes.
// Callees: the world's virtual slot 13 (+0x34) and the map table's virtual slot 17 (+0x44, with the record's address). Shape: `add esi,1D24h` for the push, `ret`.
struct CWorldMapTable {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016();
    virtual long GetWorldMapNumber(const void* position);   // slot 17 (+0x44)
};
struct CWorld {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012();
    virtual CWorldMapTable* GetWorldMapTable();   // slot 13 (+0x34)
};

struct CMap {
    unsigned char _pad[0x20];
    CWorld*       m_pWorld;                        // +0x20
    unsigned char _pad24[0x1d00];
    unsigned char m_position[0xc];                 // +0x1d24: the map's world-map position record
    long GetWorldMapNumber();
};

long CMap::GetWorldMapNumber()
{
    return m_pWorld->GetWorldMapTable()->GetWorldMapNumber(m_position);
}
