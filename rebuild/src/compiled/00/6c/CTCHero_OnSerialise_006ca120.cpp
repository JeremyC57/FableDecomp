// CTCHero::OnSerialise(CPersistContext*, long) -- persists the last equipped weapon's remapped UID and the hero title def by name
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006CA120, 608 bytes, `ret 8`.
// Callees: CBaseIntelligentPointer::GetPItem() const @ 0x00A01B50, CThing::IsLevelLess() const @ 0x004C7930,
//          CTCBase::GetThingManager() const @ 0x00686AE0, CThingManager::RemapUIDForSerialisation @ 0x0051E210,
//          CPersistContext::Transfer<unsigned __int64> @ 0x0044BB00, CPersistContext::Transfer<CCharString> @ 0x004109A0,
//          CTCBase::GetDefinitionManager() const @ 0x006869D0, CDefinitionManager::GetPDefFromGlobalIndex @ 0x009AD9E0,
//          CDefinitionManager::GetDefGlobalIndexFromName @ 0x009AD410, CDefStringTable::GetString @ 0x009D49B0 (table
//          object 0x013CA828), CCharString ctors/dtor/operator=.
// Layout (Ego_r.pdb CTCHero, matches retail): HeroTitleObjectDef +0x14, PHeroTitleSubDef +0x18, LastWeaponEquipped
//         +0x1D (an unaligned intelligent pointer), LastWeaponEquippedID +0x28 (unsigned __int64).
class CCharString {
public:
    void* PStringData;
    CCharString();                                  // 0x0099E4B0
    CCharString(const char* s, long len = -1);      // 0x0099EBF0
    CCharString(const CCharString& other);          // 0x0099EC30
    ~CCharString();                                 // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};
// The empty default comes from an inline by-value helper over a named local (no NRVO in VC7.1).
inline CCharString EmptyString()
{
    CCharString empty;
    return empty;
}

class CDefString {
public:
    long Index;
    CDefString() : Index(0) {}
};
class CDefStringTable {
public:
    CCharString GetString(long index);              // 0x009D49B0
};
extern CDefStringTable g_DefStringTable;            // 0x013CA828

class CDefClassBase {
public:
    virtual void v00();
    virtual void Destroy() const;                   // slot 1
    mutable long RefCount;                          // +0x04
    void Release() const
    {
        if (--RefCount == 0)
            Destroy();
    }
};
class CDefinition : public CDefClassBase {
public:
    virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06();
    virtual void v07(); virtual void v08(); virtual void v09();
    virtual CDefString GetDefName() const;          // slot 10 (+0x28)
};
class CHeroTitleDef : public CDefinition { };
template<class T> class CDefPointer {
public:
    T* Object;
    CDefPointer() : Object(0) {}
    ~CDefPointer()
    {
        if (Object) {
            Object->Release();
            Object = 0;
        }
    }
    T* operator->() const { return Object; }
    CDefPointer& operator=(T* p)
    {
        if (Object) {
            Object->Release();
            Object = 0;
        }
        Object = p;
        return *this;
    }
};
class CDefinitionManager {
public:
    CDefPointer<const CDefinition> GetPDefFromGlobalIndex(long index) const;   // 0x009AD9E0
    long GetDefGlobalIndexFromName(const CCharString& name) const;                       // 0x009AD410
};
class CGameDefinitionManager : public CDefinitionManager
{
};

class CThing {
public:
    unsigned char _pad00[0x18];
    unsigned __int64 UID;                           // +0x18
    unsigned char _pad20[0x8e - 0x20];
    short LevelIndex;                               // +0x8E
    unsigned char _pad90;
    unsigned char Flags91;                          // +0x91 (bit 0: not saved)
    bool IsLevelLess() const;                       // 0x004C7930
};
class CThingManager {
public:
    unsigned __int64 RemapUIDForSerialisation(bool saving, bool levelLess, long level, unsigned __int64 uid);   // 0x0051E210
};
// CIntelligentPointer<CThing> sits unaligned at +0x1D in retail; modelled as an opaque byte block.
class CBaseIntelligentPointer {
public:
    unsigned char Storage[8];
    CThing* GetPItem() const;                       // 0x00A01B50
};

class CPersistContext {
public:
    unsigned char _pad00[0x18];
    long Mode;                                      // +0x18
    bool IsSaving() const { return Mode == 1 || Mode == 3; }
    void Transfer(const char* name, unsigned __int64& value, const unsigned __int64& defaultValue);   // 0x0044BB00
    void Transfer(const char* name, CCharString& value, const CCharString& defaultValue);            // 0x004109A0
};

class CTCBase {
public:
    virtual void v00();
    CThing* Thing;                                  // +0x04
    unsigned char _pad08[4];
protected:
    const CGameDefinitionManager& GetDefinitionManager() const;   // 0x006869D0
    CThingManager& GetThingManager();   // 0x00686AE0
};

class CTCHero : public CTCBase {
public:
    bool WeaponsUsable;                             // +0x0C
    bool WillUsable;                                // +0x0D
    bool WeaponUseOverriddenByScripts;              // +0x0E
    bool WillUseOverriddenByScripts;                // +0x0F
    long Title;                                     // +0x10
    long HeroTitleObjectDef;                        // +0x14
    CDefPointer<const CHeroTitleDef> PHeroTitleSubDef;   // +0x18
    bool OnBoastingPodium;                          // +0x1C
    CBaseIntelligentPointer LastWeaponEquipped;     // +0x1D
    unsigned char _pad25[3];
    unsigned __int64 LastWeaponEquippedID;          // +0x28
    void OnSerialise(CPersistContext* context, long version);
};

void CTCHero::OnSerialise(CPersistContext* context, long)
{
    if (context->IsSaving()) {
        LastWeaponEquippedID = 0;
        if (LastWeaponEquipped.GetPItem() && !(LastWeaponEquipped.GetPItem()->Flags91 & 1)) {
            CThing* weapon = LastWeaponEquipped.GetPItem();
            unsigned __int64 uid = weapon->UID;
            LastWeaponEquippedID = GetThingManager().RemapUIDForSerialisation(true, Thing->IsLevelLess(), Thing->LevelIndex, uid);
        }
        context->Transfer("LastWeaponEquippedID", LastWeaponEquippedID, 0);
        CCharString defName("");
        CDefPointer<const CDefinition> def = GetDefinitionManager().GetPDefFromGlobalIndex(HeroTitleObjectDef);
        if (def.Object)
        {
            CCharString name = g_DefStringTable.GetString(def->GetDefName().Index);
            defName = name;
        }
        context->Transfer("hero_title_object_def_name", defName, EmptyString());
    } else {
        context->Transfer("LastWeaponEquippedID", LastWeaponEquippedID, 0);
        if (LastWeaponEquippedID)
            LastWeaponEquippedID = GetThingManager().RemapUIDForSerialisation(false, Thing->IsLevelLess(), Thing->LevelIndex, LastWeaponEquippedID);
        CCharString defName("");
        context->Transfer("hero_title_object_def_name", defName, EmptyString());
        HeroTitleObjectDef = GetDefinitionManager().GetDefGlobalIndexFromName(defName);
        PHeroTitleSubDef = 0;
    }
}