// CGameScriptInterface::SetRegionEntranceAsActive(const CScriptThing& thing, bool active) const -- when the valid script thing holds a region-entrance component (kind 99: +0x2c bit 3), set the active byte (+0xc) of the object at the thing's +0x5c
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0089EF00, length 48 bytes.
// Callees: the script thing's slots 75 (+0x12c) and 11 (+0x2c).
// Shape: no map lookup (the object pointer sits at +0x5c), the kind mask test folded to `test byte ptr [eax+2Ch],8`, `ret 8`.
struct CTCBase_Retail;
struct CTCBase_Retail;
struct STCEntry_Retail {                           // one CVectorMap<ETCInterfaceType, CTCBase*> element
    long           m_kind;
    CTCBase_Retail* m_pTC;
};
struct CTCMap_Retail {                             // the thing's component map at +0x44: a sorted vector of pairs
    STCEntry_Retail* m_pFirst;
    STCEntry_Retail* m_pLast;
    STCEntry_Retail* m_pEnd;
    STCEntry_Retail* LowerBound(const unsigned long& kind);         // 0x0040F020 (CVectorMap<ETCInterfaceType,...>::LowerBound; the key by address)
    STCEntry_Retail* LowerBound(const unsigned long& kind) const;   // 0x004365B0 (the second instantiation)
    __forceinline CTCBase_Retail* Find(long kind)          // the inlined map `find`: lower bound (the key converted into a stack temporary), then end when the key is not there
    {
        STCEntry_Retail* it = LowerBound((unsigned long)kind);
        if (it == m_pLast || kind < it->m_kind)
            it = m_pLast;
        return it->m_pTC;
    }
    __forceinline CTCBase_Retail* Find(long kind) const
    {
        STCEntry_Retail* it = LowerBound((unsigned long)kind);
        if (it == m_pLast || kind < it->m_kind)
            it = m_pLast;
        return it->m_pTC;
    }
};
struct CThing_Retail {
    unsigned char  _pad00[0x10];
    unsigned char  m_type;                         // +0x10 (1 = the player creature)
    unsigned char  _pad11[0xf];
    unsigned long  m_tcMask[8];                    // +0x20: which component kinds the thing holds
    unsigned char  _pad40[4];
    CTCMap_Retail  m_tcs;                          // +0x44
    unsigned char  _pad50[0xc];
    void*          m_pRegionLink;                  // +0x5c
    unsigned char  _pad60[0x31];
    unsigned char  m_dead : 1;                     // +0x91 bit 0
    unsigned char  m_91_1 : 7;
    __forceinline unsigned long HasTC(long kind) const { return m_tcMask[kind >> 5] & (1UL << (kind & 31)); }   // the masked word (a bool return would materialise `shr ; and 1`)
};
class CThingPlayerCreature : public CThing_Retail { };
__forceinline CTCBase_Retail* GetTC(CThing_Retail* thing, long kind) { return thing->m_tcs.Find(kind); }          // free inline: the pointer comes back in eax
__forceinline CTCBase_Retail* GetTCConst(const CThing_Retail* thing, long kind) { return thing->m_tcs.Find(kind); }
struct CRegionExit_Retail {
    unsigned char _pad[0xc];
    bool m_active;                                 // +0x0c
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
    void SetRegionEntranceAsActive(const CScriptThing& thing, bool active) const;
};

void CGameScriptInterface::SetRegionEntranceAsActive(const CScriptThing& thing, bool active) const
{
    if (thing.IsAlive()) {
        CThing_Retail* t = ((CThing_Retail*)thing.GetPThing());
        if (t->HasTC(99))
            ((CRegionExit_Retail*)t->m_pRegionLink)->m_active = active;
    }
}