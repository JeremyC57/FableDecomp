// NScript::CV_ExposeMayorScript::Main() -- the expose-the-mayor quest's main: bind five entity scripts ("EM_SuspiciousVillager", "EM_Sheriff", "EM_FlashLightPoint", "EM_Rhodri", "HH_CellarDoor"), finish adding scripted entities, activate the "V_ExposeMayorRhodri" quest, spawn DoMission and PreventConflictWithTCE, then declare the gossip categories
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E75BA0, 848 bytes. Catalog label "CQ_SummoningTheShipScript::Main" is propagated:
//   the row's only reference is the vtable at 0x012E96D4, its strings are the EM_ (Expose Mayor) entities, and it
//   ends in the landed DeclareGossipCategories 0x00E76040 (the mayor gossip categories; landed under the
//   propagated CGameflowScript label).
// Callees: operator new 0x00BFEA1A, CCharString(const char*, long) 0x0099EBF0 / CCharString(const CCharString&)
//   0x0099EC30 / ~CCharString 0x0099EAE0, CScriptBase::AddEntityScriptBinding 0x00CB8230, CScriptBase::
//   PostAddScriptedEntities 0x00CB8930, CSpawnedFuncBase 0x00CDD450 (CSpawnedFunc vtable 0x012E96E0),
//   CScriptBase::AddSpawnedFunction 0x00CB7E50; entity Allocs 0x00E76360 0x00E76470 0x00E76730 0x00E765D0 0x00E76800;
//   DoMission 0x00E7A410, PreventConflictWithTCE 0x00E75EF0, DeclareGossipCategories 0x00E76040; game script
//   interface PostAddScriptedEntities (slot 64, +0x100) and ActivateQuest (slot 276, +0x450).
// Layout: CEntityScriptBinding<CV_ExposeMayorScript> (0x1C bytes, vtable 0x012E97DC) as the landed
//   CQ_DragonBossFightScript::Main 0x00D255E0; the first two bindings carry Flags 1.
// Shape: each name string is a temporary guarded by its own bit of one flag word (ebx: 1 .. 0x40).
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};
void* __cdecl operator new(unsigned int size);          // 0x00BFEA1A
// Layout: CSpawnedFuncBase is the retail polymorphic suspendable-process chain from the shared headers
//   (FABLE_RETAIL_SUSPENDABLE_PROCESS opt-in: CSpawnedFuncBase : CSuspendableProcessMicrothread : CASuspendableProcess :
//   CBaseClassNonCopyable : CBaseClass, 0x34 bytes, vtable 0x012C2F9C; ctor 0x00CDD450).
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#include "engine/CSpawnedFuncBase.h"
template <class T> struct CSpawnedFunc : CSpawnedFuncBase {
    void (T::*Func)();                                  // +0x34
    T* PClass;                                          // +0x38
    CSpawnedFunc(const CCharString& name, unsigned long id, T* pclass, void (T::*func)())
        : CSpawnedFuncBase(name, id), Func(func), PClass(pclass) {}
    virtual void SuspendableProcess() { (PClass->*Func)(); }
};

class CScriptThing;
class CScriptDataBase;
class CEntityScriptBase;
class CActiveEntityScriptBase;
template <class T> class CCountedPointer { public: T* Data; void* Info; };

class __declspec(novtable) CEntityScriptBindingBase { public:
    virtual const CCharString& GetEntityScriptName() const = 0;
};
template <class T> struct CEntityScriptBinding : CEntityScriptBindingBase {
    typedef CCountedPointer<CEntityScriptBase> (__fastcall *TAllocFunc)(T& parent, CScriptDataBase* data, const CScriptThing& thing);
    CCharString     EntityScriptName;                   // +0x04
    T*              PParentScript;                      // +0x08
    void (T::*Func)();                                  // +0x0c
    TAllocFunc      ClassAllocFunc;                     // +0x10
    bool            ClassAllocValid;                    // +0x14
    unsigned long   Flags;                              // +0x18
    CEntityScriptBinding(const CCharString& name, T* parent, TAllocFunc alloc, unsigned long flags)
        : EntityScriptName(name), PParentScript(parent), ClassAllocFunc(alloc), ClassAllocValid(true), Flags(flags) {}
    virtual const CCharString& GetEntityScriptName() const { return EntityScriptName; }
};

