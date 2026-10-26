// CTCHeroOnlineScoreboard::OnOkClicked() -- closes the scoreboard screen and gives input back to the inventory's mouse pointer
// Leaf: OnOkClicked
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0055EE20, 211 bytes.
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020, CCharString::CCharString(const char*, long)
//          @ 0x0099EBF0 (the by-value name), the screen's CComponent slots 3 (+0x0C, find child by name) and 48
//          (+0xC0), NUISystem::CManager::GetInstance() @ 0x0041E5F2 (x3), the manager's CObservable slots 10 (+0x28,
//          0x0055CA00; name unproven) and 2 (+0x08, AddObserver), the pointer's CObserver slot 3 (+0x0C, ObserveEvent).
// Shape: the hero's inventory (interface 0x11) is the inlined raw bitset probe (`test dword ptr [thing+20h],20000h`)
// plus an inline GetTC (key in the `push ecx` word); the screen's vtable is read before the by-value name is
// built; components go to the manager as their CObserver sub-object at +4 (converting null tests); the mouse
// pointer re-observes event 0x19 through the +4 base without a null test -- the twin of CTCInGameMenu::
// ConfirmedVideoOptionsChanged (0x0047D012).
// Layout: retail CTCHeroOnlineScoreboard (12-byte maps/vectors, 8-byte lists) keeps PScoreboardScreen at +0x60
// (PDB +0x78); retail CTCInventory's PMouse_Pointer is +0x214 (PDB +0x260).
class CTCBase;

class CCharStringData;
class CCharString { public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length);      // 0x0099EBF0
    CCharString(const CCharString& other);           // (declared: by-value arguments are built in place)
    ~CCharString();
};

namespace NUISystem {
class CObserver { public:                                   // the observer interface at +4 of every component
    virtual void o00(); virtual void o01(); virtual void o02();
    virtual void ObserveEvent(long eventType);       // slot 3 (+0x0C)
};
struct CComponentBase {                              // the primary vtable
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual class CComponent* FindChild(CCharString name);   // slot 3 (+0x0C)
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13();
    virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18();
    virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28();
    virtual void v29(); virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33();
    virtual void v34(); virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38();
    virtual void v39(); virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43();
    virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void SetActive_slot48(bool active);      // slot 48 (+0xC0)
};
class CComponent : public CComponentBase, public CObserver { public:
};
class CManager { public:                                    // NUISystem::CManager (CObservable vtable part)
    virtual void m00(); virtual void m01();
    virtual void AddObserver(CObserver* observer);   // slot 2 (+0x08)
    virtual void m03(); virtual void m04(); virtual void m05(); virtual void m06(); virtual void m07(); virtual void m08();
    virtual void m09();
    virtual void ReleaseObserver_0055ca00(CObserver* observer);   // slot 10 (+0x28)
    static CManager* __fastcall GetInstance();       // 0x0041E5F2
};
}

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;                                   // +0x00
    SEntry* last;                                    // +0x04
    SEntry* LowerBound(const long* type);            // 0x0040F020
};

class CThing { public:
    unsigned char _pad_0x00[0x20];
    unsigned long m_presentBits[9];                  // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                      // +0x44
    bool HasTC(long wanted)
    {
        if (m_presentBits[wanted >> 5] & (1UL << (wanted & 31)))
            return true;
        return false;
    }
    template<class T> __forceinline T* GetTC(long wanted)
    {
        long type = wanted;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || wanted < it->type)
            it = map->last;
        return (T*)it->component;
    }
};

class CTCInventory { public:                                // interface type 0x11
    unsigned char          _pad_0x000[0x214];
    NUISystem::CComponent* PMouse_Pointer;           // +0x214 (PDB +0x260)
};

class CTCHeroOnlineScoreboard { public:
    void*                  __vftable;                // +0x00
    CThing*                ParentThing;              // +0x04 CTCBase
    unsigned char          _pad_0x08[0x58];
    NUISystem::CComponent* PScoreboardScreen;        // +0x60 (PDB +0x78)
    void OnOkClicked();
};

void CTCHeroOnlineScoreboard::OnOkClicked()
{
    CThing* thing = ParentThing;
    if (thing->HasTC(0x11)) {
        CTCInventory* inventory = thing->GetTC<CTCInventory>(0x11);
        NUISystem::CComponent* okButton = PScoreboardScreen->FindChild(CCharString("PC_OK_GAME_OKBUTTON_SCOREBOARD", -1));
        NUISystem::CManager::GetInstance()->ReleaseObserver_0055ca00(inventory->PMouse_Pointer);
        NUISystem::CManager::GetInstance()->ReleaseObserver_0055ca00(okButton);
        PScoreboardScreen->SetActive_slot48(false);
        NUISystem::CManager::GetInstance()->AddObserver(inventory->PMouse_Pointer);
        inventory->PMouse_Pointer->ObserveEvent(0x19);
    }
}