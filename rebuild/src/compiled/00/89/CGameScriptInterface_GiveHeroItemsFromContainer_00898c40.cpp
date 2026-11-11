// CGameScriptInterface::GiveHeroItemsFromContainer(const CScriptThing& container, bool addToInventory) const -- with a live hero and a valid container: either move the container's items into the inventory (CTCContainer::AddItemsToInventory), or, when the container component is a CTCContainerRewardHero, reward the hero with them and let two script frames pass
// Leaf: ?GiveHeroItemsFromContainer@CGameScriptInterface@@UBE_NABVCScriptThing@@_N@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00898C40, length 353 bytes (`ret 8`, bool in al).
// Callees: CPlayerManager::GetMainPlayer @ 0x00449970, CPlayer::GetPControlledCreature @ 0x00487DC0, the script thing's slots 75
//   (+0x12C IsValid) and 11 (+0x2C GetThing), the component-map LowerBound @ 0x0040F020 (TC 0x3C, the container), the
//   component's virtual slot 22 (+0x58, class name as a CCharString by value), CCharString ctor/dtor @ 0x0099EBF0 /
//   0x0099EAE0, the string compare @ 0x00411570 (`__fastcall`), CTCContainerRewardHero::RewardHeroWithItems @ 0x0074EE90,
//   CTCContainer::AddItemsToInventory @ 0x008ED170, own virtual slot 7 NewScriptFrame (0x006E7410) twice when the
//   script runner (0x013D2838) exists.
// Shape: a same-TU LowerBound stand-in lets the lookup key share the dead flag slot (chunk 242 idiom); the class-name compare keeps its bool in bl across the two string dtors; the result string lives in the dead
//   container slot and the literal in the dead flag slot; the add-to-inventory arm returns the callee's bool directly.
#include "fable_compiler.h"
class CCharStringData { public:
    char*         PData;                    // +0x00
    unsigned long StringLength;             // +0x04
};
template <class T> class CBasicString { public:                       // Ego_r.pdb: the string base of CCharString (data and inline comparisons)
    CCharStringData* PStringData;
protected:
    static long __fastcall Compare(const T* a, const T* b);   // 0x00411570 (Ego_r.pdb ?Compare@?$CBasicString@D@@KIJPBD0@Z, protected static)
public:
    bool SameText(const CBasicString& other) const
    {
        if (PStringData->StringLength != other.PStringData->StringLength)
            return false;
        return Compare(PStringData->PData, other.PStringData->PData) == 0;
    }
};
class CCharString : public CBasicString<char> { public:
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
    FABLE_FORCEINLINE bool operator==(const CCharString& other) const     // the landed IsActionRunning 0x00712D70 form
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
class CTCBase;
// agentNF16: the stand-in CInterfaceMap_Retail struct is the explicit specialisation of CVectorMap<ETCInterfaceType, CTCBase*> (the landed
//   0x0074EE90 idiom); 0x0040F020 is its LowerBound (NF14 / established spelling).
enum ETCInterfaceType { };
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class C> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> value_type;
    value_type* _M_start;                                   // +0x00 (the STLport vector)
    value_type* _M_finish;                                  // +0x04
    value_type* LowerBound(const ETCInterfaceType& key);    // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap_Retail;
#pragma auto_inline(off)
// Same-TU stand-in for CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound (STLport lower_bound over the 8-byte entries):
// compiled above the caller so VC7.1 knows the key's address does not escape and packs the frame words as retail
// (the chunk 242 / landed GetItemDefNamesFromContainer 0x008A8F20 idiom).
CInterfaceMap_Retail::value_type* CInterfaceMap_Retail::LowerBound(const ETCInterfaceType& key)
{
    value_type* it = _M_start;
    int len = int(_M_finish - _M_start);
    while (len > 0) {
        int half = len / 2;
        value_type* middle = it + half;
        if (middle->first < key) {
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
    FABLE_FORCEINLINE CTCBase_Retail* GetInterface(long interfaceType)
    {
        ETCInterfaceType type = (ETCInterfaceType)interfaceType;
        CInterfaceMap_Retail* map = &m_interfaces;
        CInterfaceMap_Retail::value_type* it = map->LowerBound(type);
        if (it == map->_M_finish || interfaceType < it->first)
            it = map->_M_finish;
        return (CTCBase_Retail*)it->second;   // agentNF16: the specialisation's mapped type is CTCBase*
    }
};
class CThingPlayerCreature : public CThing_Retail { };
#include "engine/CCountedPointer.h"
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
class CPlayer { public: CThingPlayerCreature* GetPControlledCreature() const; };      // 0x00487DC0
// CPlayerManager: the shared model engine/CPlayerManager.h (Ego_r.pdb CPlayerManager; the five accessors declared
//   there with their retail addresses: GetPlayer 0x004498C0 / 0x00449910, GetMainPlayer 0x00449970 / 0x00449960,
//   GetMainPlayerNumber 0x00449700).
#include "engine/CPlayerManager.h"
    // 0x00449970
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