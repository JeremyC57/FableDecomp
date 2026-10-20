// CThing::InitFromThingInfo(const CThingInfo& info) -- copies the unique id and player number from the info record, resolves the def name through the definition manager (a "NULL" name means no def; an unknown name kills the thing and fails) and binds the base def pointer when an index was found
// Grade: DIFFER(ours v retail) -- 175v167: the inlined CCharString::operator==("NULL") materialises a bool (`sete al ; jmp ; test al,al`) where retail branches straight off `repe cmpsb` / the out-of-line result and lays the out-of-line arm after the fail exit
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
struct CCharStringData {                              // 17 bytes
    CBasicStringChar Data;
    long             NoRefs;
};
#pragma pack(pop)
extern "C" int __cdecl strcmp(const char* a, const char* b);
class CCharString {
public:
    CCharStringData* PStringData;
};
struct CThingBaseDef;
struct CDefPointer { const CThingBaseDef* Object; };  // CDefPointer<const CThingBaseDef>
struct CDefinitionManager {
    long GetDefGlobalIndexFromName(const CCharString& name);   // 0x009AD410
    bool GetDef(long index, CDefPointer* out);        // 0x0042AF3C
};
struct CThingManager {
    unsigned char _pad00[0x20];
    CDefinitionManager* DefinitionManager;            // +0x20 (PDB +0x2c)
};
extern CThingManager* g_pThingManager;                // 0x013B8A1C
struct CThingInfo {
    unsigned __int64 UniqueID;                        // +0x00
    CCharString      DefName;                         // +0x08
    signed char      Player;                          // +0x0c
};
struct CThing {
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
    if (info.DefName.PStringData == 0 ? strcmp("NULL", "") == 0 : info.DefName.PStringData->Data == "NULL") {   // CCharString::operator==(const char*) written out
        DefGlobalIndex = 0;
    } else {
        DefGlobalIndex = (unsigned short)g_pThingManager->DefinitionManager->GetDefGlobalIndexFromName(info.DefName);
        if (DefGlobalIndex == 0) {
            Dead = true;
            return false;
        }
    }
    if (DefGlobalIndex > 0)
        g_pThingManager->DefinitionManager->GetDef(DefGlobalIndex, &PBaseDef);
    return true;
}
