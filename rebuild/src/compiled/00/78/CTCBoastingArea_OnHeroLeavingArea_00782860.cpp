// CTCBoastingArea::OnHeroLeavingArea(CThing& hero) -- close the hero's boast UI when it has one, and tell the hero's boasting crowd control (when any) that the hero left the podium area
// Leaf: ?OnHeroLeavingArea@CTCBoastingArea@@AAEXAAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00782860, length 121 bytes (+7 int3 padding). Private; `ret 4` (`this` unused).
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020 (the inlined GetInterface, twice), CTCBoastUI::Deactivate @ 0x0056A880 (landed),
// the crowd control's 0x00783C20 (catalog "OnHeroAttackCrowdMember"; named OnHeroLeavePodiumArea after the PDB member list).
// Shape: two independent ifs (each lookup forms &interfaces in esi again); each component used straight from the out pointer.
// Layout: retail CThing component bitset +0x20 (0xE8 = CTCBoastUI, bit 8 of the dword at +0x3C; 0x73 = CTCBoastingCrowdControl,
// bit 19 of +0x2C) and interface map +0x44.

class CTCBase;
struct CInterfaceMap {                          // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44: {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
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
class CTCBoastUI { public:
    void RequestActivate();                     // 0x0056A860
    void Deactivate();                          // 0x0056A880
};
class CTCBoastingCrowdControl { public:
    void OnHeroEnterPodiumArea();               // 0x00783BF0 (catalog "OnHeroApproachPodiumArea")
    void OnHeroLeavePodiumArea();               // 0x00783C20 (catalog "OnHeroAttackCrowdMember")
};
struct CDrawQuestInfo {
    long GetMiniGameType();                     // 0x00644D54 (EMiniGameType)
};
struct CGameUI {
    unsigned char _pad00[0x1c8];
    CDrawQuestInfo* m_pQuestInfo;               // +0x1c8
};
extern CGameUI* g_pGameUI;                      // 0x013B8790

class CTCBase {
public:
    virtual ~CTCBase();
    CThing* ParentThing;                        // +0x04
    bool    ToBeRemoved;                        // +0x08
    bool    IsInGlobalUpdateTCs;                // +0x09
};
class CTCBoastingArea : public CTCBase {
private:
    void OnHeroEnteringArea(CThing& hero);
    void OnHeroLeavingArea(CThing& hero);
};

void CTCBoastingArea::OnHeroLeavingArea(CThing& hero)
{
    CTCBoastUI* boastUI;
    if (hero.GetInterface(&boastUI, 0xe8))
        boastUI->Deactivate();
    CTCBoastingCrowdControl* crowd;
    if (hero.GetInterface(&crowd, 0x73))
        crowd->OnHeroLeavePodiumArea();
}