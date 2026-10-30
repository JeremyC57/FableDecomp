// Grade: RELOCATION_MATCH (Fable landing 2026-10-19 after the Codex review hold: test checks an unrelated local struct; header comment describes another function; names CBaseObject ctor 0x99a380 as CBaseIntelligentPointer and vtable 0x127094c (CScriptGameResourceObjectScriptedThingBase) as CIntelligentThing)
// NScript::CV_BordelloScript::CV_BordelloScript(void* data, CGameScriptInterface* game) -- quest-script ctor: script base and
//   interface/data pair, vtable 0x012E38B0, the BordelloVillage handle, the four seh_ entity handles and the empty
//   cut-scene argument map
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E398F0, `ret 8`. Identity: its factory 0x00E46A00 (`push 0C0h`) is registered at
//   0x00CD8539 under "S_VB" / "V_Bordello"; the manifest label "CQ_HangingTreeGoodScript" is propagated (that PDB class has
//   no such members) and the old header text described CQ_WizardBattleScript.
// Callees: CScriptBase::CScriptBase 0x00CB8110, CBaseClass::CBaseClass 0x0099A2D0 (CScriptThing base; vtable 0x01238C8C =
//   CScriptThing), CBaseObject::CBaseObject 0x0099A380 (CScriptGameResourceObjectScriptedThingBase base; vtable 0x0127094C
//   = CScriptGameResourceObjectScriptedThingBase), CRT malloc 0x00BFEA0E (the map's 0x18-byte header).
// Leaf: ??0CV_BordelloScript
// Layout: PDB (Ego_r) names; retail = PDB - 0x14 (script base): flags/counters +0x48..+0x5b, BordelloVillage +0x5c,
//   WearingWhoreWig..BeerSetToDPad +0x68..+0x73, seh_Boss +0x74, seh_Guard +0x84, seh_Madam +0x94, seh_Whore +0xa4,
//   csargs (12-byte STLport map) +0xb4; size 0xc0. Model shared with the landed CV_ChickenKickingScript 0x00E62830.
// CGameScriptInterface: the first 88 virtual slots of vtable 0x01260F0C, named from the slot labels (rtti-vtables.tsv +
//   manifest); only the slot position matters here, so the signatures are simplified (void, no parameters) except
//   RegisterTimer (slot 87, returns the new timer index). Overloaded names carry a _2 suffix.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (already NScript). Same bytes.
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterface* g_pGameScriptInterface;    // 0x0143E8F8
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
struct __declspec(novtable) CQuestScriptObject : CScriptBase {   // the argument-carrying intermediate base (landed model)
    CGameScriptInterface* Game;                         // +0x40 (second argument)
    void*                 Data;                         // +0x44 (first argument, the script data)
    CQuestScriptObject(void* data, CGameScriptInterface* game) : CScriptBase(), Game(game), Data(data) { }
};
// CTimer: the retail model from engine/CTimer.h (Ego_r.pdb CTimer {long TimerIndex}, 0x4; the setter is the retail
//   operator=(long), Ego_r 0x00CCE720). Bodies the row expands inline follow the include.
#include "engine/CTimer.h"
inline CTimer::CTimer() : TimerIndex(g_pGameScriptInterface->RegisterTimer()) { }
extern "C" void* __cdecl malloc(unsigned int size);    // 0x00BFEA0E
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
#include "engine/CCountedPointer.h"
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
inline CScriptThing::CScriptThing() : CBaseClass(), PImp() { }
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
#include "engine/CScriptGameResourceObjectBase.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
struct _Rb_tree_node_base { char _M_color; _Rb_tree_node_base* _M_parent; _Rb_tree_node_base* _M_left; _Rb_tree_node_base* _M_right; };
template<unsigned int NodeSize> struct _Rb_tree_base {
    _Rb_tree_node_base* _M_header;
    _Rb_tree_base() : _M_header(0) { _M_header = (_Rb_tree_node_base*)malloc(NodeSize); }
};
template<unsigned int NodeSize> struct CRetailMap : _Rb_tree_base<NodeSize> {   // STLport map (12 bytes in retail)
    unsigned long _M_node_count;
    unsigned char _M_key_compare;
    CRetailMap() : _M_node_count(0) { _M_empty_initialize(); }
    void _M_empty_initialize() {
        this->_M_header->_M_color = 0; this->_M_header->_M_parent = 0;
        this->_M_header->_M_left = this->_M_header; this->_M_header->_M_right = this->_M_header;
    }
};
namespace NScript {
class CV_BordelloScript : public CQuestScriptObject { public:
    bool HeroPartying, HeroTricking, MagicianSleeping, CutscenePlaying, PlayerOwned, BecomeNunnery, PlayerHasLeftRegion,
         HeroFoundDeeds, HeroFoundDeedsLocation;        // +0x48..+0x50
    long ClientsAlive;                                  // +0x54
    bool ClientInUse[3];                                // +0x58
    CScriptThing BordelloVillage;                       // +0x5c
    bool WearingWhoreWig;                               // +0x68
    bool HadSex;                                        // +0x69
    long BeersDrunk;                                    // +0x6c
    bool BeerSetToDPad;                                 // +0x70
    CScriptGameResourceObjectScriptedThingBase seh_Boss;   // +0x74
    CScriptGameResourceObjectScriptedThingBase seh_Guard;  // +0x84
    CScriptGameResourceObjectScriptedThingBase seh_Madam;  // +0x94
    CScriptGameResourceObjectScriptedThingBase seh_Whore;  // +0xa4
    CRetailMap<0x18> csargs;                            // +0xb4 map<CCharString,CCharString>
    virtual ~CV_BordelloScript();
    CV_BordelloScript(void* data, CGameScriptInterface* game);
};
CV_BordelloScript::CV_BordelloScript(void* data, CGameScriptInterface* game)
    : CQuestScriptObject(data, game)
{
}
}