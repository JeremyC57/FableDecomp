// NScript::CV_BodyGuardScript::CBodyGuard::OnPredicateFail() -- a henchman is gone: drop him from the henchman list and his health bar, stop his wages if hired; when he was killed (or sacrificed) mark his type dead and start its 240-second respawn timer
// Leaf: ?OnPredicateFail@CBodyGuard@CV_BodyGuardScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E5FB80, 232 bytes, thiscall, plain `ret` (private virtual, entity vtable slot 5).
// Identity (proven): slot 5 (OnPredicateFail) of the RTTI-less entity vtable 0x012E701C, which the landed
//   constructor 0x00E5DDE0 (manifest label CDarkwoodTrader::CDarkwoodTrader is propagated) of the "BodyGuard"
//   binding's Alloc 0x00E5DD60 (bound by NScript::CV_BodyGuardScript::Main 0x00E5D4F0 at 0x00E5D53C) stores (site 0x00E5DE01); Ego_r.pdb has ?OnPredicateFail@CBodyGuard@CV_BodyGuardScript@NScript@@EAEXXZ.
//   Entity slot order (landed Init / OnPredicateFail rows, Ego_r.pdb CEntityScriptBase): 0 dtor, 1 Main, 2 Init,
//   3 GetParentScript, 4 OnPersist, 5 OnPredicateFail, 6 OnInterrupted.
// Layout: Ego_r.pdb CBodyGuard (entity offsets equal retail): ParentClass +0x14, PMasterData +0x18, members from +0x1c;
//   the parent level script's members sit at Ego_r offset - 0x14 (retail CScriptBase is 0x40).
// Callees: CScriptThing::operator== (Ego_r.pdb ??8CScriptThing@@QBE_NABV0@@Z, inline) through the implementation's slot 78
//   IsEqualTo; std::vector<CScriptThing>::erase(iterator) 0x00CD3152; interface slot 338 RemoveQuestInfoElement;
//   CBodyGuard::GetGoldCost 0x00E5FC70 (Ego_r.pdb ?GetGoldCost@CBodyGuard@CV_BodyGuardScript@NScript@@AAEJXZ, the only
//   long() member of the class; landed under the descriptive name GetHenchmanGoldCost); CScriptThing slot 18 MsgIsKilledBy
//   ("" 0x0122D70E temporary); CTimer::operator=(long) inline: g_pGameScriptInterface 0x0143E8F8 slot 89 SetTimer.
// Parent members: Ego_r.pdb CV_BodyGuardScript - 0x14 (HenchmanList is the one vector before the arrays).
class CCharString {
public:
    void* PData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class CCounter {                                        // Ego_r.pdb: one long
public:
    long RelevantWorldFrame;
};
namespace NScript {
class CQ_SunnyvaleMasterData {                          // Ego_r.pdb layout (offsets equal retail at the head)
public:
    virtual ~CQ_SunnyvaleMasterData();
    long PostSavePosition;                              // +0x04 (EGameflowPosition)
    long HenchmanCommentaryState;                       // +0x08
    long HenchmanOverrideState;                         // +0x0c
    bool HenchmanSacrificedBodge;                       // +0x10
};
}
class CBaseClass {                                      // vtable 0x01231710
public:
    CBaseClass() { }
    virtual ~CBaseClass();
};
class CCPPointerInfo {
public:
    unsigned long RefCount;
};
template <class T> class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    T* operator->() const { return Data; }
    T& operator*() const { return *Data; }
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
};
class CScriptThing : public CBaseClass {                // 12 bytes, vtable 0x01238C8C
public:
    virtual ~CScriptThing();                            // slot 0
    virtual void t01(); virtual void t02(); virtual void t03(); virtual void t04(); virtual void t05(); virtual void t06(); virtual void t07(); virtual void t08();
    virtual void t09(); virtual void t10(); virtual void t11(); virtual void t12(); virtual void t13(); virtual void t14(); virtual void t15(); virtual void t16();
    virtual void t17();
    virtual bool MsgIsKilledBy(const CCharString& killer) const;   // slot 18 (+0x48)
    virtual void t19(); virtual void t20(); virtual void t21(); virtual void t22(); virtual void t23(); virtual void t24(); virtual void t25(); virtual void t26();
    virtual void t27(); virtual void t28(); virtual void t29(); virtual void t30(); virtual void t31(); virtual void t32(); virtual void t33(); virtual void t34();
    virtual void t35(); virtual void t36(); virtual void t37(); virtual void t38(); virtual void t39(); virtual void t40(); virtual void t41(); virtual void t42();
    virtual void t43(); virtual void t44(); virtual void t45(); virtual void t46(); virtual void t47(); virtual void t48(); virtual void t49(); virtual void t50();
    virtual void t51(); virtual void t52(); virtual void t53(); virtual void t54(); virtual void t55(); virtual void t56(); virtual void t57(); virtual void t58();
    virtual void t59(); virtual void t60(); virtual void t61(); virtual void t62(); virtual void t63(); virtual void t64(); virtual void t65(); virtual void t66();
    virtual void t67(); virtual void t68(); virtual void t69(); virtual void t70(); virtual void t71(); virtual void t72(); virtual void t73(); virtual void t74();
    virtual void t75(); virtual void t76(); virtual void t77();
    virtual bool IsEqualTo(const CScriptThing* other) const;   // slot 78 (+0x138) (on the implementation)
    bool operator==(const CScriptThing& other) const { return PImp->IsEqualTo(&*other.PImp); }
    CCountedPointer<CScriptThing> PImp;                 // +0x04 Data, +0x08 Info
};
namespace std {
template <class T> class allocator { };
template <class T, class A = allocator<T> > class vector {   // STLport 4.6: start, finish, end of storage (0xC)
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() { return _M_start; }
    T* end() { return _M_finish; }
    T* erase(T* position);                              // 0x00CD3152 for vector<CScriptThing>
};
}

