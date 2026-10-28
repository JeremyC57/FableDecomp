// CPlayerManager::SaveGameState(CPersistContext& context) -- persist the main player's character as a CPlayerSave: its unique ID and the name of the region whose map holds the character
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00449F90, length 224 bytes, `ret 4`.
// Identity: Ego_r.pdb `?SaveGameState@CPlayerManager@@QAEXAAVCPersistContext@@@Z`.
// Callees: CPlayerManager::GetPlayer(long) 0x004498C0, CBaseIntelligentPointer::GetPItem 0x00A01B50, CCharString default
// ctor 0x0099E4B0 / copy ctor 0x0099EC30 / operator= 0x0099EFB0 / dtor 0x0099EAE0, CThing::GetPos 0x004C73D0,
// CWorld::PeekWorldMap (slot 13, +0x34), CWorldMap::GetMapNumberAt(const C3DVector&) (slot 16, +0x40),
// CWorldMap::GetRegionNumberMapIsIn(long) 0x004FC190, CWorldMap::GetRegion(long) 0x004FC180,
// CPersistContext::Transfer(const char*, unsigned __int64&, const unsigned __int64&) 0x0044BB00 and
// Transfer(const char*, CCharString&, const CCharString&) 0x004109A0 (both catalogued under propagated
// "Transfer<...>" names).
// Layout: Component +0x04 (its World at +0x24), MainPlayer +0x1C (engine/CPlayerManager.h: +0x20, PDB 16-byte
// vector); CPlayer::PlayerCharacter +0x34; CThing::UniqueID +0x18; CRegion::Name +0x18; NPlayerManager::CPlayerSave
// (Ego_r.pdb) {CharacterUID ulong64 +0, RegionName CCharString +8}.
// Shape: the character is read through an inline CPlayer accessor (`lea ecx,[eax+34h]`); the save record is a frame local (its CCharString default-constructed first, the UID copied in after);
// the two defaults are const-reference temporaries sharing one frame word pair: a literal 0 widened to ulong64, then
// a CCharString from an inline by-value getter (its named local built in the dead `context` argument slot, copied
// out and destroyed: VC7.1 has no NRVO).
class C3DVector;
class CCharString { public:
    void* PStringData;
    CCharString();                                      // 0x0099E4B0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};
inline CCharString EmptyString()
{
    CCharString empty;
    return empty;
}

class CPersistContext { public:
    void Transfer(const char* name, unsigned __int64& value, const unsigned __int64& defaultValue);   // 0x0044BB00
    void Transfer(const char* name, CCharString& value, const CCharString& defaultValue);             // 0x004109A0
};

class CThing { public:
    unsigned char    _pad00[0x18];
    unsigned __int64 UniqueID;                          // +0x18
    const C3DVector& GetPos() const;               // 0x004C73D0
};
class CBaseObject;
class CBaseIntelligentPointer {
public:
    virtual void v00();
    CBaseObject* PItem;                                 // +0x04
    CBaseObject* GetPItem() const;                      // 0x00A01B50
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};
class CThingPlayerCreature : public CThing { public: };

class CRegion { public:
    unsigned char _pad00[0x18];
    CCharString   Name;                                 // +0x18
};
class CWorldMap { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15();
    virtual long GetMapNumberAt(const C3DVector& position) const;   // slot 16 (+0x40)
    long GetRegionNumberMapIsIn(long mapNumber) const;  // 0x004FC190
    CRegion& GetRegion(long region);                    // 0x004FC180
};
class CWorld { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12();
    virtual CWorldMap* PeekWorldMap();                  // slot 13 (+0x34)
};
class CMainGameComponent { public:
    unsigned char _pad00[0x24];
    CWorld*       PWorld;                               // +0x24 scoped_ptr<CWorld>
};

class CPlayer { public:
    unsigned char _pad00[0x34];
    CIntelligentPointer<CThingPlayerCreature> PlayerCharacter;   // +0x34
    CThingPlayerCreature* GetPlayerCharacter() const { return PlayerCharacter.GetPItem(); }
};

namespace NPlayerManager {
class CPlayerSave { public:
    unsigned __int64 CharacterUID;                      // +0x00
    CCharString      RegionName;                        // +0x08
};
}

class CPlayerManager { public:
    void*               __vftable;
    CMainGameComponent* Component;                      // +0x04
    void*               DefinitionManager;              // +0x08
    unsigned char       Players[0x0c];                  // +0x0c vector<CPlayer*>
    long                PlayerNeutral;                  // +0x18
    long                MainPlayer;                     // +0x1c
    CPlayer* GetPlayer(long number);                    // 0x004498C0
    void SaveGameState(CPersistContext& context);
};

void CPlayerManager::SaveGameState(CPersistContext& context)
{
    CThing* character = GetPlayer(MainPlayer)->GetPlayerCharacter();
    NPlayerManager::CPlayerSave save;
    save.CharacterUID = character->UniqueID;
    const C3DVector& position = character->GetPos();
    CWorldMap* worldMap = Component->PWorld->PeekWorldMap();
    save.RegionName = worldMap->GetRegion(worldMap->GetRegionNumberMapIsIn(worldMap->GetMapNumberAt(position))).Name;
    context.Transfer("PlayerCharacterUID", save.CharacterUID, 0);
    context.Transfer("PlayerRegionName", save.RegionName, EmptyString());
}