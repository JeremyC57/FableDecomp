// CPlayerManager::InitPlayerCharacter(bool) -- give the main player its character: the creature named by the PLAYER_HERO player def's CharacterDef, or CREATURE_HERO when that def is missing
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00449D90, length 208 bytes (+ int3 padding), `ret 4`.
// Identity: Ego_r.pdb `?InitPlayerCharacter@CPlayerManager@@QAEX_N@Z` (the bool is not read here).
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0,
// CDefinitionManager::GetDefGlobalIndexFromName(const CCharString&) 0x009AD410,
// CDefinitionManager::GetDef<CPlayerDef>(long, CDefPointer<const CPlayerDef>*) 0x0044BA90 (catalogued under a
// propagated CThingPhysicalSwitchDef instantiation name), CDefinitionManager::GetDefNameFromGlobalIndex(long)
// 0x009ACCC0 (CDefString by hidden pointer), CDefStringTable::GetString(CDefString) 0x009D49B0 on GDefStringTable
// 0x013CA828 (CCharString by hidden pointer), CPlayerManager::GetPlayer(long) 0x004498C0,
// CPlayer::InitCharacterAs(const CCharString&) 0x0048A070.
// Layout: DefinitionManager +0x08, MainPlayer +0x1C (engine/CPlayerManager.h has the PDB's 16-byte Players vector, so
// MainPlayer sits at +0x20 there; retail vectors are 12 bytes); CPlayerDef::CharacterDef +0x3C (PDB +0x40).
// Shape: "PLAYER_HERO" reaches GetDefGlobalIndexFromName as an implicit conversion temporary (re-addressed with lea).
// GetDef fills a plain zeroed out slot; its object is taken once into a holder (edi) whose destructor releases it
// (null test + `dec [edi+4] ; jne ; call [vtable+4]`) after either arm, and the dead out slot is reused for the
// character name. Each arm builds its own name in the same slot, so the GetPlayer/InitCharacterAs tail and the
// name's destructor are shared.
// Retry 10b: the same-TU GetDef<CPlayerDef> body (operator=-shaped out assignment) plus the definition manager read through
//   an inline GetDefinitionManager() accessor give retail's register use and frame packing.
// Name fix (agentNF5 2026-11-10): CDefStringTable::GetString 0x009D49B0 is ?GetString@CDefStringTable@@QBE?AVCCharString@@J@Z (Ego_r.pdb / FableWin.pdb): const, takes the long table index (the CDefString's one word).
void ReleaseDef_s7(void* def);   // stand-in: the def's release (count decrement, Destroy on zero)
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};

class CDefString { public:                                     // a definition-string handle: one table index
    long m_index;
};
class CDefStringTable { public:
    CCharString GetString(long name) const;             // 0x009D49B0
};
extern CDefStringTable GDefStringTable;   // 0x013CA828 ?GDefStringTable@@3VCDefStringTable@@A (ESTABLISHED; naming wave 21)                // 0x013CA828

class CDefPointeeBase { public:
    virtual void v00();
    virtual void Destroy() const;                       // slot 1
    mutable long RefCount;                              // +0x04
    void Release() const { if (--RefCount == 0) Destroy(); }
};
class CPlayerDef : public CDefPointeeBase { public:
    unsigned char _pad08[0x3c - 0x08];
    long          CharacterDef;                         // +0x3c
};
template <class T> class CDefPointer { public:                 // the out slot GetDef fills
    T* Object;
};
template <class T> struct CDefHolder {                  // owns one reference taken from an out slot
    T* Object;
    CDefHolder(T* object) : Object(object) { }
    ~CDefHolder() { if (Object) Object->Release(); }
    T* operator->() const { return Object; }
};

class CDefinitionManager { public:
    long GetDefGlobalIndexFromName(const CCharString& name) const;         // 0x009AD410
    CDefString GetDefNameFromGlobalIndex(long index);                // 0x009ACCC0
    template <class T> bool GetDef(long index, CDefPointer<const T>* def);   // 0x0044BA90 (CPlayerDef)
};
void* FindDef_s7r_0();
#pragma auto_inline(off)
// Same-TU body of GetDef (retail instantiates it in this TU): fetch the def, then assign it to the out pointer the way
// CDefPointer::operator= does (read the old pointer, release it when different, store the new one).
template <class T> __declspec(noinline) bool CDefinitionManager::GetDef(long p0, CDefPointer<const T>* out)
{
    void* def = FindDef_s7r_0();
    if (!def)
        return false;
    void* old = *(void**)out;
    if (old != def) {
        if (old)
            ReleaseDef_s7(old);
        *(void**)out = def;
    }
    return true;
}
#pragma auto_inline(on)
class CGameDefinitionManager : public CDefinitionManager { public: };

class CPlayer { public:
    void InitCharacterAs(const CCharString& name);      // 0x0048A070
};

class CPlayerManager { public:
    void*                   __vftable;
    class CMainGameComponent* Component;               // +0x04
    CGameDefinitionManager* DefinitionManager;          // +0x08
    CGameDefinitionManager* GetDefinitionManager() const { return DefinitionManager; }
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
    long index = GetDefinitionManager()->GetDefGlobalIndexFromName("PLAYER_HERO");
    bool found = GetDefinitionManager()->GetDef(index, &out);
    CDefHolder<const CPlayerDef> playerDef(out.Object);
    if (found) {
        CCharString characterName = GDefStringTable.GetString((GetDefinitionManager()->GetDefNameFromGlobalIndex(playerDef->CharacterDef)).m_index);
        GetPlayer(MainPlayer)->InitCharacterAs(characterName);
    } else {
        CCharString characterName("CREATURE_HERO", -1);
        GetPlayer(MainPlayer)->InitCharacterAs(characterName);
    }
}