class CGameScriptInterface { public:                          // vtable 0x01260F0C
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055();
    virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063();
    virtual void PostAddScriptedEntities() const;   // slot 64 (+0x100)
    virtual void v065();
    virtual void v066();
    virtual void v067();
    virtual void v068();
    virtual void v069();
    virtual void v070();
    virtual void v071();
    virtual void v072();
    virtual void v073();
    virtual void v074();
    virtual void v075();
    virtual void v076();
    virtual void v077();
    virtual void v078();
    virtual void v079();
    virtual void v080();
    virtual void v081();
    virtual void v082();
    virtual void v083();
    virtual void v084();
    virtual void v085();
    virtual void v086();
    virtual void v087();
    virtual void v088();
    virtual void v089();
    virtual void v090();
    virtual void v091();
    virtual void v092();
    virtual void v093();
    virtual void v094();
    virtual void v095();
    virtual void v096();
    virtual void v097();
    virtual void v098();
    virtual void v099();
    virtual void v100();
    virtual void v101();
    virtual void v102();
    virtual void v103();
    virtual void v104();
    virtual void v105();
    virtual void v106();
    virtual void v107();
    virtual void v108();
    virtual void v109();
    virtual void v110();
    virtual void v111();
    virtual void v112();
    virtual void v113();
    virtual void v114();
    virtual void v115();
    virtual void v116();
    virtual void v117();
    virtual void v118();
    virtual void v119();
    virtual void v120();
    virtual void v121();
    virtual void v122();
    virtual void v123();
    virtual void v124();
    virtual void v125();
    virtual void v126();
    virtual void v127();
    virtual void v128();
    virtual void v129();
    virtual void v130();
    virtual void v131();
    virtual void v132();
    virtual void v133();
    virtual void v134();
    virtual void v135();
    virtual void v136();
    virtual void v137();
    virtual void v138();
    virtual void v139();
    virtual void v140();
    virtual void v141();
    virtual void v142();
    virtual void v143();
    virtual void v144();
    virtual void v145();
    virtual void v146();
    virtual void v147();
    virtual void v148();
    virtual void v149();
    virtual void v150();
    virtual void v151();
    virtual void v152();
    virtual void v153();
    virtual void v154();
    virtual void v155();
    virtual void v156();
    virtual void v157();
    virtual void v158();
    virtual void v159();
    virtual void v160();
    virtual void v161();
    virtual void v162();
    virtual void v163();
    virtual void v164();
    virtual void v165();
    virtual void v166();
    virtual void v167();
    virtual void v168();
    virtual void v169();
    virtual void v170();
    virtual void v171();
    virtual void v172();
    virtual void v173();
    virtual void v174();
    virtual void v175();
    virtual void v176();
    virtual void v177();
    virtual void v178();
    virtual void v179();
    virtual void v180();
    virtual void v181();
    virtual void v182();
    virtual void v183();
    virtual void v184();
    virtual void v185();
    virtual void v186();
    virtual void v187();
    virtual void v188();
    virtual void v189();
    virtual void v190();
    virtual void v191();
    virtual void v192();
    virtual void v193();
    virtual void v194();
    virtual void v195();
    virtual void v196();
    virtual void v197();
    virtual void v198();
    virtual void v199();
    virtual void v200();
    virtual void v201();
    virtual void v202();
    virtual void v203();
    virtual void v204();
    virtual void v205();
    virtual void v206();
    virtual void v207();
    virtual void v208();
    virtual void v209();
    virtual void v210();
    virtual void v211();
    virtual void v212();
    virtual void v213();
    virtual void v214();
    virtual void v215();
    virtual void v216();
    virtual void v217();
    virtual void v218();
    virtual void v219();
    virtual void v220();
    virtual void v221();
    virtual void v222();
    virtual void v223();
    virtual void v224();
    virtual void v225();
    virtual void v226();
    virtual void v227();
    virtual void v228();
    virtual void v229();
    virtual void v230();
    virtual void v231();
    virtual void v232();
    virtual void v233();
    virtual void v234();
    virtual void v235();
    virtual void v236();
    virtual void v237();
    virtual void v238();
    virtual void v239();
    virtual void v240();
    virtual void v241();
    virtual void v242();
    virtual void v243();
    virtual void v244();
    virtual void v245();
    virtual void v246();
    virtual void v247();
    virtual void v248();
    virtual void v249();
    virtual void v250();
    virtual void v251();
    virtual void v252();
    virtual void v253();
    virtual void v254();
    virtual void v255();
    virtual void v256();
    virtual void v257();
    virtual void v258();
    virtual void v259();
    virtual void v260();
    virtual void v261();
    virtual void v262();
    virtual void v263();
    virtual void v264();
    virtual void v265();
    virtual void v266();
    virtual void v267();
    virtual void v268();
    virtual void v269();
    virtual void v270();
    virtual void v271();
    virtual void v272();
    virtual void v273();
    virtual void v274();
    virtual void v275();
    virtual void ActivateQuest(const CCharString& name) const;   // slot 276 (+0x450)
};

