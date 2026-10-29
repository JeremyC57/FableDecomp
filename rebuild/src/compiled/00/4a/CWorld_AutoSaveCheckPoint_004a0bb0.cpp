// CWorld::AutoSaveCheckPoint() -- checkpoint autosave: record the save type, have the main hero's three inventories take a quest-persistent reference on their non-persistent items, save as the checkpoint file and remember that a checkpoint exists
// Leaf: AutoSaveCheckPoint
// Retail: Fable.exe (TLC, VC7.1) @ 0x004A0BB0, 285 bytes (288 with padding), plain `ret`. PDB ?AutoSaveCheckPoint@CWorld@@QAE_NXZ.
// Callees: CPlayerManager::GetMainPlayer 0x00449970, CPlayer::GetPControlledCreature 0x00487DC0, CInterfaceMap::LowerBound
//   0x0040F020 (x3), NInventory::CTCInventoryBase::IncrementQuestPersistentRefCountForNonPersistentItems 0x005BCF51
//   (`UpdateNonQuestPersistentRefCountForItems(true)`; label "_Dest_val<...CFlash>" is wrong; name from the PDB list,
//   its false twin at 0x005BCF59), CTBaseSingleton<CUserProfileManager>::Get 0x0040D2A0, CUserProfileManager::
//   GetAutoSaveCheckPointFileName 0x004066F0, CWorld::AutoSaveInternal(const CWideString&, bool) 0x004A0850, ~CWideString
//   0x0099B510.
// Layout: landed siblings AutoSave / AutoSaveQuestStart (MostRecentSaveType +0x134, ...BeforeManualSave +0x138,
//   AutoSaveCheckPointExists +0x140); CThing interface bitset +0x20 (TCs 0x11/0x12/0x13 = word 0 bits 17/18/19),
//   interface map +0x44, Dead +0x91 bit 0 (landed EACloseHeroCentreDoor model).
// Shape: HasTC + inline GetTC per inventory inside the alive test (edi pushed only there, all three keys in the
//   single frame word that later holds the file-name temporary); the save result held in bl across that temporary's
//   destructor; the flag returned by re-reading the member.
class CWideStringData;
class CWideString { public:
    CWideStringData* PStringData;
    ~CWideString();                                          // 0x0099B510
};
class CUserProfileManager { public:
    static CUserProfileManager& __fastcall Get();            // 0x0040D2A0 CTBaseSingleton<CUserProfileManager>::Get
    CWideString GetAutoSaveCheckPointFileName() const;       // 0x004066F0
};
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);          // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long UsingComponents[9];              // +0x20 bitset<274>
    CInterfaceMap Components;                      // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char Dead : 1;                        // +0x91 bit 0
    bool HasTC(long type) const
    {
        if (UsingComponents[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
        CInterfaceMap* map = &Components;
        CInterfaceMap::SEntry* it = map->LowerBound(type);
        if (it == map->last || interfaceType < it->first)
            it = map->last;
        return (T*)it->second;
    }
};
class CThingPlayerCreature : public CThing { };
namespace NInventory {
class CTCInventoryBase { public:                          // TC 0x11 / 0x12 / 0x13 (the three inventories)
    void IncrementQuestPersistentRefCountForNonPersistentItems();   // 0x005BCF51
};
}
class CPlayer { public: CThingPlayerCreature* GetPControlledCreature() const; };          // 0x00487DC0
class CPlayerManager { public: public:
    CPlayer& GetMainPlayer();   // 0x00449970
}; // 0x00449970
class CWorld { public:
    unsigned char   _pad00[0xc];
    CPlayerManager* PlayerManager;                            // +0x0C
    unsigned char   _pad10[0x134 - 0x10];
    long            MostRecentSaveType;                       // +0x134 ESaveType (PDB +0x144)
    long            MostRecentSaveTypeBeforeManualSave;       // +0x138 ESaveType (PDB +0x148)
    unsigned char   _pad13c[4];                               // +0x13C MostRecentManualSaveName
    bool            AutoSaveCheckPointExists;                 // +0x140 (PDB +0x150)
    bool AutoSaveInternal(const CWideString& name, bool b);   // 0x004A0850
    bool AutoSaveCheckPoint();
};
bool CWorld::AutoSaveCheckPoint()
{
    MostRecentSaveType = 2;
    MostRecentSaveTypeBeforeManualSave = 2;
    CThing* hero = PlayerManager->GetMainPlayer().GetPControlledCreature();
    if (hero && !hero->Dead)
    {
        if (hero->HasTC(0x11))
            hero->GetTC<NInventory::CTCInventoryBase>(0x11)->IncrementQuestPersistentRefCountForNonPersistentItems();
        if (hero->HasTC(0x13))
            hero->GetTC<NInventory::CTCInventoryBase>(0x13)->IncrementQuestPersistentRefCountForNonPersistentItems();
        if (hero->HasTC(0x12))
            hero->GetTC<NInventory::CTCInventoryBase>(0x12)->IncrementQuestPersistentRefCountForNonPersistentItems();
    }
    bool saved = AutoSaveInternal(CUserProfileManager::Get().GetAutoSaveCheckPointFileName(), false);
    if (saved)
        AutoSaveCheckPointExists = true;
    return AutoSaveCheckPointExists;
}