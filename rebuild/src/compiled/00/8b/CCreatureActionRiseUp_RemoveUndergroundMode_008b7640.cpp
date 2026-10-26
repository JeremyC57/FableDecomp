// CCreatureAction_RiseUp::RemoveUndergroundMode() -- take the creature out of mode 30 through its mode-manager component (interface type 0x31) and refresh the mini-map display
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008B7640, length 96 bytes.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8), CVectorMap::LowerBound @ 0x0040F020 (the inlined GetInterface lookup for type 0x31 = bit 17 of the
// bitset dword at +0x24), CTCCreatureModeManager::RemoveMode(unsigned long) @ 0x0073ABA0 (landed naming), the game UI singleton at 0x013B8790: GetMiniMapDisplay(0) @ 0x00437CE0
// then CMiniMapDisplay::Refresh @ 0x0082B7C0 (the manifest's labels of those two are propagated). Shape: the mode manager is read from the frame slot even when absent (as 0x00849250).
class CTCBase;
struct CInterfaceMap {                      // a thing's sorted {type, component} vector map at +0x44
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);   // 0x0040F020
};
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];         // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;             // +0x44
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;   // named pointer: retail advances esi to the map once
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
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
class CTCCreatureModeManager { public:
    void RemoveMode(unsigned long mode);    // 0x0073ABA0
};
class CMiniMapDisplay { public:
    void Refresh();                         // 0x0082B7C0
};
struct CGameUI {
    CMiniMapDisplay* GetMiniMapDisplay(long which);   // 0x00437CE0
};
extern CGameUI* g_pGameUI_013b8790;         // 0x013B8790

class CCreatureAction_RiseUp { public:
    virtual void v00();
    unsigned char _pad04[4];
    CBaseIntelligentPointer PCreature;      // +0x08
    void RemoveUndergroundMode();
};

void CCreatureAction_RiseUp::RemoveUndergroundMode()
{
    CTCCreatureModeManager* pModes;
    PCreature.GetPItem()->GetInterface(&pModes, 0x31);
    pModes->RemoveMode(30);
    g_pGameUI_013b8790->GetMiniMapDisplay(0)->Refresh();
}