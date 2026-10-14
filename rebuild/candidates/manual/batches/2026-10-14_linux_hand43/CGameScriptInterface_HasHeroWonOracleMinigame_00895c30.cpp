// CGameScriptInterface::HasHeroWonOracleMinigame() const -- the "success" byte (+0xa8, engine/CTCOracleMinigame.h) of the live hero's oracle-minigame component (interface type 0x107); false without one
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00895C30, length 93 bytes.
// Callees: the hero thing through the player (0x00449970 on PlayerManager, then 0x00487DC0; both labels propagated), live (+0x91 bit 0 clear), the inlined CThing::GetInterface (the presence bitset at +0x20, CVectorMap::LowerBound @ 0x0040F020 on the map at +0x44, end or a larger key -> the end entry). Shape: a `push ecx` frame for the lookup key, the presence bit tested as the sign of the byte at +0x40 (`jns`), the false exits folded into `xor al,al`, `ret`.
// Leaf: HasHeroWonOracleMinigame
struct CDisplayEngine_Retail;
struct CPlayerManager_Retail;
struct CMainGameComponent_Retail;
struct CDefinitionManager_Retail;
struct CWorld;
struct CCharString;
struct CScriptThing;
struct CDefPointer_Retail;
#include "engine/CTCOracleMinigame.h"
struct CTCBase_Retail;
struct CInterfaceMap_Retail {                 // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44: {first, last}
    struct SEntry { long type; CTCBase_Retail* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);   // 0x0040F020 (a second copy of the same body sits at 0x004365B0)
};
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
            long type = interfaceType;
            CInterfaceMap_Retail* map = &m_interfaces;   // named pointer: retail advances esi to the map once
            CInterfaceMap_Retail::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
    unsigned long HasInterface(long interfaceType) const { return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)); }   // the masked word (a bool would narrow the test)
};
struct CPlayer_Retail {
    CThing_Retail* GetHeroThing();               // 0x00487DC0 (the label is propagated; the landed IsHeroNaked helper)
};
struct CPlayerManager_Retail {
    CPlayer_Retail* GetPlayer();                 // 0x00449970 (labelled "Catch@0040d959", propagated)
};
struct CGameScriptInterface {
    virtual void v000();
    CWorld*                     World;              // +0x04 (engine/CGameScriptInterface.h)
    CMainGameComponent_Retail*  Component;          // +0x08
    CDisplayEngine_Retail*      DisplayEngine;      // +0x0c
    CDefinitionManager_Retail*  DefinitionManager;  // +0x10
    CPlayerManager_Retail*      PlayerManager;      // +0x14
    long GetTextIDFromSymbol(const CCharString& symbol) const;   // 0x006E77F0 (labelled LTextSystem::GetIDFromSymbol; `this` is unused)
    long GetCurrentWorldFrameForMessages() const;   // 0x006E7510 (labelled GetMaxWorldFrameForMessages)
    long GetMaxWorldFrameForMessages() const;       // 0x006E7530
    bool HasHeroWonOracleMinigame() const;
};

bool CGameScriptInterface::HasHeroWonOracleMinigame() const
{
    CThing_Retail* hero = PlayerManager->GetPlayer()->GetHeroThing();
    if (hero && !hero->m_dead) {
        CTCOracleMinigame* minigame;
        if (hero->GetInterface(&minigame, 0x107))
            return minigame->Success;
    }
    return false;
}
