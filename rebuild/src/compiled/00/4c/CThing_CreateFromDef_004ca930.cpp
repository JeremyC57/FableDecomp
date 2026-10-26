// CThing::CreateFromDef(const CThingDef* def) -- reserves room for the def's component list, adds each named component (its creation flag forced on while a saved game is being loaded), then takes the kill-lock bit from the def
// Leaf: CreateFromDef
// Retail: Fable.exe (TLC, VC7.1) @ 0x004CA930, 204 bytes (+4 int3). `ret 4`.
// Callees: CVectorMap (vector) reserve 0x004CC9B0 on Components (+0x44), CDefStringTable::GetString(long) 0x009D49B0 on
//          g_DefStringTable (0x013CA828, CCharString by hidden pointer), CThing::AddTC(const CCharString&, bool,
//          CTCParamsBase*) 0x004C9D60, ~CCharString 0x0099EAE0.
// Shape: the force flag is a frame byte (0, then 1 when ThingTypeFlags bit 1 is set and the world's +0x102 byte is on);
//        the reserve count is the signed pair distance (`sar 3`) plus the def's signed component count; the loop keeps
//        the index in ebx and the 9-byte record offset in edi, re-reading the record pointer and the count after the
//        calls; the per-component flag byte is copied to the frame then overwritten with 1 under the force flag and
//        pushed as a dword; the def bit 2 of +0x41 lands in KillLocked (+0x91 bit 6) through the `shl 4 / and 40h` merge.
// Layout: def classes pack to 1 (CDefClassBase is 0x25): the component records at +0x39, their count at +0x3D, the
//         flag bits at +0x41; CThingManager::World +0x24 (PDB +0x30).
class CCharString {
public:
    void* PStringData;
    ~CCharString();                                  // 0x0099EAE0
};
struct CDefStringTable {
    CCharString GetString(long id);                  // 0x009D49B0
};
extern CDefStringTable g_DefStringTable;             // 0x013CA828
class CTCBase; class CTCParamsBase;
struct SComponentPair { long first; CTCBase* second; };
struct CComponentMap {                               // CVectorMap<ETCInterfaceType, CTCBase*>: STLport vector first
    SComponentPair* m_first;
    SComponentPair* m_last;
    SComponentPair* m_end;
    void reserve(unsigned long count);               // 0x004CC9B0
    long size() const { return m_last - m_first; }
};
class CWorld { public:
    unsigned char _pad00[0x102];
    bool          m_loadingSavedGame;                // +0x102 (descriptive)
};
class CThingManager { public:
    unsigned char _pad00[0x24];
    CWorld*       World;                             // +0x24
};
extern CThingManager* g_pThingManager;               // 0x013B8A1C
#pragma pack(push, 1)
struct STCDefEntry {                                 // 9 bytes
    long          NameId;                            // +0x00
    long          _unused;                           // +0x04
    bool          Flag;                              // +0x08
};
struct CThingDef {
    unsigned char _pad00[0x39];
    STCDefEntry*  TCs;                               // +0x39
    signed char   NoTCs;                             // +0x3d
    unsigned char _pad3e[0x41 - 0x3e];
    unsigned char DefBit0 : 1;                       // +0x41
    unsigned char DefBit1 : 1;
    unsigned char KillLocked : 1;
};
#pragma pack(pop)
class CThing { public:
    unsigned char _pad00[0x44];
    CComponentMap Components;                        // +0x44
    unsigned char _pad50[0x6c - 0x50];
    unsigned long ThingTypeFlags;                    // +0x6c
    unsigned char _pad70[0x91 - 0x70];
    unsigned char Dead : 1;                          // +0x91
    unsigned char CreatedByOwnerThingCreation : 1;
    unsigned char IncludeInThingScans : 1;
    unsigned char Activated : 1;
    unsigned char GloballyLoaded : 1;
    unsigned char InLimbo : 1;
    unsigned char KillLocked : 1;                    // bit 6
    unsigned char CreatedBySerialisation : 1;
    CTCBase* AddTC(const CCharString& name, bool a2, CTCParamsBase* params);   // 0x004C9D60
    void CreateFromDef(const CThingDef* def);
};
void CThing::CreateFromDef(const CThingDef* def)
{
    bool force = false;
    if ((ThingTypeFlags & 2) && g_pThingManager->World->m_loadingSavedGame)
        force = true;
    Components.reserve(Components.size() + def->NoTCs);
    for (long i = 0; i < def->NoTCs; ++i) {
        CCharString name = g_DefStringTable.GetString(def->TCs[i].NameId);
        bool flag = def->TCs[i].Flag;
        if (force)
            flag = true;
        AddTC(name, flag, 0);
    }
    KillLocked = def->KillLocked;
}