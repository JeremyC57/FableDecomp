// CGameScriptInterface::float EntityGetHearingRadius(const CScriptThing& thing) const -- the hearing radius of the valid script thing's hearing component (kind 0x34, 0x00744440); 0.0 without the component
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0089D340, length 96 bytes.
// Callees: the script thing validated through its virtual slot 75 (+0x12c), the thing fetched through slot 11 (+0x2c); the component map (+0x44) LowerBound @ 0x004365B0 with the kind 52 by address (a stack temporary), the inlined `find` tail (`cmp end ; cmp key ; jle`), the component's radius getter @ 0x00744440 (a float in st0); the constant 0.0f @ 0x0122DEDC.
// Shape: the map address in esi, the entry's value word at +4; the component-kind mask test folded onto the +0x24 word (bit 20).
// Leaf: ?EntityGetHearingRadius@CGameScriptInterface@@UBEMABVCScriptThing@@@Z
struct CTCBase_Retail;
extern const float g_zero_0122dedc;           // 0x0122DEDC (0.0f)
struct CTCBase_Retail;
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
typedef std::pair<ETCInterfaceType, CTCBase*> STCEntry_Retail;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                             // the thing's component map at +0x44: a sorted vector of pairs
    STCEntry_Retail* m_pFirst;
    STCEntry_Retail* m_pLast;
    STCEntry_Retail* m_pEnd;
    STCEntry_Retail* LowerBound(const ETCInterfaceType& kind);         // 0x0040F020 (CVectorMap<ETCInterfaceType,...>::LowerBound; the key by address)
    const STCEntry_Retail* LowerBound(const ETCInterfaceType& kind) const;   // 0x004365B0 (the second instantiation)
    __forceinline CTCBase_Retail* Find(long kind)          // the inlined map `find`: lower bound (the key converted into a stack temporary), then end when the key is not there
    {
        const STCEntry_Retail* it = LowerBound((ETCInterfaceType)kind);
        if (it == m_pLast || kind < it->first)
            it = m_pLast;
        return ((CTCBase_Retail*)it->second);
    }
    __forceinline CTCBase_Retail* Find(long kind) const
    {
        const STCEntry_Retail* it = LowerBound((ETCInterfaceType)kind);
        if (it == m_pLast || kind < it->first)
            it = m_pLast;
        return ((CTCBase_Retail*)it->second);
    }
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CTCMap_Retail;
struct CThing_Retail {
    unsigned char  _pad00[0x10];
    unsigned char  m_type;                         // +0x10 (1 = the player creature)
    unsigned char  _pad11[0xf];
    unsigned long  m_tcMask[8];                    // +0x20: which component kinds the thing holds
    unsigned char  _pad40[4];
    CTCMap_Retail  m_tcs;                          // +0x44
    unsigned char  _pad50[0x41];
    unsigned char  m_dead : 1;                     // +0x91 bit 0
    unsigned char  m_91_1 : 7;
    __forceinline unsigned long HasTC(long kind) const { return m_tcMask[kind >> 5] & (1UL << (kind & 31)); }   // the masked word (a bool return would materialise `shr ; and 1`)
};
class CThingPlayerCreature : public CThing_Retail { };
__forceinline CTCBase_Retail* GetTC(CThing_Retail* thing, long kind) { return thing->m_tcs.Find(kind); }          // free inline: the pointer comes back in eax
__forceinline CTCBase_Retail* GetTCConst(const CThing_Retail* thing, long kind) { return thing->m_tcs.Find(kind); }
struct CTCHearing_Retail {                           // the component (no vtable modelled: the bytes before the first used member are padding)
    float GetHearingRadius();                      // 0x00744440 (labelled "CMap::DrawGetSeaLevel", propagated)
};
class CCPPointerInfo;
template <class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; };   // Ego_r.pdb members (Data, Info)
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
class CPlayer { public:
    CThingPlayerCreature* GetPControlledCreature() const;              // 0x00487DC0 (the manifest label "_Cons_val<...>" is propagated)
    const CThingPlayerCreature* PeekPControlledCreature() const;        // 0x00487DD0 (the manifest label "_Cons_val<...>" is propagated)
};
// CPlayerManager: the shared model engine/CPlayerManager.h (Ego_r.pdb CPlayerManager; the five accessors declared
//   there with their retail addresses: GetPlayer 0x004498C0 / 0x00449910, GetMainPlayer 0x00449970 / 0x00449960,
//   GetMainPlayerNumber 0x00449700).
#include "engine/CPlayerManager.h"
struct CDisplayEngine_Retail; class CWorld;
class CGameScriptInterface { public:
    virtual void v000();
    CWorld*                 World;                           // +0x04 (engine/CGameScriptInterface.h)
    long                    Component;
    CDisplayEngine_Retail*  DisplayEngine;                   // +0x0c
    long                    DefinitionManager;
    CPlayerManager*  PlayerManager;                   // +0x14
    unsigned char           _pad18[4];
    bool                    m_handLampLit;                   // +0x1c (read when the hero has no hand-lamp component)
    float EntityGetHearingRadius(const CScriptThing& thing) const;
};

float CGameScriptInterface::EntityGetHearingRadius(const CScriptThing& thing) const
{
    if (thing.IsAlive()) {
        CThing_Retail* t = ((CThing_Retail*)thing.GetPThing());
        if (t->HasTC(0x34)) {
            CTCHearing_Retail* tc = (CTCHearing_Retail*)GetTCConst(t, 0x34);
            return tc->GetHearingRadius();
        }
    }
    return g_zero_0122dedc;
}