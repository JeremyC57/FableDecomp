// CV_TravellingHeroesScript::DoConversation(long line) -- play one line of the Thunder conversation from the script def: the speaker says the dialogue to the hero, the named actor (Thunder/Whisper -> rival, Boy, Girl) plays its animation after the def's delay unless the Thunder scene has started, then wait out the conversation and remove it
// Leaf: ?DoConversation@CV_TravellingHeroesScript@NScript@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EE2A80, 0x325 bytes, `ret 4`.
// Callees: CCharString(const CCharString&) 0x0099EC30 / operator!=(const char*) 0x0099E960 / ~ 0x0099EAE0, Compare 0x00411570
//   (defined here under auto_inline(off): with the body in the TU VC7.1 knows it writes no memory and keeps the actor's PData
//   in esi across the four compares, as retail; declared extern it reloads the string after every call), CBaseClass()
//   0x0099A2D0, ~CScriptThing 0x004AA840, IsActiveThreadTerminating 0x00CB7940; Game slots 7, 70 GetHero, 72
//   GetThingWithScriptName, 364/365/366 conversation add, 367 RemoveConversation, 368 IsConversationActive, 376 Pause; the
//   actor handle's slot 18 (+0x48) animation call (descriptive name).
// Data: script def 0x0143E90C TH_ThunderConv_Speaker / _Dialogue / _Actor / _Animation / _AnimDelay vectors at +0x92c..+0x95c.
// Layout (retail = Ego_r.pdb - 0x14): Game +0x40, seh_Rival +0x48, seh_Girl +0x58, seh_Boy +0x68, ThunderKidsTalking +0x78,
//   ThunderSceneStarted +0x79.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
#include <string.h>
#pragma intrinsic(strcmp)
#pragma auto_inline(off)
long __fastcall Compare_00411570(const char* a, const char* b)   // 0x00411570 (the landed body)
{
    for (;;) {
        char ca = *a;
        if (ca == 0 && *b == 0)
            return 0;
        char cb = *b;
        if (ca < cb)
            return -1;
        if (ca > cb)
            return 1;
        ++a;
        ++b;
    }
}
#pragma auto_inline(on)
#pragma pack(push, 1)
struct CBasicStringChar {                       // CBasicString<char>, 13 bytes
    char*         PData;                        // +0x00
    unsigned long StringLength;                 // +0x04
    unsigned long DataLength : 31;              // +0x08
    bool          UseFastExtend : 1;            // +0x0c
    __forceinline bool operator==(const char* text) const { return Compare_00411570(PData, text) ? false : true; }
};
class CCharStringData { public:                        // 17 bytes
    CBasicStringChar Data;                      // +0x00
    long             NoRefs;                    // +0x0d
};
#pragma pack(pop)
class CCharString { public:
    CCharStringData* PStringData;
    CCharString(const CCharString& other);                  // 0x0099EC30
    ~CCharString();                                         // 0x0099EAE0
    bool operator!=(const char* text) const;                // 0x0099E960
    __forceinline bool operator==(const char* text) const
    {
        if (PStringData == 0) {
            if (strcmp(text, "") == 0)
                return true;
            return false;
        }
        return PStringData->Data == text;
    }
};
extern CCharString g_EmptyCharString;                       // 0x0143E8EC
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
    CCountedPointer() : Data(0), Info(0) { }
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        T* data = other.Data;
        CCPPointerInfo* info = other.Info;
        if (Info != info) {
            if (Info) {
                --Info->RefCount;
                if (Info->RefCount == 0) {
                    Info->DeleteFunc(Info->Data);
                    delete Info;
                }
            }
            Data = data;
            Info = info;
            if (info)
                ++info->RefCount;
        }
        return *this;
    }
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
class CScriptThing : public CBaseClass { public:                          // vtable 0x01238C8C, 12 bytes
    CCountedPointer<CScriptThing> PImp;                     // +0x04 Data, +0x08 Info
    CScriptThing() { }
    CScriptThing(const CScriptThing& other);                // 0x004ABE90
    virtual ~CScriptThing();                                        // 0x004AA840
    virtual void s01();
    virtual CCharString GetDefName() const          // slot 2 (+0x08) 0x004AA8D0
    {
        if (!PImp.Data)
            return g_EmptyCharString;
        return PImp.Data->GetDefName();
    }
    virtual void s03();
    virtual void s04();
    virtual void s05();
    virtual void s06();
    virtual void s07();
    virtual void s08();
    virtual void s09();
    virtual void s10();
    virtual void s11();
    virtual void s12();
    virtual void s13();
    virtual void s14();
    virtual void s15();
    virtual void s16();
    virtual void s17();
    virtual void s18();
    virtual void s19();
    virtual void s20();
    virtual void s21();
    virtual void s22();
    virtual void s23();
    virtual void s24();
    virtual void s25();
    virtual void s26();
    virtual void s27();
    virtual void s28();
    virtual void s29();
    virtual void s30();
    virtual void s31();
    virtual void s32();
    virtual void s33();
    virtual void s34();
    virtual void s35();
    virtual void s36();
    virtual void s37();
    virtual void s38();
    virtual void s39();
    virtual void s40();
    virtual void s41();
    virtual void s42();
    virtual void s43();
    virtual void s44();
    virtual void s45();
    virtual void s46();
    virtual void s47();
    virtual void s48();
    virtual void s49();
    virtual void s50();
    virtual void s51();
    virtual void s52();
    virtual void s53();
    virtual void s54();
    virtual void s55();
    virtual void s56();
    virtual void s57();
    virtual void s58();
    virtual void s59();
    virtual void s60();
    virtual void s61();
    virtual void s62();
    virtual void s63();
    virtual void s64();
    virtual void s65();
    virtual void s66();
    virtual void s67();
    virtual void s68();
    virtual void s69();
    virtual void s70();
    virtual void s71();
    virtual void s72();
    virtual void s73();
    virtual void s74();
    virtual bool IsAlive() const                    // slot 75 (+0x12c) 0x004AB130
    {
        if (!PImp.Data)
            return false;
        return PImp.Data->IsAlive();
    }
    virtual void s76();
    virtual bool IsNull() const                     // slot 77 (+0x134) 0x004AB150
    {
        if (!PImp.Data)
            return true;
        return PImp.Data->IsNull();
    }
};
class CGameScriptInterface { public:                       // retail vtable 0x01260F0C (slots: ghidra_out/cgsi_slots_aligned.tsv)
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
    virtual void NewScriptFrame(void) const;   // slot 7 (+0x1c) 0x006E7410
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017();
    virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027();
    virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037();
    virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057();
    virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067();
    virtual void v068(); virtual void v069();
    virtual CScriptThing & GetHero(void);   // slot 70 (+0x118) 0x00891CA0
    virtual void v071();
    virtual CScriptThing GetThingWithScriptName(CCharString const &) const;   // slot 72 (+0x120) 0x008A7D60
    virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079(); virtual void v080(); virtual void v081(); virtual void v082();
    virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086(); virtual void v087(); virtual void v088(); virtual void v089(); virtual void v090(); virtual void v091(); virtual void v092();
    virtual void v093(); virtual void v094(); virtual void v095(); virtual void v096(); virtual void v097(); virtual void v098(); virtual void v099(); virtual void v100(); virtual void v101(); virtual void v102();
    virtual void v103(); virtual void v104(); virtual void v105(); virtual void v106(); virtual void v107(); virtual void v108(); virtual void v109(); virtual void v110(); virtual void v111(); virtual void v112();
    virtual void v113(); virtual void v114(); virtual void v115(); virtual void v116(); virtual void v117(); virtual void v118(); virtual void v119(); virtual void v120(); virtual void v121(); virtual void v122();
    virtual void v123(); virtual void v124(); virtual void v125(); virtual void v126(); virtual void v127(); virtual void v128(); virtual void v129(); virtual void v130(); virtual void v131(); virtual void v132();
    virtual void v133(); virtual void v134(); virtual void v135(); virtual void v136(); virtual void v137(); virtual void v138(); virtual void v139(); virtual void v140(); virtual void v141(); virtual void v142();
    virtual void v143(); virtual void v144(); virtual void v145(); virtual void v146(); virtual void v147(); virtual void v148(); virtual void v149(); virtual void v150(); virtual void v151(); virtual void v152();
    virtual void v153(); virtual void v154(); virtual void v155(); virtual void v156(); virtual void v157(); virtual void v158(); virtual void v159(); virtual void v160(); virtual void v161(); virtual void v162();
    virtual void v163(); virtual void v164(); virtual void v165(); virtual void v166(); virtual void v167(); virtual void v168(); virtual void v169(); virtual void v170(); virtual void v171(); virtual void v172();
    virtual void v173(); virtual void v174(); virtual void v175(); virtual void v176(); virtual void v177(); virtual void v178(); virtual void v179(); virtual void v180(); virtual void v181(); virtual void v182();
    virtual void v183(); virtual void v184(); virtual void v185(); virtual void v186(); virtual void v187(); virtual void v188(); virtual void v189(); virtual void v190(); virtual void v191(); virtual void v192();
    virtual void v193(); virtual void v194(); virtual void v195(); virtual void v196(); virtual void v197(); virtual void v198(); virtual void v199(); virtual void v200(); virtual void v201(); virtual void v202();
    virtual void v203(); virtual void v204(); virtual void v205(); virtual void v206(); virtual void v207(); virtual void v208(); virtual void v209(); virtual void v210(); virtual void v211(); virtual void v212();
    virtual void v213(); virtual void v214(); virtual void v215(); virtual void v216(); virtual void v217(); virtual void v218(); virtual void v219(); virtual void v220(); virtual void v221(); virtual void v222();
    virtual void v223(); virtual void v224(); virtual void v225(); virtual void v226(); virtual void v227(); virtual void v228(); virtual void v229(); virtual void v230(); virtual void v231(); virtual void v232();
    virtual void v233(); virtual void v234(); virtual void v235(); virtual void v236(); virtual void v237(); virtual void v238(); virtual void v239(); virtual void v240(); virtual void v241(); virtual void v242();
    virtual void v243(); virtual void v244(); virtual void v245(); virtual void v246(); virtual void v247(); virtual void v248(); virtual void v249(); virtual void v250(); virtual void v251(); virtual void v252();
    virtual void v253(); virtual void v254(); virtual void v255(); virtual void v256(); virtual void v257(); virtual void v258(); virtual void v259(); virtual void v260(); virtual void v261(); virtual void v262();
    virtual void v263(); virtual void v264(); virtual void v265(); virtual void v266(); virtual void v267(); virtual void v268(); virtual void v269(); virtual void v270(); virtual void v271(); virtual void v272();
    virtual void v273(); virtual void v274(); virtual void v275(); virtual void v276(); virtual void v277(); virtual void v278(); virtual void v279(); virtual void v280(); virtual void v281(); virtual void v282();
    virtual void v283(); virtual void v284(); virtual void v285(); virtual void v286(); virtual void v287(); virtual void v288(); virtual void v289(); virtual void v290(); virtual void v291(); virtual void v292();
    virtual void v293(); virtual void v294(); virtual void v295(); virtual void v296(); virtual void v297(); virtual void v298(); virtual void v299(); virtual void v300(); virtual void v301(); virtual void v302();
    virtual void v303(); virtual void v304(); virtual void v305(); virtual void v306(); virtual void v307(); virtual void v308(); virtual void v309(); virtual void v310(); virtual void v311(); virtual void v312();
    virtual void v313(); virtual void v314(); virtual void v315(); virtual void v316(); virtual void v317(); virtual void v318(); virtual void v319(); virtual void v320(); virtual void v321(); virtual void v322();
    virtual void v323(); virtual void v324(); virtual void v325(); virtual void v326(); virtual void v327(); virtual void v328(); virtual void v329(); virtual void v330(); virtual void v331(); virtual void v332();
    virtual void v333(); virtual void v334(); virtual void v335(); virtual void v336(); virtual void v337(); virtual void v338(); virtual void v339(); virtual void v340(); virtual void v341(); virtual void v342();
    virtual void v343(); virtual void v344(); virtual void v345(); virtual void v346(); virtual void v347(); virtual void v348(); virtual void v349(); virtual void v350(); virtual void v351(); virtual void v352();
    virtual void v353(); virtual void v354(); virtual void v355(); virtual void v356(); virtual void v357(); virtual void v358(); virtual void v359(); virtual void v360(); virtual void v361(); virtual void v362();
    virtual void v363();
    virtual long AddNewConversation(CScriptThing const &, bool, bool) const;   // slot 364 (+0x5b0) 0x008906C0
    virtual void AddPersonToConversation(long, CScriptThing const &) const;   // slot 365 (+0x5b4) 0x00890710
    virtual void AddLineToConversation(long, CCharString const &, bool, CScriptThing const &, CScriptThing const &) const;   // slot 366 (+0x5b8) 0x00890750
    virtual void RemoveConversation(long, bool) const;   // slot 367 (+0x5bc) 0x008907C0
    virtual bool IsConversationActive(long) const;   // slot 368 (+0x5c0) 0x008907D0
    virtual void v369(); virtual void v370(); virtual void v371(); virtual void v372(); virtual void v373(); virtual void v374(); virtual void v375();
    virtual void Pause(float) const;   // slot 376 (+0x5e0) 0x0088E4F0
};
class CScriptGameResourceObjectScriptedThingBase { public:        // 0x10 bytes, vtable 0x0127094C
    virtual void h00(); virtual void h01(); virtual void h02(); virtual void h03(); virtual void h04(); virtual void h05();
    virtual void h06(); virtual void h07(); virtual void h08(); virtual void h09(); virtual void h10(); virtual void h11();
    virtual void h12(); virtual void h13(); virtual void h14(); virtual void h15(); virtual void h16(); virtual void h17();
    virtual void PlayAnimation(const CCharString& animation, bool a, bool b, bool c, bool d, bool e, bool f, bool g);   // slot 18 (+0x48), descriptive name
    unsigned char _body[0xc];
};
class CScriptDef { public:                                         // retail layout (STLport 12-byte vectors; only _M_start used)
    unsigned char _pad0[0x92c];
    CCharString* TH_ThunderConv_Speaker;   unsigned char _pad930[8];   // +0x92c
    CCharString* TH_ThunderConv_Dialogue;  unsigned char _pad93c[8];   // +0x938
    CCharString* TH_ThunderConv_Actor;     unsigned char _pad948[8];   // +0x944
    CCharString* TH_ThunderConv_Animation; unsigned char _pad954[8];   // +0x950
    float*       TH_ThunderConv_AnimDelay;                             // +0x95c
};
extern CScriptDef* pGlobalScriptDef;                            // 0x0143E90C (Ego_r.pdb `pGlobalScriptDef`, a def pointer: its first word)
class CScriptBase { public:                                        // retail 0x40 bytes
    virtual void v00();
    unsigned char _body[0x3c];
    bool IsActiveThreadTerminating();                       // 0x00CB7940
};
struct CQuestScriptBase : CScriptBase {
    CGameScriptInterface* Game;                             // +0x40
    void*                 Data;                             // +0x44
};
namespace NScript {
class CV_TravellingHeroesScript : public CQuestScriptBase {
public:
    CScriptGameResourceObjectScriptedThingBase seh_Rival;   // +0x48
    CScriptGameResourceObjectScriptedThingBase seh_Girl;    // +0x58
    CScriptGameResourceObjectScriptedThingBase seh_Boy;     // +0x68
    bool ThunderKidsTalking;                            // +0x78
    bool ThunderSceneStarted;                           // +0x79
    void DoConversation(long line);
};
} // namespace NScript
using NScript::CV_TravellingHeroesScript;

