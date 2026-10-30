// CGameScriptInterface::GetHeroFishingLevel() const -- the fishing level (+0xc) of the live hero's fishing component (interface type 0x68); -1 without one
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00895B00, length 89 bytes.
// Callees: the hero thing through the player (0x00449970 on PlayerManager, then 0x00487DC0; both labels propagated), live (+0x91 bit 0 clear), the inlined CThing::GetInterface (the presence bitset at +0x20, CVectorMap::LowerBound @ 0x004365B0 on the map at +0x44, end or a larger key -> the end entry). Shape: a `push ecx` frame for the lookup key, `or eax,-1` for the shared failure exit, `ret`.
// Leaf: GetHeroFishingLevel
struct CDisplayEngine_Retail;
class CPlayerManager;
struct CMainGameComponent_Retail;
struct CDefinitionManager_Retail;
class CWorld;
class CCharString;
class CScriptThing;
struct CDefPointer_Retail;
struct CTCBase_Retail;
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                 // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44: {first, last}
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const ETCInterfaceType& type) const;   // 0x004365B0 (the second copy of the 0x0040F020 body)
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap_Retail;
struct CThingPhysics_Retail;
struct CThing_Retail {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037();
    virtual CThing_Retail* GetStandingOnThing();    // slot 38 (+0x98)
    unsigned char         _pad04[0xc];              // +0x04 .. +0x10
    unsigned char         m_type;                   // +0x10 (1 = the player creature, 2 = an AI creature, 3 = a building)
    unsigned char         _pad11[0xf];
    unsigned long         m_presentBits[9];         // +0x20 std::bitset of the interface types present
    CInterfaceMap_Retail  m_interfaces;             // +0x44 the sorted {type, component} map
    unsigned char         _pad4c[0x14];
    CThingPhysics_Retail* m_pPhysics;               // +0x60
    CThingPhysics_Retail* GetPhysics() { return m_pPhysics; }
    unsigned char         _pad64[8];
    unsigned char         m_flags;                  // +0x6c (bit 1 = physical, bit 3 = creature)
    unsigned char         _pad6d[0x24];
    unsigned char         m_dead : 1;               // +0x91 bit 0
    unsigned char         m_91_1 : 7;
    unsigned char         m_92_0 : 8;               // +0x92
    unsigned char         m_93_0 : 1;               // +0x93
    unsigned char         m_killedByScript : 1;     //       bit 1
    unsigned char         m_93_2 : 6;
    void Kill(bool flag);                           // 0x004C9B80 (CThing::Kill)
    CScriptThing GetScriptThing();                  // 0x004C7E50 (CThing::GetScriptThing, by value through the hidden result pointer)
    void AssignHomeBuilding(CThing_Retail* building);   // 0x00512210 (landed as AssignTarget_00512210; the label "CVillageTaskCarryCrate::AssignCreatureToTask" is propagated)
    void SetNewBrain(CDefPointer_Retail brain);     // 0x00833010 (CThingAICreature::SetNewBrain, the counted def pointer by value)
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap_Retail* map = &m_interfaces;   // named pointer: retail advances esi to the map once
            const CInterfaceMap_Retail::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (T*)((CTCBase_Retail*)it->second);
            return true;
        }
        return false;
    }
    unsigned long HasInterface(long interfaceType) const { return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)); }   // the masked word (a bool would narrow the test)
};
class CThingPlayerCreature : public CThing_Retail { };
class CPlayer { public:
    CThingPlayerCreature* GetPControlledCreature() const;               // 0x00487DC0 (the label is propagated; the landed IsHeroNaked helper)
};
class CPlayerManager { public:
    CPlayer& GetMainPlayer();                 // 0x00449970
};
struct CTCHeroFishing_Retail {                 // interface type 0x68
    unsigned char _pad[0xc];
    long          m_fishingLevel;                // +0xc
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

long CGameScriptInterface::GetHeroFishingLevel() const
{
    CThing_Retail* hero = PlayerManager->GetMainPlayer().GetPControlledCreature();
    if (hero && !hero->m_dead) {
        CTCHeroFishing_Retail* fishing;
        if (hero->GetInterface(&fishing, 0x68))
            return fishing->m_fishingLevel;
    }
    return -1;
}