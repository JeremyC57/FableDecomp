// CV_ChapelOfEvilScript::WatchForAcolyteHit(CScriptThing acolyte) -- with an is-alive predicate on the acolyte, flag AttackedByHero whenever the hero hits him and, the first time, have the sound altar deliver the TEXT_QST_019_WEAPON_02 warning
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E6AF20, length 725 bytes, `ret 0Ch` (the by-value CScriptThing).
// Callees: CScriptThing(const CScriptThing&) 0x004ABE90 (the predicate's copy), CScriptBase::SetPredicate 0x00CB7920,
//   ~CIsThingAlive 0x00E241B0, CScriptBase::IsActiveThreadTerminating 0x00CB7940, CCharString(const char*, long) 0x0099EBF0 /
//   ~CCharString 0x0099EAE0, ~CBaseClass 0x0099A2E0, operator delete 0x00BFE9BC, ~CScriptThing 0x004AA840 (out of line at
//   the in-loop returns); the thing implementation's slots 21 MsgIsHitBy (+0x54), 43
//   MsgIsHitByAnyAggressiveSpecialAbilityFrom (+0xac), 75 IsAlive (+0x12c) behind the inlined null-safe wrappers; GameCalls
//   slots 7 NewScriptFrame, 70 GetHero, 72 GetThingWithScriptName (by value), 364 AddNewConversation, 365
//   AddPersonToConversation, 366 AddLineToConversation. Strings "M_SoundAltar", "SCRIPT_NAME_HERO", "TEXT_QST_019_WEAPON_02".
// Layout: Ego_r.pdb CV_ChapelOfEvilScript minus 0x14 (retail CScriptBase): AttackedByHero +0x5a,
//   AcolyteAttackMessageComplete +0x5b. CScriptThing and its wrappers as the landed
//   CV_TalentlessBardScript::WatchForAttack 0x00ED9790.
// Shape: generated script code; ~CScriptThing is __forceinline and the two in-loop terminate returns sit under
//   `#pragma inline_depth(0)` (out-of-line destructor), the prologue return and the loop exit expand it.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);        // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
void operator delete(void* p);                              // 0x00BFE9BC (MSVCR71)
class CCPPointerInfo { public:                                     // CCountedPointer's shared block
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);              // destroys the pointee
    void* Data;
};
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};
struct CScriptThingImpl {                                   // the game-side thing behind PImp.Data (CScriptThing vtable layout)
    virtual void i000(); virtual void i001(); virtual void i002(); virtual void i003(); virtual void i004(); virtual void i005(); virtual void i006(); virtual void i007();
    virtual void i008(); virtual void i009(); virtual void i010(); virtual void i011(); virtual void i012(); virtual void i013(); virtual void i014(); virtual void i015();
    virtual void i016(); virtual void i017();
    virtual bool MsgIsKilledBy(const CCharString& killer) const;             // slot 18 (+0x48)
    virtual void i019(); virtual void i020();
    virtual bool MsgIsHitBy(const CCharString& attacker) const;              // slot 21 (+0x54)
    virtual void i022(); virtual void i023(); virtual void i024(); virtual void i025(); virtual void i026(); virtual void i027(); virtual void i028(); virtual void i029();
    virtual void i030(); virtual void i031(); virtual void i032(); virtual void i033(); virtual void i034(); virtual void i035(); virtual void i036(); virtual void i037();
    virtual void i038(); virtual void i039(); virtual void i040(); virtual void i041(); virtual void i042();
    virtual bool MsgIsHitByAnyAggressiveSpecialAbilityFrom(const CCharString& attacker) const; // slot 43 (+0xac)
    virtual void i044();
    virtual void i045();
    virtual void i046();
    virtual void i047();
    virtual void i048();
    virtual void i049();
    virtual void i050();
    virtual void i051();
    virtual void i052();
    virtual void i053();
    virtual void i054();
    virtual void i055();
    virtual void i056();
    virtual void i057();
    virtual void i058();
    virtual void i059();
    virtual void i060();
    virtual void i061();
    virtual void i062();
    virtual void i063();
    virtual void i064();
    virtual void i065();
    virtual void i066();
    virtual void i067();
    virtual void i068();
    virtual void i069();
    virtual void i070();
    virtual void i071();
    virtual void i072();
    virtual void i073();
    virtual void i074();
    virtual bool IsAlive() const;                                                      // slot 75 (+0x12c)
};
class CScriptThing : public CBaseClass { public:                          // vtable 0x01238C8C, 12 bytes
    CCountedPointer<CScriptThingImpl> PImp;                 // +0x04 Data, +0x08 Info
    CScriptThing(const CScriptThing& other);                // 0x004ABE90
    __forceinline ~CScriptThing() {}                                      // out of line 0x004AA840
    bool IsAlive() const                                    // 0x004AB130
    {
        if (!PImp.Data)
            return false;
        return PImp.Data->IsAlive();
    }
    bool MsgIsKilledBy(const CCharString& killer) const     // 0x004AAAF0
    {
        if (!PImp.Data)
            return false;
        return PImp.Data->MsgIsKilledBy(killer);
    }
    bool MsgIsHitBy(const CCharString& attacker) const      // 0x004AAB30
    {
        if (!PImp.Data)
            return false;
        return PImp.Data->MsgIsHitBy(attacker);
    }
    bool MsgIsHitByAnyAggressiveSpecialAbilityFrom(const CCharString& attacker) const   // 0x004AAD60
    {
        if (!PImp.Data)
            return false;
        return PImp.Data->MsgIsHitByAnyAggressiveSpecialAbilityFrom(attacker);
    }
};
struct GameCalls {                                       // CGameScriptInterface, retail vtable 0x01260F0C
 virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
 virtual void NewScriptFrame(void) const; //slot7 0x006e7410
 virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
 virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
 virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
 virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
 virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
 virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055();
 virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063();
 virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069();
 virtual CScriptThing & GetHero(void); //slot70 0x00891ca0
 virtual void v071();
 virtual CScriptThing GetThingWithScriptName(CCharString const &) const; //slot72 0x008a7d60
 virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079(); virtual void v080();
 virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086(); virtual void v087(); virtual void v088();
 virtual void v089(); virtual void v090(); virtual void v091(); virtual void v092(); virtual void v093(); virtual void v094(); virtual void v095(); virtual void v096();
 virtual void v097(); virtual void v098(); virtual void v099(); virtual void v100(); virtual void v101(); virtual void v102(); virtual void v103(); virtual void v104();
 virtual void v105(); virtual void v106(); virtual void v107(); virtual void v108(); virtual void v109(); virtual void v110(); virtual void v111(); virtual void v112();
 virtual void v113(); virtual void v114(); virtual void v115(); virtual void v116(); virtual void v117(); virtual void v118(); virtual void v119(); virtual void v120();
 virtual void v121(); virtual void v122(); virtual void v123(); virtual void v124(); virtual void v125(); virtual void v126(); virtual void v127(); virtual void v128();
 virtual void v129(); virtual void v130(); virtual void v131(); virtual void v132(); virtual void v133(); virtual void v134(); virtual void v135(); virtual void v136();
 virtual void v137(); virtual void v138(); virtual void v139(); virtual void v140(); virtual void v141(); virtual void v142(); virtual void v143(); virtual void v144();
 virtual void v145(); virtual void v146(); virtual void v147(); virtual void v148(); virtual void v149(); virtual void v150(); virtual void v151(); virtual void v152();
 virtual void v153(); virtual void v154(); virtual void v155(); virtual void v156(); virtual void v157(); virtual void v158(); virtual void v159(); virtual void v160();
 virtual void v161(); virtual void v162(); virtual void v163(); virtual void v164(); virtual void v165(); virtual void v166(); virtual void v167(); virtual void v168();
 virtual void v169(); virtual void v170(); virtual void v171(); virtual void v172(); virtual void v173(); virtual void v174(); virtual void v175(); virtual void v176();
 virtual void v177(); virtual void v178(); virtual void v179(); virtual void v180(); virtual void v181(); virtual void v182(); virtual void v183(); virtual void v184();
 virtual void v185(); virtual void v186(); virtual void v187(); virtual void v188(); virtual void v189(); virtual void v190(); virtual void v191(); virtual void v192();
 virtual void v193(); virtual void v194(); virtual void v195(); virtual void v196(); virtual void v197(); virtual void v198(); virtual void v199(); virtual void v200();
 virtual void v201(); virtual void v202(); virtual void v203(); virtual void v204(); virtual void v205(); virtual void v206(); virtual void v207(); virtual void v208();
 virtual void v209(); virtual void v210(); virtual void v211(); virtual void v212(); virtual void v213(); virtual void v214(); virtual void v215(); virtual void v216();
 virtual void v217(); virtual void v218(); virtual void v219(); virtual void v220(); virtual void v221(); virtual void v222(); virtual void v223(); virtual void v224();
 virtual void v225(); virtual void v226(); virtual void v227(); virtual void v228(); virtual void v229(); virtual void v230(); virtual void v231(); virtual void v232();
 virtual void v233(); virtual void v234(); virtual void v235(); virtual void v236(); virtual void v237(); virtual void v238(); virtual void v239(); virtual void v240();
 virtual void v241(); virtual void v242(); virtual void v243(); virtual void v244(); virtual void v245(); virtual void v246(); virtual void v247(); virtual void v248();
 virtual void v249(); virtual void v250(); virtual void v251(); virtual void v252(); virtual void v253(); virtual void v254(); virtual void v255(); virtual void v256();
 virtual void v257(); virtual void v258(); virtual void v259(); virtual void v260(); virtual void v261(); virtual void v262(); virtual void v263(); virtual void v264();
 virtual void v265(); virtual void v266(); virtual void v267(); virtual void v268(); virtual void v269(); virtual void v270(); virtual void v271(); virtual void v272();
 virtual void v273(); virtual void v274(); virtual void v275(); virtual void v276(); virtual void v277(); virtual void v278(); virtual void v279(); virtual void v280();
 virtual void v281(); virtual void v282(); virtual void v283(); virtual void v284(); virtual void v285(); virtual void v286(); virtual void v287(); virtual void v288();
 virtual void v289(); virtual void v290(); virtual void v291(); virtual void v292(); virtual void v293(); virtual void v294(); virtual void v295(); virtual void v296();
 virtual void v297(); virtual void v298(); virtual void v299(); virtual void v300(); virtual void v301(); virtual void v302(); virtual void v303(); virtual void v304();
 virtual void v305(); virtual void v306(); virtual void v307(); virtual void v308(); virtual void v309(); virtual void v310(); virtual void v311(); virtual void v312();
 virtual void v313(); virtual void v314(); virtual void v315(); virtual void v316(); virtual void v317(); virtual void v318(); virtual void v319(); virtual void v320();
 virtual void v321(); virtual void v322(); virtual void v323(); virtual void v324(); virtual void v325(); virtual void v326(); virtual void v327(); virtual void v328();
 virtual void v329(); virtual void v330(); virtual void v331(); virtual void v332(); virtual void v333(); virtual void v334(); virtual void v335(); virtual void v336();
 virtual void v337(); virtual void v338(); virtual void v339(); virtual void v340(); virtual void v341(); virtual void v342(); virtual void v343(); virtual void v344();
 virtual void v345(); virtual void v346(); virtual void v347(); virtual void v348(); virtual void v349(); virtual void v350(); virtual void v351(); virtual void v352();
 virtual void v353(); virtual void v354(); virtual void v355(); virtual void v356(); virtual void v357(); virtual void v358(); virtual void v359(); virtual void v360();
 virtual void v361(); virtual void v362(); virtual void v363();
 virtual long AddNewConversation(CScriptThing const &, bool, bool) const; //slot364 0x008906c0
 virtual void AddPersonToConversation(long, CScriptThing const &) const; //slot365 0x00890710
 virtual void AddLineToConversation(long, CCharString const &, bool, CScriptThing const &, CScriptThing const &) const; //slot366 0x00890750
};
namespace NScript {
class __declspec(novtable) CPredicateBase { public:               // vtable 0x012C2FF4 (never stored: novtable)
    virtual bool IsTrue() = 0;
};
}
namespace NScript {
class CIsThingAlive : public NScript::CPredicateBase { public:            // vtable 0x012C32F0, 16 bytes
    CScriptThing Thing;                                     // +0x04
    CIsThingAlive(const CScriptThing& thing) : Thing(thing) {}
    virtual ~CIsThingAlive();                                       // 0x00E241B0
    virtual bool IsTrue();
};
}
using NScript::CIsThingAlive;
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptBase.h"
struct CV_ChapelOfEvilScript : CScriptBase {
    GameCalls*    Game;                                     // +0x40
    void*         Owner;                                    // +0x44
    unsigned char _pad48[0x5a - 0x48];
    bool          AttackedByHero;                           // +0x5a
    bool          AcolyteAttackMessageComplete;             // +0x5b
    void WatchForAcolyteHit(CScriptThing acolyte);
};

void CV_ChapelOfEvilScript::WatchForAcolyteHit(CScriptThing acolyte)
{
    SetPredicate(CIsThingAlive(acolyte));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    CScriptThing altar = Game->GetThingWithScriptName("M_SoundAltar");
    AcolyteAttackMessageComplete = false;
    while (!IsActiveThreadTerminating()) {
        bool hit = acolyte.MsgIsHitBy("SCRIPT_NAME_HERO") || acolyte.MsgIsHitByAnyAggressiveSpecialAbilityFrom("SCRIPT_NAME_HERO");
        if (hit) {
#pragma inline_depth(0)
            if (IsActiveThreadTerminating())
                return;
#pragma inline_depth()
            AttackedByHero = true;
            if (!AcolyteAttackMessageComplete && altar.IsAlive()) {
#pragma inline_depth(0)
                if (IsActiveThreadTerminating())
                    return;
#pragma inline_depth()
                long conversation = Game->AddNewConversation(altar, false, false);
                Game->AddPersonToConversation(conversation, Game->GetHero());
                Game->AddLineToConversation(conversation, "TEXT_QST_019_WEAPON_02", false, altar, Game->GetHero());
                AcolyteAttackMessageComplete = true;
            }
        }
        Game->NewScriptFrame();
    }
}