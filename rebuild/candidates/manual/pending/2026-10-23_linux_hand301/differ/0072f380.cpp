// Grade: DIFFER(1002v968) -- register allocation: retail keeps context in ebp and the UID address in edi with ebx=0; ours puts context in edi and the UID address in ebp, then runs out of registers and spills Thing ([esp+0x24]) in each RemapUIDForSerialisation argument list (+34 bytes); call/temporary structure (GetString temp copied into the name, EmptyString default) matches
// CTCDummyVillager::OnSerialise(CPersistContext*, long) -- persists the def type by name, the home/work building and creature UIDs (remapped for serialisation) and the recreate/respawn state
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0072F380, 976 bytes, `ret 8`.
// Callees: CTCBase::GetDefinitions() const @ 0x006869D0, CDefinitionManager::GetDefNameFromGlobalIndex @ 0x009ACCC0,
//          CDefinitionManager::GetDefGlobalIndexFromName @ 0x009AD410, CDefStringTable::GetString @ 0x009D49B0 (table
//          0x013CA828), CBaseIntelligentPointer::GetPItem() const @ 0x00A01B50, CThing::IsLevelLess() const @ 0x004C7930,
//          CTCBase::GetThingManager() const @ 0x00686AE0, CThingManager::RemapUIDForSerialisation @ 0x0051E210,
//          CPersistContext::Transfer<CCharString> @ 0x004109A0, <unsigned __int64> @ 0x0044BB00, <long> @ 0x00410BE0,
//          <signed char> @ 0x004045C0, CCharString ctors/dtor.
// Layout (Ego_r.pdb CTCDummyVillager, matches retail): DefType +0x0C, HomeBuilding +0x10 / HomeBuildingUID +0x18,
//         WorkBuilding +0x20 / WorkBuildingUID +0x28, Creature +0x30 / CreatureUID +0x38, DayToBeRecreated +0x40,
//         FrameToBeRecreated +0x44, Respawnable +0x48, Guard +0x49.
class CCharString {
public:
    void* PStringData;
    CCharString();                                  // 0x0099E4B0
    CCharString(const CCharString& other);          // 0x0099EC30
    ~CCharString();                                 // 0x0099EAE0
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
};
class CDefStringTable {
public:
    CCharString GetString(long index);              // 0x009D49B0
};
extern CDefStringTable g_DefStringTable;            // 0x013CA828

class CDefinitionManager {
public:
    CDefString GetDefNameFromGlobalIndex(long index) const;     // 0x009ACCC0
    long GetDefGlobalIndexFromName(const CCharString& name) const;   // 0x009AD410
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
class CBaseIntelligentPointer {
public:
    void* _vfptr;
    void* PData;
    CThing* GetPItem() const;                       // 0x00A01B50
};

class CPersistContext {
public:
    unsigned char _pad00[0x18];
    long Mode;                                      // +0x18
    bool IsSaving() const { return Mode == 1 || Mode == 3; }
    void Transfer(const char* name, unsigned __int64& value, const unsigned __int64& defaultValue);   // 0x0044BB00
    void Transfer(const char* name, CCharString& value, const CCharString& defaultValue);            // 0x004109A0
    void Transfer(const char* name, long& value, const long& defaultValue);                          // 0x00410BE0
    void Transfer(const char* name, bool& value, const bool& defaultValue);                          // 0x004045C0
};

class CTCBase {
public:
    virtual void v00();
    CThing* Thing;                                  // +0x04
    unsigned char _pad08[4];
    const CDefinitionManager* GetDefinitions() const;   // 0x006869D0
    CThingManager* GetThingManager() const;         // 0x00686AE0
    // Inline by-value name lookup over a named local: the string table's result is built in a temporary, copied
    // into the caller's object and destroyed (no NRVO in VC7.1).
    CCharString GetDefNameString(long index) const
    {
        CCharString name = g_DefStringTable.GetString(GetDefinitions()->GetDefNameFromGlobalIndex(index).Index);
        return name;
    }
};

class CTCDummyVillager : public CTCBase {
public:
    unsigned long DefType;                          // +0x0C
    CBaseIntelligentPointer HomeBuilding;           // +0x10
    unsigned __int64 HomeBuildingUID;               // +0x18
    CBaseIntelligentPointer WorkBuilding;           // +0x20
    unsigned __int64 WorkBuildingUID;               // +0x28
    CBaseIntelligentPointer Creature;               // +0x30
    unsigned __int64 CreatureUID;                   // +0x38
    long DayToBeRecreated;                          // +0x40
    long FrameToBeRecreated;                        // +0x44
    bool Respawnable;                               // +0x48
    bool Guard;                                     // +0x49
    void OnSerialise(CPersistContext* context, long version);
};

void CTCDummyVillager::OnSerialise(CPersistContext* context, long)
{
    if (context->IsSaving()) {
        CCharString defName = GetDefNameString(DefType);
        context->Transfer("DefType", defName, EmptyString());
        HomeBuildingUID = 0;
        if (HomeBuilding.GetPItem() && !(HomeBuilding.GetPItem()->Flags91 & 1))
            HomeBuildingUID = GetThingManager()->RemapUIDForSerialisation(true, Thing->IsLevelLess(), Thing->LevelIndex, HomeBuilding.GetPItem()->UID);
        context->Transfer("HomeBuildingUID", HomeBuildingUID, 0);
        WorkBuildingUID = 0;
        if (WorkBuilding.GetPItem() && !(WorkBuilding.GetPItem()->Flags91 & 1))
            WorkBuildingUID = GetThingManager()->RemapUIDForSerialisation(true, Thing->IsLevelLess(), Thing->LevelIndex, WorkBuilding.GetPItem()->UID);
        context->Transfer("WorkBuildingUID", WorkBuildingUID, 0);
        if (Creature.GetPItem() && !(Creature.GetPItem()->Flags91 & 1))
            CreatureUID = GetThingManager()->RemapUIDForSerialisation(true, Thing->IsLevelLess(), Thing->LevelIndex, Creature.GetPItem()->UID);
        context->Transfer("CreatureUID", CreatureUID, 0);
    } else {
        CCharString defName;
        context->Transfer("DefType", defName, EmptyString());
        DefType = GetDefinitions()->GetDefGlobalIndexFromName(defName);
        context->Transfer("HomeBuildingUID", HomeBuildingUID, 0);
        if (HomeBuildingUID)
            HomeBuildingUID = GetThingManager()->RemapUIDForSerialisation(false, Thing->IsLevelLess(), Thing->LevelIndex, HomeBuildingUID);
        context->Transfer("WorkBuildingUID", WorkBuildingUID, 0);
        if (WorkBuildingUID)
            WorkBuildingUID = GetThingManager()->RemapUIDForSerialisation(false, Thing->IsLevelLess(), Thing->LevelIndex, WorkBuildingUID);
        context->Transfer("CreatureUID", CreatureUID, 0);
        if (CreatureUID)
            CreatureUID = GetThingManager()->RemapUIDForSerialisation(false, Thing->IsLevelLess(), Thing->LevelIndex, CreatureUID);
    }
    context->Transfer("DayToBeRecreated", DayToBeRecreated, 0);
    context->Transfer("FrameToBeRecreated", FrameToBeRecreated, 0);
    context->Transfer("Respawnable", Respawnable, true);
    context->Transfer("Guard", Guard, false);
}
