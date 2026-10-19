// CCreatureAction_TavernGameMasterInteraction::Initialise() -- the creature-action Initialise, then, when both the creature and the game master are alive, enter the tavern game the game master's interface-0xD2 component points at unless that game is already being played
// Grade: DIFFER(165v165) -- register swap only: retail keeps `this` in edi and the intelligent pointers in esi (lea esi,[edi+0A8h]); ours puts `this` in esi and reuses it (add esi,0A8h)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00845B10, length 176 bytes (plain `ret`, virtual).
// Callees: CCreatureActionBase::Initialise 0x00694D10 (direct base call, first), CBaseIntelligentPointer::GetPItem 0x00A01B50
// (PCreature +8 twice, PTarget +0xA8 three times), CVectorMap::LowerBound 0x0040F020 (inline GetInterface 0xD2, bit 18 of the
// present-bits dword +0x38), CTCTavernGame::EnterGame virtual slot 29 (+0x74; 0x005EC0F0 in vtable 0x012560BC by RTTI).
// Layout: PTarget +0xA8 (PDB +0xB4; the debug CCreatureActionBase is 0xC larger); the 0xD2 component keeps the tavern game
// at +0x0C (named CTCGameState / m_pGameMaster in the landed 0x0094CA80); CTCTavernGame::IsBeingPlayed is retail +0x20 (PDB
// +0x28: the 12-byte retail vector moves the 8-aligned double up).
// Shape: the UNCHECKED out-parameter GetInterface (the absent path reads the never-written result, which shares the key's
// frame word: `mov eax,[esp+8]`).
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_deleteFlags;                // +0x91 bit 0: being deleted
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};

struct CTCTavernGame {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28();
    virtual void EnterGame();                   // slot 29 (+0x74)
    unsigned char _pad04[0x20 - 4];
    bool          IsBeingPlayed;                // +0x20
};

struct CTCGameState {                           // interface 0xd2
    unsigned char  _pad[0xc];
    CTCTavernGame* m_pGameMaster;               // +0x0c
};

struct CBaseIntelligentPointer {
    void*   __vftable;
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
    bool IsAlive() const { return GetPItem() && !(GetPItem()->m_deleteFlags & 1); }
};

class CCreatureActionBase {
public:
    virtual void v00();
    virtual void Initialise();                  // 0x00694D10
    unsigned char           _pad04[4];
    CBaseIntelligentPointer PCreature;          // +0x08
    unsigned char           _pad10[0xa8 - 0x10];
};

class CCreatureAction_TavernGameMasterInteraction : public CCreatureActionBase {
public:
    CBaseIntelligentPointer PTarget;            // +0xa8
    virtual void Initialise();
};

void CCreatureAction_TavernGameMasterInteraction::Initialise()
{
    CCreatureActionBase::Initialise();
    if (PCreature.IsAlive() && PTarget.IsAlive()) {
        CTCGameState* state;
        PTarget.GetPItem()->GetInterface(&state, 0xd2);
        CTCTavernGame* game = state->m_pGameMaster;
        if (game && !game->IsBeingPlayed)
            game->EnterGame();
    }
}
