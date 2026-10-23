// Grade: DIFFER(609v607) -- register/slot allocation only: retail keeps context in edi and the zero constant / enemy-def pointer in ebp (ours swaps them) and homes the enemy-def out slot, the differs bool and the load-branch name in the dead context argument slot (frame 0x14 v 0x18)
// CTCEnemy::OnSerialise(CPersistContext*, long) -- persists the two flags and, when it differs from the def's faction, the faction name
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0076C9A0, 608 bytes, `ret 8`.
// Callees: CPersistContext::Transfer<signed char> @ 0x004045C0, CPersistContext::Transfer<CCharString> @ 0x004109A0,
//          CTCBase::GetDefinitions() const @ 0x006869D0, CDefinitionManager::GetDef<CThingBaseDef> @ 0x0042AF3C,
//          CDefClassBase::GetSubDef<CEnemyDef> @ 0x006C7720, CDefinitionManager::GetDefNameFromGlobalIndex @ 0x009ACCC0,
//          CDefString::operator!=(const CCharString&) @ 0x0076D640, CTCBase::GetWorld() @ 0x00686A80,
//          CFactionManager::FindFaction(const CCharString&) @ 0x006B93D0, CCharString ctors/dtor/operator=/operator!=.
// Layout: retail CTCEnemy: FriendsWithEverythingFlag +0x0C, EnableFollowersEnemyProxy +0x0D, two 4-byte STLport lists
//         +0x10/+0x14, PFaction +0x18 (PDB +0x28: its lists are 12 bytes); CFaction's name is its first member.
class CCharString {
public:
    void* PStringData;
    CCharString();                                  // 0x0099E4B0
    CCharString(const char* s, long len = -1);      // 0x0099EBF0
    CCharString(const CCharString& other);          // 0x0099EC30
    ~CCharString();                                 // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
    bool operator!=(const char* s) const;           // 0x0099E960
};

// The empty default comes from an inline by-value helper over a named local (no NRVO in VC7.1: the local is
// built, copied into the result temporary and destroyed before the call).
inline CCharString EmptyString()
{
    CCharString empty;
    return empty;
}

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
template<class T> class CDefPointer {
public:
    T* Object;
    CDefPointer() : Object(0) {}
    T* operator->() const { return Object; }
};
class CEnemyDef : public CDefClassBase {
public:
    unsigned char _pad08[0x28 - 0x08];
    long FactionGlobalIndex;                        // +0x28
};
class CThingBaseDef : public CDefClassBase {
public:
    bool GetSubDef(CDefPointer<const CEnemyDef>* out) const;   // 0x006C7720
};
class CDefString {
public:
    void* PEntry;
    bool operator!=(const CCharString& s) const;    // 0x0076D640
};
class CDefinitionManager {
public:
    bool GetDef(long index, CDefPointer<const CThingBaseDef>* out) const;   // 0x0042AF3C
    CDefString GetDefNameFromGlobalIndex(long index) const;               // 0x009ACCC0
};

class CThing {
public:
    unsigned char _pad00[0x8c];
    unsigned short DefGlobalIndex;                  // +0x8C
};
class CFaction {
public:
    CCharString Name;                               // +0x00
};
class CFactionManager {
public:
    CFaction* FindFaction(const CCharString& name); // 0x006B93D0
};
class CWorld {
public:
    unsigned char _pad00[0x54];
    CFactionManager* PFactionManager;               // +0x54
};

class CPersistContext {
public:
    unsigned char _pad00[0x18];
    long Mode;                                      // +0x18 (1 / 3 = saving)
    bool IsSaving() const { return Mode == 1 || Mode == 3; }
    void Transfer(const char* name, bool& value, const bool& defaultValue);                  // 0x004045C0
    void Transfer(const char* name, CCharString& value, const CCharString& defaultValue);    // 0x004109A0
};

class CTCEnemy {
public:
    virtual void v00();
    CThing* Thing;                                  // +0x04
    unsigned char _pad08[4];
    bool FriendsWithEverythingFlag;                 // +0x0C
    bool EnableFollowersEnemyProxy;                 // +0x0D
    unsigned char _pad0e[2];
    void* EnemyThings;                              // +0x10
    void* AllyThings;                               // +0x14
    CFaction* PFaction;                             // +0x18
    const CDefinitionManager* GetDefinitions() const;   // 0x006869D0
    CWorld* GetWorld() const;                       // 0x00686A80
    void OnSerialise(CPersistContext* context, long version);
};

void CTCEnemy::OnSerialise(CPersistContext* context, long)
{
    context->Transfer("FriendsWithEverythingFlag", FriendsWithEverythingFlag, false);
    context->Transfer("EnableFollowersEnemyProxy", EnableFollowersEnemyProxy, true);
    if (context->IsSaving()) {
        CDefPointer<const CThingBaseDef> thingDef;
        GetDefinitions()->GetDef(Thing->DefGlobalIndex, &thingDef);
        const CEnemyDef* enemyDef = 0;
        {
            CCharString factionName;
            CDefPointer<const CEnemyDef> out;
            bool differs = false;
            if (PFaction) {
                bool found = thingDef->GetSubDef(&out);
                enemyDef = out.Object;
                if (found && GetDefinitions()->GetDefNameFromGlobalIndex(enemyDef->FactionGlobalIndex) != CCharString(PFaction->Name))
                    differs = true;
            }
            if (differs)
                factionName = PFaction ? CCharString(PFaction->Name) : CCharString("");
            context->Transfer("FactionName", factionName, EmptyString());
        }
        if (enemyDef && --enemyDef->RefCount == 0)
            enemyDef->Destroy();
        if (thingDef.Object && --thingDef.Object->RefCount == 0)
            thingDef.Object->Destroy();
    } else {
        CCharString factionName;
        context->Transfer("FactionName", factionName, EmptyString());
        if (factionName != "")
        {
            CFactionManager* factions = GetWorld()->PFactionManager;
            PFaction = factions->FindFaction(factionName);
        }
    }
}
