// CGameScriptThing::GetHomeMapName() const -- the level script name (CWorldMap::GetLevelScriptName @ 0x004FCFC0) of the map the world map (world at +0x44, slot 13) finds for the UID record at +0x34 (world map virtual slot 16); the empty string when there is no such map
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008CFEB0, length 96 bytes.
// Callees: CWorld virtual slot 13 (+0x34), the world map's virtual slot 16 (+0x40: the record by address, a map number), CWorldMap::GetLevelScriptName @ 0x004FCFC0, the CCharString
// copy constructor @ 0x0099EC30 into the hidden return slot, or CCharString(const char*, long) @ 0x0099EBF0 with the empty string @ 0x0122D70E and -1. Shape: the world map in esi,
// the record address formed with `add edi,34h`, the hidden pointer read from the stack on both arms, `ret 4`.
// Leaf: ?GetHomeMapName@CGameScriptThing@@UBE?AVCCharString@@XZ
struct CTCGraphicAppearance_Retail;
struct CCharString {
    void* m_pData;
    CCharString(const CCharString& other);         // 0x0099EC30
    CCharString(const char* text, long length);    // 0x0099EBF0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};
extern const char g_emptyString_0122d70e[];       // 0x0122D70E ("")
struct CWorldMap_Retail {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual long GetMapNumberForRecord(const void* record);   // slot 16 (+0x40)
    const CCharString& GetLevelScriptName(long map);   // 0x004FCFC0
};
struct CWorld {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012();
    virtual CWorldMap_Retail* GetWorldMap();         // slot 13 (+0x34)
};

struct CGameScriptThing {
    virtual void v00();
    unsigned char _pad04[0x30];
    unsigned char m_record[4];                     // +0x34
    unsigned char _pad38[0xc];
    CWorld*       m_pWorld;                        // +0x44
    virtual CCharString GetHomeMapName() const;
};

CCharString CGameScriptThing::GetHomeMapName() const
{
    CWorldMap_Retail* worldMap = m_pWorld->GetWorldMap();
    long map = worldMap->GetMapNumberForRecord(m_record);
    if (map)
        return CCharString(worldMap->GetLevelScriptName(map));
    return CCharString(g_emptyString_0122d70e, -1);
}