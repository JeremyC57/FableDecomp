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
class CCPPointerInfo;
template <class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; };   // Ego_r.pdb members (Data, Info)
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
class CPlayer { public: CThingPlayerCreature* GetPControlledCreature() const; };      // 0x00487DC0
class CPlayerManager { public: CPlayer& GetMainPlayer(); };    // 0x00449970
struct CScriptRunner;
extern CScriptRunner* g_pScriptRunner;                  // 0x013D2838
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

bool CGameScriptInterface::GiveHeroItemsFromContainer(const CScriptThing& container, bool addToInventory) const
{
    CThing_Retail* hero = PlayerManager->GetMainPlayer().GetPControlledCreature();
    if (hero && !hero->m_dead && container.IsAlive()) {
        if (!addToInventory) {
            CThing_Retail* thing = ((CThing_Retail*)container.GetPThing());
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
            CThing_Retail* thing = ((CThing_Retail*)container.GetPThing());
            if (thing->HasInterface(0x3c))
                return static_cast<CTCContainer*>(thing->GetInterface(0x3c))->AddItemsToInventory();
        }
    }
    return false;
}