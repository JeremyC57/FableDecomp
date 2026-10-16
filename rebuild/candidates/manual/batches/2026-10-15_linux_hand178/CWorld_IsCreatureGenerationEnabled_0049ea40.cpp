// CWorld::IsCreatureGenerationEnabled(const CThing&) const -- may this creature generator spawn now?
// Retail: Fable.exe @ 0x0049EA40, 126 bytes. PDB ?IsCreatureGenerationEnabled@CWorld@@QBE_NABVCThing@@@Z
// Callees: the quest manager singleton 0x013B89FC: IsCreatureGenerationEnabled() 0x004AFC60 (label
// "COptimisedPrimitive::DetectFoldOver" is a propagation), IsActiveQuestInRegion(const CRegion&) 0x004AFDD0 (label
// "FindMostRecentMessage<...>"), the CCharString lookup 0x004AF610 (as named by the landed 0x00844090: FindByName);
// CWorld virtual slot 13 PeekWorldMap() (+0x34), CWorldMap::GetLoadedRegion() 0x004FB150 (returns +0x9C; label
// "CWorld::DrawGetEnvironment" is wrong), CWorldMap::GetRegion(long) 0x004FC180.
// Retail offsets: CWorld CreatureGenerationEnabled +0xD4 (PDB +0xE0); CThing SerialisationSectionName +0x58 and
// PBaseDef +0x70 (PDB +0x68/+0x80: retail lists are 4 bytes); CRegion +0x55 flag, CThingBaseDef +0x42 bit 0 (unnamed).
// Shape: global generation on + the quest manager allowing it + the loaded region flagged and holding no active
// quest => yes; otherwise yes when the thing's section names a quest or its def carries the bit.
struct CCharStringData;
struct CCharString { CCharStringData* PStringData; };

struct CRegion {
    unsigned char _pad[0x55];
    bool          CreatureGenerationFlag;          // +0x55 (unnamed)
};

struct CQuestManager {
    bool IsCreatureGenerationEnabled() const;      // 0x004AFC60
    bool FindByName(const CCharString& name);      // 0x004AF610
    bool IsActiveQuestInRegion(const CRegion& region) const;   // 0x004AFDD0
};
extern CQuestManager* g_pQuestManager;             // 0x013B89FC

struct CWorldMap {
    long GetLoadedRegion() const;                  // 0x004FB150
    CRegion& GetRegion(long region);               // 0x004FC180
};

struct CThingBaseDef {
    unsigned char _pad[0x42];
    unsigned char CreatureGenerationBit : 1;       // +0x42 bit 0 (unnamed)
};

struct CThing {
    unsigned char  _pad[0x58];
    CCharString    SerialisationSectionName;       // +0x58 (PDB +0x68)
    unsigned char  _pad5c[0x14];
    CThingBaseDef* PBaseDef;                       // +0x70 (PDB +0x80, CDefPointer)
};

struct CWorld {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual CWorldMap* GetWorldMap();              // slot 12
    virtual CWorldMap* PeekWorldMap() const;       // slot 13 (+0x34)
    unsigned char _pad04[0xd0];
    bool          CreatureGenerationEnabled;       // +0xD4 (PDB +0xE0)
    bool IsCreatureGenerationEnabled(const CThing& thing) const;
};

bool CWorld::IsCreatureGenerationEnabled(const CThing& thing) const
{
    if (CreatureGenerationEnabled && g_pQuestManager->IsCreatureGenerationEnabled())
    {
        CRegion& region = PeekWorldMap()->GetRegion(PeekWorldMap()->GetLoadedRegion());
        if (region.CreatureGenerationFlag && !g_pQuestManager->IsActiveQuestInRegion(region))
            return true;
    }
    if (g_pQuestManager->FindByName(thing.SerialisationSectionName))
        return true;
    const CThingBaseDef* def = thing.PBaseDef;
    if (def && def->CreatureGenerationBit)
        return true;
    return false;
}
