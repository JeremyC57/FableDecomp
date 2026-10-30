// CCreatureAction_TavernGameMasterInteraction::Initialise() -- the creature-action Initialise, then, when both the creature and the game master are alive, enter the tavern game the game master's interface-0xD2 component points at unless that game is already being played
// Grade: DIFFER(165v165) -- wave 2 sec3: same-TU CVectorMap::LowerBound 0x0040F020 body and the PDB IS_THING_ALIVE<CIntelligentPointer<T>> inline template tried; residue unchanged: this in esi (retail edi) with the pointer temporaries in edi (retail esi)
// Leaf: ?Initialise@CCreatureAction_TavernGameMasterInteraction@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00845B10, length 176 bytes (plain `ret`, virtual).
// Callees: CCreatureActionBase::Initialise 0x00694D10 (direct base call, first), CBaseIntelligentPointer::GetPItem 0x00A01B50
// (PCreature +8 twice, PTarget +0xA8 three times), CVectorMap::LowerBound 0x0040F020 (inline GetInterface 0xD2, bit 18 of the
// present-bits dword +0x38), CTCTavernGame::EnterGame virtual slot 29 (+0x74; 0x005EC0F0 in vtable 0x012560BC by RTTI).
// Layout: PTarget +0xA8 (PDB +0xB4; the debug CCreatureActionBase is 0xC larger); the 0xD2 component keeps the tavern game
// at +0x0C (named CTCGameState / m_pGameMaster in the landed 0x0094CA80); CTCTavernGame::IsBeingPlayed is retail +0x20 (PDB
// +0x28: the 12-byte retail vector moves the 8-aligned double up).
// Shape: the UNCHECKED out-parameter GetInterface (the absent path reads the never-written result, which shares the key's
// frame word: `mov eax,[esp+8]`).
enum ETCInterfaceType { TC_INTERFACE_NONE = 0 };
class CTCBase;
namespace std {
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
template <class _Tp> class allocator { public: allocator() { } };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    _Tp* _M_start; _Tp* _M_finish; _Tp* _M_end_of_storage;
};
}
template <class K, class V> class CKeyPairCompareLess { public: CKeyPairCompareLess() { } };
template <class K, class V, class C = CKeyPairCompareLess<K, V> >
class CVectorMap : public std::vector<std::pair<K, V> > {
public:
    typedef std::pair<K, V> value_type;
    __declspec(noinline) value_type* LowerBound(const K& key)       // 0x0040F020: the retail body in the TU (same-TU mod-ref)
    {
        long count = this->_M_finish - this->_M_start;
        value_type* first = this->_M_start;
        while (count > 0) {
            long half = count / 2;
            value_type* middle = first + half;
            if (middle->first < key) {
                first = middle + 1;
                count = count - half - 1;
            } else
                count = half;
        }
        return first;
    }
    C Compare;
    bool Dirty;
};
typedef CVectorMap<ETCInterfaceType, CTCBase*> CInterfaceMap;
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad54[0x91 - 0x54];
    unsigned char m_deleteFlags;                // +0x91 bit 0: being deleted
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::value_type* it = map->LowerBound(type);
            if (it == map->_M_finish || interfaceType < it->first)
                it = map->_M_finish;
            *ppOut = (T*)it->second;
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
};
template <class T> inline bool __fastcall IS_THING_ALIVE(const T& thing)   // Ego_r ??$IS_THING_ALIVE@V?$CIntelligentPointer@..@@@@YI_NABV..@@Z (inline here)
{
    return thing.GetPItem() && !(thing.GetPItem()->m_deleteFlags & 1);
}

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
    if (IS_THING_ALIVE(PCreature) && IS_THING_ALIVE(PTarget)) {
        CTCGameState* state;
        PTarget.GetPItem()->GetInterface(&state, 0xd2);
        CTCTavernGame* game = state->m_pGameMaster;
        if (game && !game->IsBeingPlayed)
            game->EnterGame();
    }
}
