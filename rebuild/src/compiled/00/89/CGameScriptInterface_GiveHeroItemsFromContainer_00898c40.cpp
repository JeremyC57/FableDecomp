// CGameScriptInterface::GiveHeroItemsFromContainer(const CScriptThing& container, bool addToInventory) const -- with a live hero and a valid container: either move the container's items into the inventory (CTCContainer::AddItemsToInventory), or, when the container component is a CTCContainerRewardHero, reward the hero with them and let two script frames pass
// Leaf: ?GiveHeroItemsFromContainer@CGameScriptInterface@@UBE_NABVCScriptThing@@_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00898C40, length 353 bytes (`ret 8`, bool in al).
// Callees: CPlayerManager::GetMainPlayer @ 0x00449970, CPlayer::GetPControlledCreature @ 0x00487DC0, the script thing's slots 75
//   (+0x12C IsValid) and 11 (+0x2C GetThing), the component-map LowerBound @ 0x0040F020 (TC 0x3C, the container), the
//   component's virtual slot 22 (+0x58, class name as a CCharString by value), CCharString ctor/dtor @ 0x0099EBF0 /
//   0x0099EAE0, the string compare @ 0x00411570 (`__fastcall`), CTCContainerRewardHero::RewardHeroWithItems @ 0x0074EE90,
//   CTCContainer::AddItemsToInventory @ 0x008ED170, own virtual slot 7 NewScriptFrame (0x006E7410) twice when the
//   script runner (0x013D2838) exists.
// Shape: a same-TU LowerBound stand-in lets the lookup key share the dead flag slot (chunk 242 idiom); the class-name compare keeps its bool in bl across the two string dtors; the result string lives in the dead
//   container slot and the literal in the dead flag slot; the add-to-inventory arm returns the callee's bool directly.
class CCharStringData { public:
    char*         PData;                    // +0x00
    unsigned long StringLength;             // +0x04
};
long __fastcall CompareStrings(const char* a, const char* b);   // 0x00411570
class CCharString { public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
    bool SameText(const CCharString& other) const
    {
        if (PStringData->StringLength != other.PStringData->StringLength)
            return false;
        return CompareStrings(PStringData->PData, other.PStringData->PData) == 0;
    }
    __forceinline bool operator==(const CCharString& other) const     // the landed IsActionRunning 0x00712D70 form
    {
        if (PStringData == other.PStringData)
            return true;
        if (PStringData && other.PStringData)
            return SameText(other);
        return false;
    }
};
struct CTCBase_Retail {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21();
    virtual CCharString GetClassName() const;           // slot 22 (+0x58)
};
class CTCContainer : public CTCBase_Retail { public: bool AddItemsToInventory(); };                   // 0x008ED170
class CTCContainerRewardHero : public CTCContainer { public: bool RewardHeroWithItems(); };           // 0x0074EE90
struct CInterfaceMap_Retail {
    struct SEntry { long type; CTCBase_Retail* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);   // 0x0040F020
};
#pragma auto_inline(off)
// Same-TU stand-in for CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound (STLport lower_bound over the 8-byte entries):
// compiled above the caller so VC7.1 knows the key's address does not escape and packs the frame words as retail
// (the chunk 242 / landed GetItemDefNamesFromContainer 0x008A8F20 idiom).
CInterfaceMap_Retail::SEntry* CInterfaceMap_Retail::LowerBound(const long* type)
{
    SEntry* it = first;
    int len = int(last - first);
    while (len > 0) {
        int half = len / 2;
        SEntry* middle = it + half;
        if (middle->type < *type) {
            it = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return it;
}
#pragma auto_inline(on)
struct CThing_Retail {
    unsigned char         _pad00[0x20];
    unsigned long         m_presentBits[9];         // +0x20
    CInterfaceMap_Retail  m_interfaces;             // +0x44
    unsigned char         _pad4c[0x45];
    unsigned char         m_dead : 1;               // +0x91 bit 0
    unsigned char         m_91_1 : 7;
    unsigned long HasInterface(long interfaceType) const { return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)); }
    __forceinline CTCBase_Retail* GetInterface(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap_Retail* map = &m_interfaces;
        CInterfaceMap_Retail::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return it->component;
    }
};
class CThingPlayerCreature : public CThing_Retail { };
class CScriptThing { public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010();
    virtual CThing_Retail* GetThing() const;        // slot 11 (+0x2c)
    virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027();
    virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035();
    virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043();
    virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051();
    virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067();
    virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074();
    virtual bool IsValid() const;                  // slot 75 (+0x12c)
};
class CPlayer { public: CThingPlayerCreature* GetPControlledCreature() const; };      // 0x00487DC0
class CPlayerManager { public: CPlayer* GetMainPlayer(); };    // 0x00449970
struct CScriptRunner;
extern CScriptRunner* g_pScriptRunner;                  // 0x013D2838
class CGameScriptInterface { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06();
    virtual void NewScriptFrame();                      // slot 7 (+0x1c, 0x006E7410)
    virtual bool GiveHeroItemsFromContainer(const CScriptThing& container, bool addToInventory) const;
    void*            World;                             // +0x04 (engine/CGameScriptInterface.h)
    void*            Component;                         // +0x08
    void*            DisplayEngine;                     // +0x0c
    void*            DefinitionManager;                 // +0x10
    CPlayerManager*  PlayerManager;                     // +0x14
};

bool CGameScriptInterface::GiveHeroItemsFromContainer(const CScriptThing& container, bool addToInventory) const
{
    CThing_Retail* hero = PlayerManager->GetMainPlayer()->GetPControlledCreature();
    if (hero && !hero->m_dead && container.IsValid()) {
        if (!addToInventory) {
            CThing_Retail* thing = container.GetThing();
            if (thing->HasInterface(0x3c)) {
                CTCBase_Retail* tc = thing->GetInterface(0x3c);
                bool isReward;
                {
                    CCharString rewardClass("CTCContainerRewardHero");
                    isReward = tc->GetClassName() == rewardClass;
                }
                if (isReward && static_cast<CTCContainerRewardHero*>(tc)->RewardHeroWithItems()) {
                    if (g_pScriptRunner) {
                        const_cast<CGameScriptInterface*>(this)->NewScriptFrame();
                        const_cast<CGameScriptInterface*>(this)->NewScriptFrame();
                    }
                    return true;
                }
            }
        } else {
            CThing_Retail* thing = container.GetThing();
            if (thing->HasInterface(0x3c))
                return static_cast<CTCContainer*>(thing->GetInterface(0x3c))->AddItemsToInventory();
        }
    }
    return false;
}