namespace NScript {
}   // namespace NScript (CScriptBase is global in retail)
class CScriptBase { public:                                    // retail 0x40 bytes
    unsigned char _body[0x40];
    void AddEntityScriptBinding(CEntityScriptBindingBase* binding);                   // 0x00CB8230
    void PostAddScriptedEntities();                                                   // 0x00CB8930
    void SpawnFunction(CSpawnedFuncBase* func, const char* regionName = "") { AddSpawnedFunction(func, CCharString(regionName)); }
public:
    void AddSpawnedFunction(CSpawnedFuncBase*, const CCharString&);   // 0x00CB7E50
};
namespace NScript {
class CV_ExposeMayorScript : public CScriptBase { public:
    struct CEM_SuspiciousVillager {
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_ExposeMayorScript& parent, CScriptDataBase* data, const CScriptThing& thing);   // 0x00E76360
    };
    struct CEM_Sheriff {
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_ExposeMayorScript& parent, CScriptDataBase* data, const CScriptThing& thing);   // 0x00E76470
    };
    struct CEM_FlashLightPoint {
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_ExposeMayorScript& parent, CScriptDataBase* data, const CScriptThing& thing);   // 0x00E76730
    };
    struct CEM_Rhodri {
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_ExposeMayorScript& parent, CScriptDataBase* data, const CScriptThing& thing);   // 0x00E765D0
    };
    struct CHH_CellarDoor {
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_ExposeMayorScript& parent, CScriptDataBase* data, const CScriptThing& thing);   // 0x00E76800
    };
    CGameScriptInterface* GSI;                          // +0x40
    void*                 Data;                         // +0x44
    void Main();
    void DoMission();   // 0x00E7A410
    void PreventConflictWithTCE();   // 0x00E75EF0
    void DeclareGossipCategories();   // 0x00E76040
};
void CV_ExposeMayorScript::Main()
{
    AddEntityScriptBinding(new CEntityScriptBinding<CV_ExposeMayorScript>("EM_SuspiciousVillager", this, &CEM_SuspiciousVillager::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_ExposeMayorScript>("EM_Sheriff", this, &CEM_Sheriff::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_ExposeMayorScript>("EM_FlashLightPoint", this, &CEM_FlashLightPoint::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_ExposeMayorScript>("EM_Rhodri", this, &CEM_Rhodri::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_ExposeMayorScript>("HH_CellarDoor", this, &CHH_CellarDoor::Alloc, 0));
    PostAddScriptedEntities();
    GSI->PostAddScriptedEntities();
    GSI->ActivateQuest("V_ExposeMayorRhodri");
    SpawnFunction(new CSpawnedFunc<CV_ExposeMayorScript>("DoMission", 0, this, &CV_ExposeMayorScript::DoMission));
    SpawnFunction(new CSpawnedFunc<CV_ExposeMayorScript>("PreventConflictWithTCE", 0, this, &CV_ExposeMayorScript::PreventConflictWithTCE));
    DeclareGossipCategories();
}
}