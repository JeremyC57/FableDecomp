// CTCSoundAtmosphereVillage::FrameUpdate() -- when village sound is on and the world has a sound atmosphere manager: for every sound-group name of the village, look through every SOUND_THEME def that lists it and, when the village's level for that group is set, hand the theme's text name, the group and the level to the manager; then count the update timer down by one frame
// Grade: DIFFER(441v424) -- first-pass model (leaf FrameUpdate); this/iterator registers swap (retail this=ebx, group=edi), frame 4 bytes larger, the def pointer/name temporaries take different words. One shape tried.
// Retail: Fable.exe @ 0x007F53B0, 432 bytes (plain `ret`). Label correction: the catalog says vfunc_9; vtable 0x0124111C slot 9
//   is CTCBase::FrameUpdate (slot 9, as the landed CTCDoor::FrameUpdate).
// Callees: CTCBase::GetWorld 0x00686A70, CDefinitionManager::Get 0x0044C6B0, GetNumberOfDefsOfType 0x009AD3B0,
//   GetDef<CSoundThemeDef> 0x006B25E0, CCharString(const char*, long) 0x0099EBF0 / copy 0x0099EC30 / ~ 0x0099EAE0,
//   map<CCharString,long>::find 0x0042B467, CDefStringTable::GetString 0x009D49B0 on the table at 0x013CA828,
//   CSoundAtmosphereManager::SetThemeLevel 0x006B1900; own virtual slot 28 (+0x70, group level), def virtual slot 10 (+0x28).
// Layout (local model): +0x0c update timer, +0x10 village (sound-group names vector +0x28); world +0x24 atmosphere manager;
//   CSoundThemeDef group map +0x54; globals: sound enabled 0x013B8394, frame rate 0x01375550.
struct CCharString {
    void* m_data;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};
struct CDefString { long ID; CDefString(const CDefString& o) : ID(o.ID) {} CDefString() {} };
struct CDefStringTable { CCharString GetString(CDefString name); };   // 0x009D49B0
extern CDefStringTable g_DefStringTable;               // 0x013CA828
extern long g_SoundEnabled;                            // 0x013B8394
extern long g_FrameRate;                               // 0x01375550
struct CGroupMap {
    void* _M_header;
    void* find(const CCharString& key);                // 0x0042B467
};
struct CSoundThemeDef {
    virtual void v00(); virtual void Destroy(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual CDefString GetSymbol() const;              // slot 10 (+0x28)
    long RefCount;                                      // +0x04
    unsigned char _pad08[0x4c];
    CGroupMap Groups;                                   // +0x54
};
struct CSoundThemeDefPtr {
    CSoundThemeDef* Object;
    CSoundThemeDefPtr() : Object(0) {}
    ~CSoundThemeDefPtr() { if (--Object->RefCount == 0) Object->Destroy(); }
};
struct CDefinitionManager {
    static CDefinitionManager* Get();                   // 0x0044C6B0
    long GetNumberOfDefsOfType(const CCharString& type);   // 0x009AD3B0
    bool GetDef(const CCharString& type, long index, CSoundThemeDefPtr* out);   // 0x006B25E0
};
struct CSoundAtmosphereManager {
    void SetThemeLevel(const CCharString& theme, const CCharString& group, float level, long flags);   // 0x006B1900
};
struct CWorld {
    unsigned char _pad00[0x24];
    CSoundAtmosphereManager* AtmosphereManager;         // +0x24
};
struct CVillageSounds {
    unsigned char _pad00[0x28];
    CCharString* Groups_first;                          // +0x28
    CCharString* Groups_last;                           // +0x2c
};
struct CTCSoundAtmosphereVillage {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08();
    virtual void FrameUpdate();                         // slot 9
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21();
    virtual void v22(); virtual void v23(); virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27();
    virtual float GetGroupLevel(const CCharString& group);   // slot 28 (+0x70)
    void*   ParentThing;                                // +0x04
    unsigned char _pad08[4];
    float   UpdateTimer;                                // +0x0c
    CVillageSounds* Village;                            // +0x10
    CWorld* GetWorld();                                 // 0x00686A70
};

void CTCSoundAtmosphereVillage::FrameUpdate()
{
    if (g_SoundEnabled && GetWorld()->AtmosphereManager)
    {
        CSoundAtmosphereManager* manager = GetWorld()->AtmosphereManager;
        for (CCharString* group = Village->Groups_first; group != Village->Groups_last; ++group)
        {
            long count = CDefinitionManager::Get()->GetNumberOfDefsOfType("SOUND_THEME");
            for (long i = 1; i < count; ++i)
            {
                CSoundThemeDefPtr def;
                CDefinitionManager::Get()->GetDef("SOUND_THEME", i, &def);
                CSoundThemeDef* theme = def.Object;
                void* end = theme->Groups._M_header;
                if (theme->Groups.find(CCharString(*group)) != end)
                {
                    float level = GetGroupLevel(*group);
                    if (level != -1.0f)
                        manager->SetThemeLevel(g_DefStringTable.GetString(theme->GetSymbol()), *group, level, 0);
                }
            }
        }
    }
    UpdateTimer -= 1.0f / (float)g_FrameRate;
}
