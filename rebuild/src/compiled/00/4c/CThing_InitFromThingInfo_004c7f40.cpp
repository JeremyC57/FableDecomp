// CThing::InitFromThingInfo(const CThingInfo& info) -- copies the unique id and player number from the info record, resolves the def name through the definition manager (a "NULL" name means no def; an unknown name kills the thing and fails) and binds the base def pointer when an index was found
// Grade: RELOCATION_MATCH (retry 10, RTM 3077) -- the inline CCharString::operator==("NULL") hand-split: null-block strcmp and block compare as two tests that `goto` one out-of-line arm (DefGlobalIndex = 0), the lookup arm in line with its own early return
// Leaf: InitFromThingInfo
// Retail: Fable.exe (TLC, VC7.1) @ 0x004C7F40, 167 bytes (+9 int3). `ret 4`, bool in al.
// Callees: CBasicString<char>::operator==(const char*) 0x004115A0 on the name's data block (the null-data case is the
//          inline `repe cmpsb` of "" @ 0x0122D70E against "NULL" @ 0x012393E4), CDefinitionManager::GetDefGlobalIndexFromName
//          0x009AD410 and GetDef(long, CDefPointer<const CThingBaseDef>*) 0x0042AF3C (manifest template label propagated)
//          on g_pThingManager->DefinitionManager (+0x20, global 0x013B8A1C).
// Shape: the two id words copied first, the player byte next, the name's address kept in ebp; the found index is
//        stored as a 16-bit word and tested through ax (`jne` for the dead/fail exit, unsigned `jbe` for the GetDef guard);
//        Dead is bit 0 of the +0x91 flag byte (`or ..,1`).
// Layout: retail CThingInfo: UniqueID +0x00 (8 bytes), DefName +0x08 (CCharString), Player +0x0C; CThing (Ego_r.pdb
//         minus 0x10 from UsingComponents on): UniqueID +0x18, PBaseDef +0x70, DefGlobalIndex +0x8C, PlayerNumber +0x90,
//         Dead +0x91 bit 0.
#pragma pack(push, 1)
struct CBasicStringChar {                             // CBasicString<char>, 13 bytes
    char*         PData;
    unsigned long StringLength;
    unsigned long DataLength : 31;
    bool          UseFastExtend : 1;
    bool operator==(const char* text) const;          // 0x004115A0
};
class CCharStringData { public:                              // 17 bytes
    CBasicStringChar Data;
    long             NoRefs;
};
#pragma pack(pop)
extern "C" int __cdecl strcmp(const char* a, const char* b);
class CCharString {
public:
    CCharStringData* PStringData;
};
class CThingBaseDef;
struct CDefPointer { const CThingBaseDef* Object; };  // CDefPointer<const CThingBaseDef>
class CDefinitionManager { public:
    long GetDefGlobalIndexFromName(const CCharString& name);   // 0x009AD410
    bool GetDef(long index, CDefPointer* out);        // 0x0042AF3C
};
class CThingManager { public:
    unsigned char _pad00[0x20];
    CDefinitionManager* DefinitionManager;            // +0x20 (PDB +0x2c)
};
extern CThingManager* g_pThingManager;                // 0x013B8A1C
class CThingInfo { public:
    unsigned __int64 UniqueID;                        // +0x00
    CCharString      DefName;                         // +0x08
    signed char      Player;                          // +0x0c
};
class CThing { public:
    unsigned char    _pad00[0x18];
    unsigned __int64 UniqueID;                        // +0x18
    unsigned char    _pad20[0x70 - 0x20];
    CDefPointer      PBaseDef;                        // +0x70
    unsigned char    _pad74[0x8c - 0x74];
    unsigned short   DefGlobalIndex;                  // +0x8c
    short            CurrentMapNumber;                // +0x8e
    signed char      PlayerNumber;                    // +0x90
    unsigned char    Dead : 1;                        // +0x91 bit 0
    unsigned char    CreatedByOwnerThingCreation : 1;
    unsigned char    IncludeInThingScans : 1;
    unsigned char    Activated : 1;
    unsigned char    GloballyLoaded : 1;
    unsigned char    InLimbo : 1;
    unsigned char    KillLocked : 1;
    unsigned char    CreatedBySerialisation : 1;
    bool InitFromThingInfo(const CThingInfo& info);
};
bool CThing::InitFromThingInfo(const CThingInfo& info)
{
    UniqueID = info.UniqueID;
    PlayerNumber = info.Player;
    if (info.DefName.PStringData == 0) {              // CCharString::operator==(const char*) hand-split: the two tests join the same arm
        if (strcmp("NULL", "") == 0)
            goto null_name;
    } else if (info.DefName.PStringData->Data == "NULL") {
        goto null_name;
    }
    DefGlobalIndex = (unsigned short)g_pThingManager->DefinitionManager->GetDefGlobalIndexFromName(info.DefName);
    if (DefGlobalIndex == 0) {
        Dead = true;
        return false;
    }
    goto lookup;
null_name:
    DefGlobalIndex = 0;
lookup:
    if (DefGlobalIndex > 0)
        g_pThingManager->DefinitionManager->GetDef(DefGlobalIndex, &PBaseDef);
    return true;
}