// CQ_TraderConflictGoodScript::WatchForRegionTransitions() -- each region load: prepare the region creatures and set their combat range, sleep and Bandits faction
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString { public:
    void* PStringData;
    CCharString();                                 // 0x0099E4B0
    CCharString(const char* text, long length);    // 0x0099EBF0
    ~CCharString();                                // 0x0099EAE0
};
class CThing;
struct SCount { long m_references; };
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
struct CCPPointerImp {                             // CCPPointer<CScriptThingImp>: Data, Info (memberwise copy adds a reference)
    CThing* Data;
    SCount* Info;
    CCPPointerImp(const CCPPointerImp& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->m_references; }
};
class CScriptThing : public CBaseClass { public:                 // vtable 0x01238C8C, 12 bytes
    virtual void s00();
    CCPPointerImp PImp;                            // +4
    // the copy constructor is the compiler-generated memberwise one
};
template <class T> struct CRetailVector {          // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    T* begin() { return _M_start; }
    T& operator[](unsigned long n) { return *(begin() + n); }
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern const char g_factionBandits[];              // 0x0125FE68 "FACTION_BANDITS"
class CScriptBase { public:
    bool IsActiveThreadTerminating();              // 0x00CB7940
};
namespace NScript {
class CQ_TraderConflictGoodScript : public CScriptBase {
public:
    unsigned char              _pad_0x00[0x40];
    CGameScriptInterface*      Game;                        // +0x40
    void*                      Data;                        // +0x44
    CRetailVector<CScriptThing> AllCreatures;               // +0x48
    long                       TradersFollowing;            // +0x54
    long                       TradersReachedTeleporter;    // +0x58
    long                       BanditSecurityLinesSaid;     // +0x5c
    bool                       FirstRegionDead;             // +0x60
    bool                       EnteredNewRegion;            // +0x61
    bool                       IntroDone;                   // +0x62
    bool                       OutroStart;                  // +0x63
    bool                       OutroDone;                   // +0x64
    void PrepareRegionCreatures();                          // 0x00DFC320
    void WatchForRegionTransitions();
};
} // namespace NScript
using NScript::CQ_TraderConflictGoodScript;
void NScript::CQ_TraderConflictGoodScript::WatchForRegionTransitions()
{
    CCharString region;
    while (!OutroDone) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        if (Game->MsgOnRegionLoaded(region)) {
            if (IsActiveThreadTerminating())
                return;
            PrepareRegionCreatures();
            for (unsigned long i = 0; i < AllCreatures.size(); ++i) {
                if (IsActiveThreadTerminating())
                    return;
                Game->SetCombatNearbyBreakOffRange(AllCreatures[i], 10.0f);
                Game->EntitySetSleepEnabled(AllCreatures[i], false);
                CCharString faction(g_factionBandits, -1);
                Game->EntitySetInFaction(AllCreatures[i], faction);
            }
            if (IsActiveThreadTerminating())
                return;
            EnteredNewRegion = true;
        }
    }
    if (IsActiveThreadTerminating())
        return;
}