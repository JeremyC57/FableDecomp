// CCreatureAction_TavernGameMasterInteraction::Initialise() -- base Initialise, then when owner and target are alive and the target's tavern-game-master component (0xd2) has a game that is not yet running (byte +0x20 clear), start it through its virtual slot 29
// Grade: DIFFER(165v165) -- register choice only: retail this in edi with lea esi,[edi+8]/lea esi,[edi+0A8h], ours this in esi with lea edi,[esi+8]/add esi,0A8h
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00845B10, length 165 bytes (+ int3 padding), plain `ret` (virtual).
// Callees: CCreatureActionBase::Initialise @ 0x00694D10, CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CVectorMap::LowerBound
// @ 0x0040F020 (inlined GetInterface 0xD2, bit 18 of +0x38), the game's virtual slot 29 (+0x74).
// Shape: two "present and not dead" guards (thing byte +0x91 bit 0), then the UNCHECKED out-parameter lookup in an inline
// accessor (the component stays in eax on both arms; the absent arm reads the key's frame word), the game pointer at
// +0xc null-tested and its flag byte (+0x20) tested before the virtual call.
// Layout: Ego_r.pdb PTarget +0xb4, retail +0xa8 (CCreatureActionBase is 0xa8 in retail).
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
    unsigned char _pad4c[0x45];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
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

struct CTavernGame {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28();
    virtual void StartGame();                   // slot 29 (+0x74)
    unsigned char _pad04[0x20 - 4];
    bool m_running;                             // +0x20
};
struct CTCTavernGameMaster {
    unsigned char _pad[0xc];
    CTavernGame*  m_pGame;                      // +0x0c
};

struct CBaseIntelligentPointer {
    void*   __vftable;
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};

class CCreatureActionBase {
public:
    virtual void Initialise();                  // 0x00694D10
    unsigned char           _pad04[4];
    CBaseIntelligentPointer PCreature;          // +0x08
    unsigned char           _pad10[0xa8 - 0x10];
};

class CCreatureAction_TavernGameMasterInteraction : public CCreatureActionBase {
public:
    CBaseIntelligentPointer PTarget;            // +0xa8 (PDB +0xb4)
    virtual void Initialise();
    __forceinline CTCTavernGameMaster* GetGameMaster() const
    {
        CTCTavernGameMaster* master;
        PTarget.GetPItem()->GetInterface(&master, 0xd2);
        return master;
    }
};

void CCreatureAction_TavernGameMasterInteraction::Initialise()
{
    CCreatureActionBase::Initialise();
    if (PCreature.GetPItem() && !(PCreature.GetPItem()->m_flags91 & 1) &&
        PTarget.GetPItem() && !(PTarget.GetPItem()->m_flags91 & 1)) {
        CTavernGame* game = GetGameMaster()->m_pGame;
        if (game && !game->m_running)
            game->StartGame();
    }
}
