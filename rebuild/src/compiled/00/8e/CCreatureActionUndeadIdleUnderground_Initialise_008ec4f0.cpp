// CCreatureAction_UndeadIdleUnderground::Initialise() -- put the undead into mode 0x1e through its mode manager, refresh the
// player GUI, call creature slot 28 with 0, clear the weapon trails of every live carried thing, clear the byte at +0x10 of
// the creature's interface-9 component, set the "ST_IDLE" combat animation, then the base initialisation.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008EC4F0, length 432 bytes (0x1ae code + int3 pad).
// Callees: 0x00A01B50 CBaseIntelligentPointer::GetPItem (PCreature +8); 0x0040F020 CVectorMap::LowerBound (inlined interface
// lookups 0x31, 0x46, 0x4c, 9); 0x0073AE10 CTCCreatureModeManager::AddMode; 0x004378C0 CPlayerGui::Refresh on 0x013B8790
// (landed 0x008C63C0 naming); creature virtual slot 28 (+0x70); 0x006A2220 CTCCarrying fill vector<CThing*> (landed
// 0x007CDFE0 naming); 0x0076E150 CTCWeaponTrail::ClearAllWeaponTrails; 0x00BFEA14 free; 0x0099EBF0/0x0099EAE0 CCharString;
// 0x008574D0 CCombatActionBase::SetCombatAnimation; 0x00857520 CCombatActionBase::Initialise (direct base call).
// Layout: local model (landed 0x008C63C0 / 0x00848FD0 / 0x007CDFE0 models): PCreature +8, CThing bitset +0x20, interface map
// +0x44, dead bit +0x91 bit 0. Interface 9's component class not identified (byte +0x10 cleared).
// Shape: mode manager = HasTC + the one-ternary GetTC into an UNINITIALISED local (both arms load straight into ecx; the
// if/assign GetTC form loads eax and copies). The 0x46/0x4c/9 lookups are GetInterface out-parameter forms whose entry pick
// is also a ternary (the if/reassign pick gave `mov eax,[eax+4] ; mov ecx,eax`). The carried-things vector is a local
// STLport vector (free-based dtor) scoped in the 0x46 block.
// Leaf: Initialise
extern "C" void __cdecl free(void* block);           // 0x00BFEA14
class CCharString { public:
    char* m_pString;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);   // 0x0040F020
};
template <class T> struct CRetailVector {   // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    CRetailVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    ~CRetailVector() { if (_M_start) free(_M_start); }
    T* begin() { return _M_start; }
    T* end() { return _M_finish; }
};
class CThing { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27();
    virtual void Slot28(long);              // slot 28 (+0x70)
    unsigned char _pad04[0x20 - 4];
    unsigned long m_presentBits[9];         // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;             // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                // +0x91 bit 0: dead
    unsigned long HasTC(long interfaceType) const { return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)); }
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                        // +4
    CThing* GetPItem() const;               // 0x00A01B50
};
class CTCCreatureModeManager { public:             // interface type 0x31
    bool AddMode(unsigned long mode);       // 0x0073AE10
};
class CTCCarrying { public:                        // interface type 0x46
    void GetCarriedThings(CRetailVector<CThing*>& things);   // 0x006A2220
};
class CTCWeaponTrail { public:                     // interface type 0x4c
    void ClearAllWeaponTrails();            // 0x0076E150
};
struct CTCType9 {                           // interface type 9
    unsigned char _pad00[0x10];
    bool m_flag10;                          // +0x10
};
class CPlayerGui { public:
    void Refresh(long flag);                // 0x004378C0
};
extern CPlayerGui* g_pPlayerGui;            // 0x013B8790

class CCombatActionBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void Initialise();              // slot 12, 0x00857520
    unsigned char _pad04[4];
    CBaseIntelligentPointer PCreature;      // +0x08
    unsigned char _pad10[0x114 - 0x10];
    void SetCombatAnimation(const CCharString& name);   // 0x008574D0
};
class CCreatureAction_UndeadIdleUnderground : public CCombatActionBase { public:
    virtual void Initialise();
};

void CCreatureAction_UndeadIdleUnderground::Initialise()
{
    CTCCreatureModeManager* pModes;
    CThing* creature = PCreature.GetPItem();
    if (creature->HasTC(0x31))
        pModes = creature->GetTC<CTCCreatureModeManager>(0x31);
    pModes->AddMode(0x1e);
    g_pPlayerGui->Refresh(0);
    PCreature.GetPItem()->Slot28(0);
    CTCCarrying* pCarrying;
    if (PCreature.GetPItem()->GetInterface(&pCarrying, 0x46)) {
        CRetailVector<CThing*> things;
        pCarrying->GetCarriedThings(things);
        for (CThing** it = things.begin(); it != things.end(); ++it) {
            CThing* thing = *it;
            if (thing && !(thing->m_flags91 & 1)) {
                CTCWeaponTrail* pTrail;
                if (thing->GetInterface(&pTrail, 0x4c))
                    pTrail->ClearAllWeaponTrails();
            }
        }
    }
    CTCType9* p9;
    if (PCreature.GetPItem()->GetInterface(&p9, 9))
        p9->m_flag10 = false;
    SetCombatAnimation("ST_IDLE");
    CCombatActionBase::Initialise();
}