// CPlayerManager::InitPlayerCharacter(bool) -- give the main player its character: the creature named by the PLAYER_HERO player def's CharacterDef, or CREATURE_HERO when that def is missing
// Grade: DIFFER(195v195) -- frame 0xC v 8: retail reuses the dead GetDef out slot (+4) for the character name and the
// PLAYER_HERO temporary's slot for the CDefString; ours opens a third word (block-scoping the out slot or passing the
// names as temporaries keeps the third word and lengthens the release)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00449D90, length 208 bytes (+ int3 padding), `ret 4`.
// Identity: Ego_r.pdb `?InitPlayerCharacter@CPlayerManager@@QAEX_N@Z` (the bool is not read here).
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0,
// CDefinitionManager::GetDefGlobalIndexFromName(const CCharString&) 0x009AD410,
// CDefinitionManager::GetDef<CPlayerDef>(long, CDefPointer<const CPlayerDef>*) 0x0044BA90 (catalogued under a
// propagated CThingPhysicalSwitchDef instantiation name), CDefinitionManager::GetDefNameFromGlobalIndex(long)
// 0x009ACCC0 (CDefString by hidden pointer), CDefStringTable::GetString(CDefString) 0x009D49B0 on g_DefStringTable
// 0x013CA828 (CCharString by hidden pointer), CPlayerManager::GetPlayer(long) 0x004498C0,
// CPlayer::InitCharacterAs(const CCharString&) 0x0048A070.
// Layout: DefinitionManager +0x08, MainPlayer +0x1C (engine/CPlayerManager.h has the PDB's 16-byte Players vector, so
// MainPlayer sits at +0x20 there; retail vectors are 12 bytes); CPlayerDef::CharacterDef +0x3C (PDB +0x40).
// Shape: "PLAYER_HERO" reaches GetDefGlobalIndexFromName as an implicit conversion temporary (re-addressed with lea).
// GetDef fills a plain zeroed out slot; its object is taken once into a holder (edi) whose destructor releases it
// (null test + `dec [edi+4] ; jne ; call [vtable+4]`) after either arm, and the dead out slot is reused for the
// character name. Each arm builds its own name in the same slot, so the GetPlayer/InitCharacterAs tail and the
// name's destructor are shared.
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};

struct CDefString {                                     // a definition-string handle: one table index
    long m_index;
};
struct CDefStringTable {
    CCharString GetString(CDefString name);             // 0x009D49B0
};
extern CDefStringTable g_DefStringTable;                // 0x013CA828

struct CDefPointeeBase {
    virtual void v00();
    virtual void Destroy() const;                       // slot 1
    mutable long RefCount;                              // +0x04
    void Release() const { if (--RefCount == 0) Destroy(); }
};
struct CPlayerDef : CDefPointeeBase {
    unsigned char _pad08[0x3c - 0x08];
    long          CharacterDef;                         // +0x3c
};
template <class T> struct CDefPointer {                 // the out slot GetDef fills
    T* Object;
};
template <class T> struct CDefHolder {                  // owns one reference taken from an out slot
    T* Object;
    CDefHolder(T* object) : Object(object) { }
    ~CDefHolder() { if (Object) Object->Release(); }
    T* operator->() const { return Object; }
};

struct CDefinitionManager {
    long GetDefGlobalIndexFromName(const CCharString& name);         // 0x009AD410
    CDefString GetDefNameFromGlobalIndex(long index);                // 0x009ACCC0
    template <class T> bool GetDef(long index, CDefPointer<const T>* def);   // 0x0044BA90 (CPlayerDef)
};
struct CGameDefinitionManager : CDefinitionManager { };

struct CPlayer {
    void InitCharacterAs(const CCharString& name);      // 0x0048A070
};

struct CPlayerManager {
    void*                   __vftable;
    struct CMainGameComponent* Component;               // +0x04
    CGameDefinitionManager* DefinitionManager;          // +0x08
    unsigned char           Players[0x0c];              // +0x0c vector<CPlayer*>
    long                    PlayerNeutral;              // +0x18
    long                    MainPlayer;                 // +0x1c
    CPlayer* GetPlayer(long number);                    // 0x004498C0
    void InitPlayerCharacter(bool fromSave);
};

void CPlayerManager::InitPlayerCharacter(bool fromSave)
{
    (void)fromSave;
    CDefPointer<const CPlayerDef> out;
    out.Object = 0;
    long index = DefinitionManager->GetDefGlobalIndexFromName("PLAYER_HERO");
    bool found = DefinitionManager->GetDef(index, &out);
    CDefHolder<const CPlayerDef> playerDef(out.Object);
    if (found) {
        CCharString characterName = g_DefStringTable.GetString(DefinitionManager->GetDefNameFromGlobalIndex(playerDef->CharacterDef));
        GetPlayer(MainPlayer)->InitCharacterAs(characterName);
    } else {
        CCharString characterName("CREATURE_HERO", -1);
        GetPlayer(MainPlayer)->InitCharacterAs(characterName);
    }
}