class CGameScriptInterface {                     // the script interface vtable (slot names: ghidra_out/cgsi_slots_aligned.tsv)
public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055();
    virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063();
    virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071();
    virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079();
    virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086(); virtual void v087();
    virtual void v088();
    virtual void SetTimer(long timer, long time);                                   // slot 89 (+0x164)
    virtual void v090(); virtual void v091(); virtual void v092(); virtual void v093(); virtual void v094(); virtual void v095(); virtual void v096(); virtual void v097();
    virtual void v098(); virtual void v099(); virtual void v100(); virtual void v101(); virtual void v102(); virtual void v103(); virtual void v104(); virtual void v105();
    virtual void v106(); virtual void v107(); virtual void v108(); virtual void v109(); virtual void v110(); virtual void v111(); virtual void v112(); virtual void v113();
    virtual void v114(); virtual void v115(); virtual void v116(); virtual void v117(); virtual void v118(); virtual void v119(); virtual void v120(); virtual void v121();
    virtual void v122(); virtual void v123(); virtual void v124(); virtual void v125(); virtual void v126(); virtual void v127(); virtual void v128(); virtual void v129();
    virtual void v130(); virtual void v131(); virtual void v132(); virtual void v133(); virtual void v134(); virtual void v135(); virtual void v136(); virtual void v137();
    virtual void v138(); virtual void v139(); virtual void v140(); virtual void v141(); virtual void v142(); virtual void v143(); virtual void v144(); virtual void v145();
    virtual void v146(); virtual void v147(); virtual void v148(); virtual void v149(); virtual void v150(); virtual void v151(); virtual void v152(); virtual void v153();
    virtual void v154(); virtual void v155(); virtual void v156(); virtual void v157(); virtual void v158(); virtual void v159(); virtual void v160(); virtual void v161();
    virtual void v162(); virtual void v163(); virtual void v164(); virtual void v165(); virtual void v166(); virtual void v167(); virtual void v168(); virtual void v169();
    virtual void v170(); virtual void v171(); virtual void v172(); virtual void v173(); virtual void v174(); virtual void v175(); virtual void v176(); virtual void v177();
    virtual void v178(); virtual void v179(); virtual void v180(); virtual void v181(); virtual void v182(); virtual void v183(); virtual void v184(); virtual void v185();
    virtual void v186(); virtual void v187(); virtual void v188(); virtual void v189(); virtual void v190(); virtual void v191(); virtual void v192(); virtual void v193();
    virtual void v194(); virtual void v195(); virtual void v196(); virtual void v197(); virtual void v198(); virtual void v199(); virtual void v200(); virtual void v201();
    virtual void v202(); virtual void v203(); virtual void v204(); virtual void v205(); virtual void v206(); virtual void v207(); virtual void v208(); virtual void v209();
    virtual void v210(); virtual void v211(); virtual void v212(); virtual void v213(); virtual void v214(); virtual void v215(); virtual void v216(); virtual void v217();
    virtual void v218(); virtual void v219(); virtual void v220(); virtual void v221(); virtual void v222(); virtual void v223(); virtual void v224(); virtual void v225();
    virtual void v226(); virtual void v227(); virtual void v228(); virtual void v229(); virtual void v230(); virtual void v231(); virtual void v232(); virtual void v233();
    virtual void v234(); virtual void v235(); virtual void v236(); virtual void v237(); virtual void v238(); virtual void v239(); virtual void v240(); virtual void v241();
    virtual void v242(); virtual void v243(); virtual void v244(); virtual void v245(); virtual void v246(); virtual void v247(); virtual void v248(); virtual void v249();
    virtual void v250(); virtual void v251(); virtual void v252(); virtual void v253(); virtual void v254(); virtual void v255(); virtual void v256(); virtual void v257();
    virtual void v258(); virtual void v259(); virtual void v260(); virtual void v261(); virtual void v262(); virtual void v263(); virtual void v264(); virtual void v265();
    virtual void v266(); virtual void v267(); virtual void v268(); virtual void v269(); virtual void v270(); virtual void v271(); virtual void v272(); virtual void v273();
    virtual void v274(); virtual void v275(); virtual void v276(); virtual void v277(); virtual void v278(); virtual void v279(); virtual void v280(); virtual void v281();
    virtual void v282(); virtual void v283(); virtual void v284(); virtual void v285(); virtual void v286(); virtual void v287(); virtual void v288(); virtual void v289();
    virtual void v290(); virtual void v291(); virtual void v292(); virtual void v293(); virtual void v294(); virtual void v295(); virtual void v296(); virtual void v297();
    virtual void v298(); virtual void v299(); virtual void v300(); virtual void v301(); virtual void v302(); virtual void v303(); virtual void v304(); virtual void v305();
    virtual void v306(); virtual void v307(); virtual void v308(); virtual void v309(); virtual void v310(); virtual void v311(); virtual void v312(); virtual void v313();
    virtual void v314(); virtual void v315(); virtual void v316(); virtual void v317(); virtual void v318(); virtual void v319(); virtual void v320(); virtual void v321();
    virtual void v322(); virtual void v323(); virtual void v324(); virtual void v325(); virtual void v326(); virtual void v327(); virtual void v328(); virtual void v329();
    virtual void v330(); virtual void v331(); virtual void v332(); virtual void v333(); virtual void v334(); virtual void v335(); virtual void v336(); virtual void v337();
    virtual void RemoveQuestInfoElement(long a0) const;   // slot 338 (+0x548)
};
extern CGameScriptInterface* g_pGameScriptInterface;    // 0x0143E8F8 (the global script interface, bound name)
class CTimer {                                          // Ego_r.pdb: one long TimerIndex
public:
    long TimerIndex;
    CTimer& operator=(long time) { g_pGameScriptInterface->SetTimer(TimerIndex, time); return *this; }
};
class CScriptBase;
class CPersistContext;
class CEntityScriptBase {                               // Ego_r.pdb: vptr, Game +0x04, Me +0x08 (20 bytes)
public:
    virtual ~CEntityScriptBase();                       // slot 0
    virtual void Main() = 0;                            // slot 1
    virtual void Init();                                // slot 2
    virtual CScriptBase* GetParentScript() const = 0;   // slot 3
    virtual void OnPersist(CPersistContext& context);   // slot 4
    virtual void OnPredicateFail();                     // slot 5
    virtual void OnInterrupted();                       // slot 6
    bool IsActiveThreadTerminating() const;             // 0x00F35B30
    CGameScriptInterface* Game;                         // +0x04
    CScriptThing          Me;                           // +0x08
};
class CPersistContext;
class CScriptBase {                                     // retail 0x40 bytes (Ego_r 0x54: debug containers)
public:
    virtual ~CScriptBase();                             // slot 0
protected:
    virtual void RegisterMain();                        // slot 1
    virtual void Main();                                // slot 2
    virtual void Init();                                // slot 3
public:
    virtual void OnPersist(CPersistContext& context);   // slot 4
    bool IsActiveThreadTerminating();                   // 0x00CB7940
    unsigned char _body[0x3c];
};
template <class TData> class CLevelScriptBase : public CScriptBase {
public:
    CGameScriptInterface* Game;                         // +0x40
    TData*                MasterScriptData;             // +0x44
};

namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_BodyGuardScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    long TotalWages;                                    // +0x48
    bool HeroCannotAffordHelp;                          // +0x4c
    bool HeroWieldingWeapon;                            // +0x4d
    CCounter HeroStoodStillCounter;                     // +0x50
    CTimer HenchmanLastIdleChatTimer;                   // +0x54
    CTimer HenchmanLastBoredChatTimer;                  // +0x58
    CTimer CommentaryTimer;                             // +0x5c
    CTimer HenchmanFreakedOutThePlayerTimer;            // +0x60
    std::vector<CScriptThing> HenchmanList;             // +0x64
    long ConversationIndex;                             // +0x70
    CTimer HenchmanDeadRespawnTimer[4];                 // +0x74
    long HenchmanGlobalStatus[4];                       // +0x84
    class CBodyGuard;
};
class CV_BodyGuardScript::CBodyGuard : public CEntityScriptBase {
public:
    CV_BodyGuardScript* ParentClass;   // +0x14
    CQ_SunnyvaleMasterData* PMasterData;   // +0x18
    long HenchManType;                                  // +0x1c
    long BrainState;                                    // +0x20
    long CurrentHealthState;                            // +0x24
    long PreviousHealthState;                           // +0x28
    bool FreakedOut;                                    // +0x2c
    CTimer AngerTimer;                                  // +0x30
    CTimer AttackedBySummonersTimer;                    // +0x34
    long MyHealthBarID;                                 // +0x38
private:
    long GetGoldCost();                                 // 0x00E5FC70
private:
    virtual void OnPredicateFail();
};

void CV_BodyGuardScript::CBodyGuard::OnPredicateFail()
{
    for (CScriptThing* it = ParentClass->HenchmanList.begin(); it != ParentClass->HenchmanList.end(); ++it) {
        if (Me == *it) {
            ParentClass->HenchmanList.erase(it);
            break;
        }
    }
    Game->RemoveQuestInfoElement(MyHealthBarID);
    if (BrainState == 2)
        ParentClass->TotalWages -= GetGoldCost();
    BrainState = 1;
    if (Me.MsgIsKilledBy("") || PMasterData->HenchmanSacrificedBodge) {
        ParentClass->HenchmanGlobalStatus[HenchManType] = 1;
        ParentClass->HenchmanDeadRespawnTimer[HenchManType] = 240;
    }
}
}