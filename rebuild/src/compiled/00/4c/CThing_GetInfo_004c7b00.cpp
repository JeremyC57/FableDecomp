// CThing::GetInfo(CThingInfo*) const -- fill the thing's info: "None" names for a def-less thing, else the def name as SubTypeName
// Leaf: ?GetInfo@CThing
// Retail: Fable.exe (TLC, VC7.1) @ 0x004C7B00, 123 bytes (+5 int3). __fastcall virtual, `ret 4`.
// Class: catalog "CThing::vfunc_24" -- slot 24 of CThing's vtable 0x01239ABC; the PDB virtual
//        ?GetInfo@CThing@@UBEXPAVCThingInfo@@@Z (CThingInfo: TypeName +0, SubTypeName +4, ScriptName +8, Player +0xC).
// Callees: CCharString::operator=(const char*) 0x0099EFE0 (x2, "None" 0x01239A94); CDefinitionManager::
//          GetDefNameFromGlobalIndex(long) 0x009ACCC0 (CDefString by value, a 4-byte handle); CDefStringTable::GetString(
//          CDefString) 0x009D49B0 on the global table 0x013CA828 (CCharString by value); CCharString::operator=(const
//          CCharString&) 0x0099EFB0; ~CCharString 0x0099EAE0.
// Shape: DefGlobalIndex (+0x8C, `test ax,ax`) zero -> both names "None"; otherwise the definition manager (global systems
//        0x013B8A1C, +0x20, read into a named local first) maps the index to a CDefString temporary whose dword is pushed
//        by value, and the string table's result -- a NAMED CCharString local (`lea eax,[esp] ; push eax`) -- is assigned
//        to SubTypeName and destroyed.
// Layout: retail CThing = Ego_r.pdb - 0x10 here: DefGlobalIndex +0x8C (PDB +0x9C).
class CCharString { public:
    void* PStringData;
    CCharString& operator=(const char* text);           // 0x0099EFE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
    ~CCharString();                                     // 0x0099EAE0
};
class CDefString { public: long Handle; };
class CDefinitionManager { public:
    CDefString GetDefNameFromGlobalIndex(long index) const;   // 0x009ACCC0
};
struct CDefStringTable {
    CCharString GetString(CDefString name) const;            // 0x009D49B0
};
extern CDefStringTable g_DefStringTable;                    // 0x013CA828
struct CGlobalSystems {
    unsigned char _pad00[0x20];
    CDefinitionManager* DefinitionManager;                   // +0x20
};
extern CGlobalSystems* g_pGlobalSystems;                    // 0x013B8A1C
class CThingInfo { public:
    CCharString TypeName;                        // +0x00
    CCharString SubTypeName;                     // +0x04
    CCharString ScriptName;                      // +0x08
    long Player;                                 // +0x0c
};
class CThing {
public:
    virtual void v00();
    unsigned char _pad04[0x8c - 0x04];
    unsigned short DefGlobalIndex;               // +0x8c
    virtual void GetInfo(CThingInfo* info) const;
};
void CThing::GetInfo(CThingInfo* info) const
{
    if (DefGlobalIndex == 0) {
        info->TypeName = "None";
        info->SubTypeName = "None";
        return;
    }
    CDefinitionManager* definitions = g_pGlobalSystems->DefinitionManager;
    CCharString name = g_DefStringTable.GetString(definitions->GetDefNameFromGlobalIndex(DefGlobalIndex));
    info->SubTypeName = name;
}