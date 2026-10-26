// CThing::ConstructFromParams(long, long, long, bool) -- base thing construction: re-register with the class factory, store def index / player / map, reset the lists, fetch the base def, allocate the UID, apply the def's default script name/data and add the editor and map-who components
// Leaf: ConstructFromParams
// Retail: Fable.exe (TLC, VC7.1) @ 0x004CA010, 437 bytes, `ret 10h`. Called by CThingPhysical::ConstructFromParams 0x006A5950.
// Callees: CClassFactory::ReregisterAllocatedClass(CClass*, unsigned long, bool) 0x00A37300 and the thing-type flags
// getter 0x0051E580 on the thing manager 0x013B8A1C (a CClassFactory), GFGetCurrentFrame 0x0049D870, CCharString copy
// ctor 0x0099EC30 (from the global CCharString 0x013BAB0C) / operator= 0x0099EFB0 / dtor 0x0099EAE0 / (const char*, long)
// ctor 0x0099EBF0, CDefinitionManager::GetDef<CThingBaseDef> 0x0042AF3C (manager +0x20), CThing::CreateUID 0x004C7FF0,
// the thing's virtual slot 15 (+0x3C, the base-def hook), CDefStringTable::GetString(long) 0x009D49B0 on the global table
// 0x013CA828, the game component's virtual slot 9 (+0x24, manager +0x1C), CThing::AddTC(const CCharString&, bool,
// CTCParamsBase*) 0x004C9D60 with "CTCEditor" 0x01239BF0 and "CTCMapwho" 0x01239BE4.
// Layout (retail CThing, PDB offsets 0x10 higher from +0x58): thing class byte +0x10, SerialisationSectionName +0x58,
// WorldFrameCreatedAt +0x68, ThingTypeFlags +0x6C, PBaseDef +0x70, ScriptName +0x74, ScriptData +0x78, the render and
// update list links +0x7C..+0x88, DefGlobalIndex +0x8C, CurrentMapNumber +0x8E, PlayerNumber +0x90 (signed char),
// LevelPersistent bit 4 of +0x92. CThingBaseDef (retail = PDB - 4): DefaultScriptName +0x68, DefaultScriptData +0x6C.
// Thing manager (retail): game component +0x1C, definition manager +0x20.
// Shape: every CCharString temporary is homed in the dead bool argument slot; the section name is assigned from an
// inline by-value copy of the global (bare temporary); the two GetString results and the two AddTC names are NAMED
// locals in their own blocks (retail re-derives `lea [esp+18h]` instead of reusing eax, and builds the AddTC name
// before pushing the two zero arguments); the player-number clamp keeps its redundant `== 5` test.
class CTCBase;
class CTCParamsBase;

class CCharString { public:
    void* m_pStringData;
    CCharString(const char* text, long length);      // 0x0099EBF0
    CCharString(const CCharString& other);           // 0x0099EC30
    ~CCharString();                                  // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

extern CCharString g_DefaultSerialisationSectionName;   // 0x013BAB0C
inline CCharString GetDefaultSerialisationSectionName() { return g_DefaultSerialisationSectionName; }

class CThingBaseDef { public:
    unsigned char _pad00[0x68];
    long DefaultScriptName;                          // +0x68 (CDefString)
    long DefaultScriptData;                          // +0x6C (CDefString)
};

struct CThingBaseDefPointer {                        // CDefPointer<const CThingBaseDef>
    CThingBaseDef* m_p;
};

struct CDefStringTable {
    CCharString GetString(long index);               // 0x009D49B0
};
extern CDefStringTable g_DefStringTable;             // 0x013CA828

class CGameDefinitionManager { public:
    bool GetDef(long globalIndex, CThingBaseDefPointer* out);   // 0x0042AF3C
};

class CMainGameComponent { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual bool IsEditorRunning();                  // slot 9 (+0x24)
};

class CThing;
class CThingManager { public:                               // a CClassFactory
    unsigned char           _pad00[0x1c];
    CMainGameComponent*     Component;               // +0x1C
    CGameDefinitionManager* DefinitionManager;       // +0x20
    void ReregisterAllocatedClass(CThing* thing, unsigned long type, bool local);   // 0x00A37300
    unsigned long GetThingTypeFlags(long thingClass);                              // 0x0051E580
};
extern CThingManager* g_pThingManager;               // 0x013B8A1C

long __fastcall GFGetCurrentFrame();                 // 0x0049D870

class CThing {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void OnBaseDefSet(CThingBaseDef* def);   // slot 15 (+0x3C)

