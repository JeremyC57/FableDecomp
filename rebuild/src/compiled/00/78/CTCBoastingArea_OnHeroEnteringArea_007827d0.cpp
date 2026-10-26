// CTCBoastingArea::OnHeroEnteringArea(CThing& hero) -- unless a mini game is running, open the hero's boast UI and, when the hero has a boasting crowd control, tell it the hero entered the podium area
// Leaf: ?OnHeroEnteringArea@CTCBoastingArea@@AAEXAAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007827D0, length 138 bytes (+6 int3 padding). Private; `ret 4` (`this` unused).
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020 (the inlined GetInterface, twice), CDrawQuestInfo::GetMiniGameType
// @ 0x00644D54 on the game UI's quest info (singleton @ 0x013B8790, +0x1C8), CTCBoastUI::RequestActivate @ 0x0056A860, the crowd
// control's 0x00783BF0 (catalog "OnHeroApproachPodiumArea"; named OnHeroEnterPodiumArea after the PDB member list).
// Shape: nested ifs (the crowd test inside the boast-UI block, so &interfaces stays in esi for both lookups); each component
// is used straight from the GetInterface out pointer.
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

void CTCBoastingArea::OnHeroEnteringArea(CThing& hero)
{
    if (g_pGameUI->m_pQuestInfo->GetMiniGameType() == 0) {
        CTCBoastUI* boastUI;
        if (hero.GetInterface(&boastUI, 0xe8)) {
            boastUI->RequestActivate();
            CTCBoastingCrowdControl* crowd;
            if (hero.GetInterface(&crowd, 0x73))
                crowd->OnHeroEnterPodiumArea();
        }
    }
}