void NScript::CV_TravellingHeroesScript::DoConversation(long line)
{
    CCharString speaker = pGlobalScriptDef->TH_ThunderConv_Speaker[line];
    CCharString dialogue = pGlobalScriptDef->TH_ThunderConv_Dialogue[line];
    CCharString actor = pGlobalScriptDef->TH_ThunderConv_Actor[line];
    CCharString animation = pGlobalScriptDef->TH_ThunderConv_Animation[line];
    float delay = pGlobalScriptDef->TH_ThunderConv_AnimDelay[line];
    CScriptThing speakerThing = Game->GetThingWithScriptName(speaker);
    CScriptThing unused;
    long conversation = Game->AddNewConversation(speakerThing, false, false);
    Game->AddPersonToConversation(conversation, Game->GetHero());
    Game->AddLineToConversation(conversation, dialogue, false, speakerThing, Game->GetHero());
    if (actor != "NULL") {
        if (IsActiveThreadTerminating())
            return;
        CScriptGameResourceObjectScriptedThingBase* actorHandle = 0;
        if (actor == "Thunder" || actor == "Whisper") {
            if (IsActiveThreadTerminating())
                return;
            actorHandle = &seh_Rival;
        } else if (actor == "Boy") {
            if (IsActiveThreadTerminating())
                return;
            actorHandle = &seh_Boy;
        } else if (actor == "Girl") {
            if (IsActiveThreadTerminating())
                return;
            actorHandle = &seh_Girl;
        }
        if (actorHandle) {
            if (IsActiveThreadTerminating())
                return;
            Game->Pause(delay);
            if (!ThunderSceneStarted) {
                if (IsActiveThreadTerminating())
                    return;
                actorHandle->PlayAnimation(animation, false, true, false, false, false, false, true);
            }
        }
    }
    while (Game->IsConversationActive(conversation) && !ThunderSceneStarted && !ThunderKidsTalking) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->RemoveConversation(conversation, false);
}