    unsigned char        _pad04[0x10 - 4];
    unsigned char        ThingClass;                 // +0x10
    unsigned char        _pad11[0x58 - 0x11];
    CCharString          SerialisationSectionName;   // +0x58
    unsigned char        _pad5c[0x68 - 0x5c];
    long                 WorldFrameCreatedAt;        // +0x68
    unsigned long        ThingTypeFlags;             // +0x6C
    CThingBaseDefPointer PBaseDef;                   // +0x70
    CCharString          ScriptName;                 // +0x74
    CCharString          ScriptData;                 // +0x78
    CThing*              RenderListPrev;             // +0x7C
    CThing*              RenderListNext;             // +0x80
    CThing*              UpdateListPrev;             // +0x84
    CThing*              UpdateListNext;             // +0x88
    unsigned short       DefGlobalIndex;             // +0x8C
    short                CurrentMapNumber;           // +0x8E
    signed char          PlayerNumber;               // +0x90
    unsigned char        _flags91;                   // +0x91 (Dead .. CreatedBySerialisation)
    unsigned char        DeletedOnOwnerLevelOutOfScope : 1;   // +0x92 bit 0
    unsigned char        Drawable : 1;               // bit 1
    unsigned char        Dying : 1;                  // bit 2
    unsigned char        ForceFrameUpdate : 1;       // bit 3
    unsigned char        LevelPersistent : 1;        // bit 4

    void CreateUID();                                // 0x004C7FF0
    CTCBase* AddTC(const CCharString& name, bool activate, CTCParamsBase* params);   // 0x004C9D60
    bool ConstructFromParams(long defGlobalIndex, long playerNumber, long mapNumber, bool reregister);
};

bool CThing::ConstructFromParams(long defGlobalIndex, long playerNumber, long mapNumber, bool reregister)
{
    if (reregister)
        g_pThingManager->ReregisterAllocatedClass(this, ThingClass, reregister);
    DefGlobalIndex = (unsigned short)defGlobalIndex;
    PlayerNumber = (signed char)playerNumber;
    CurrentMapNumber = (short)mapNumber;
    WorldFrameCreatedAt = GFGetCurrentFrame();
    RenderListNext = 0;
    RenderListPrev = 0;
    UpdateListNext = 0;
    UpdateListPrev = 0;
    SerialisationSectionName = GetDefaultSerialisationSectionName();
    LevelPersistent = false;
    if (DefGlobalIndex > 0)
        g_pThingManager->DefinitionManager->GetDef(DefGlobalIndex, &PBaseDef);
    CreateUID();
    ThingTypeFlags = g_pThingManager->GetThingTypeFlags(ThingClass);
    if (PlayerNumber == 5 || PlayerNumber < 0 || PlayerNumber >= 4)
        PlayerNumber = 4;
    if (PBaseDef.m_p) {
        OnBaseDefSet(PBaseDef.m_p);
        {
            CCharString name = g_DefStringTable.GetString(PBaseDef.m_p->DefaultScriptName);
            ScriptName = name;
        }
        {
            CCharString data = g_DefStringTable.GetString(PBaseDef.m_p->DefaultScriptData);
            ScriptData = data;
        }
    }
    if (g_pThingManager->Component->IsEditorRunning()) {
        CCharString editor("CTCEditor", -1);
        AddTC(editor, false, 0);
    }
    {
        CCharString mapwho("CTCMapwho", -1);
        AddTC(mapwho, false, 0);
    }
    return true;
}