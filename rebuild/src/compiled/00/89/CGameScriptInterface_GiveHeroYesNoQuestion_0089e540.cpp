// CGameScriptInterface::GiveHeroYesNoQuestion(const CCharString& question, const CCharString& yesText, const CCharString& noText, const CCharString& extraText, bool pauseGame) const -- close the hero's boast UI, optionally pause, show the yes/no question as a subtitle (the four-answer form only when the extra text is non-empty), then let the boast UI open again
// Leaf: GiveHeroYesNoQuestion
// Retail: Fable.exe (TLC, VC7.1) @ 0x0089E540, 458 bytes, `ret 0x14`.
// Callees: CPlayerManager::GetMainPlayer 0x00449970, CPlayer::GetPControlledCreature 0x00487DC0, CBaseIntelligentPointer ctor 0x00A01B10 /
//   GetPItem 0x00A01B50 / SetPItem 0x00A01B90 / dtor 0x00A01C10, the component-map LowerBound 0x0040F020 (TC 0xe8 boast UI),
//   CTCBoastUI::AllowUIToOpen 0x0056A930, own slot 376 Pause, CCharString(const char*, long) 0x0099EBF0 / dtor 0x0099EAE0,
//   CBasicString<char>::operator==(const char*) 0x004115A0, CSubtitleRenderer::SetText 0x00493049 / 0x00492FB4 (thunks).
// Layout: the player GUI 0x013B8790 holds the subtitle renderer at +0x18; the main game component (+8) its font at +0x1614c.
//   The generated CGameScriptInterface header flattens the vptr, so `this` is a local projection.
class CThingPlayerCreature;
#include <string.h>
#pragma pack(push, 1)
template <class T> class CBasicString { public:
    char*         PData;
    unsigned long StringLength;
    unsigned long DataLength : 31;
    bool          UseFastExtend : 1;
    bool operator==(const char* text) const;            // 0x004115A0
};
class CCharStringData { public: CBasicString<char> Data; long NoRefs; };
#pragma pack(pop)
class CCharString { public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
    bool operator==(const char* text) const
    {
        if (PStringData == 0) {
            if (strcmp(text, "") == 0)
                return true;
            return false;
        }
        return PStringData->Data == text;
    }
};
class CFontBank;
class CSubtitleRenderer { public:
    void SetText(const CFontBank& font, const CCharString& a, const CCharString& b, const CCharString& c, const CCharString& d,
                 const CCharString& e, unsigned long colour, float time, bool f, bool g);   // 0x00493049
    void SetText(const CFontBank& font, const CCharString& a, const CCharString& b, const CCharString& c, const CCharString& d,
                 unsigned long colour, float time, bool f, bool g);                         // 0x00492FB4
};
class CPlayerGui { public: unsigned char Data[0x18]; CSubtitleRenderer Subtitles; };
extern CPlayerGui* g_pPlayerGui;                         // 0x013B8790
class CMainGameComponent { public: unsigned char Data[0x1614c]; CFontBank* PFont; };
class CTCBoastUI { public: void AllowUIToOpen(bool allow); };   // 0x0056A930
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
typedef std::pair<ETCInterfaceType, CTCBase*> ComponentEntry;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    ComponentEntry* Start;
    ComponentEntry* Finish;
    ComponentEntry* LowerBound(const ETCInterfaceType& key);         // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > ComponentMap;
class CThing { public:
    void*          VTable;
    unsigned char  Base[0x1c];
    unsigned long  UsingComponents[9];                   // +0x20
    ComponentMap   Components;                           // +0x44
    unsigned char  Metadata[0x45];
    unsigned char  LifeFlags;                            // +0x91
};
class CThingPlayerCreature : public CThing { };
class CBaseObject;
struct CBaseIntelligentPointer {
    void* Info;
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                            // 0x00A01B50
    void SetPItem(CThing* item);                         // 0x00A01B90
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:   // vtable 0x01231C4C
    CIntelligentPointer(T* item) { SetPItem((CThing*)item); }

};
class CPlayer { public: CThingPlayerCreature* GetPControlledCreature() const; };              // 0x00487DC0
class CPlayerManager { public: CPlayer& GetMainPlayer(); };     // 0x00449970
class CGameScriptInterface { public:
    virtual void v000();   // slot 0 (+0x0)
    virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008();
    virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016();
    virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024();
    virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031(); virtual void v032();
    virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040();
    virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048();
    virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056();
    virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064();
    virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071(); virtual void v072();
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
    virtual void v361(); virtual void v362(); virtual void v363(); virtual void v364(); virtual void v365(); virtual void v366(); virtual void v367(); virtual void v368();
    virtual void v369(); virtual void v370(); virtual void v371(); virtual void v372(); virtual void v373(); virtual void v374(); virtual void v375();
    virtual void Pause(float seconds) const;   // slot 376 (+0x5e0)
    void*               World;                           // +0x04
    CMainGameComponent* Component;                       // +0x08
    void*               DisplayEngine;                   // +0x0c
    void*               DefinitionManager;               // +0x10
    CPlayerManager*     PlayerManager;                   // +0x14
    void GiveHeroYesNoQuestion(const CCharString& question, const CCharString& yesText, const CCharString& noText, const CCharString& extraText, bool pauseGame) const;
};

void CGameScriptInterface::GiveHeroYesNoQuestion(const CCharString& question, const CCharString& yesText, const CCharString& noText, const CCharString& extraText, bool pauseGame) const
{
    CIntelligentPointer<CThingPlayerCreature> hero((CThingPlayerCreature*)PlayerManager->GetMainPlayer().GetPControlledCreature());
    CTCBoastUI* boastUI = 0;
    if (((CThing*)hero.GetPItem()) && !(((CThing*)hero.GetPItem())->LifeFlags & 1)) {
        CThing* thing = ((CThing*)hero.GetPItem());
        if (thing->UsingComponents[7] & 0x100) {
        ComponentMap* map = &thing->Components;
        ComponentEntry* it = map->LowerBound((ETCInterfaceType)0xe8);
        ComponentEntry* end = map->Finish;
        boastUI = (CTCBoastUI*)((void*)((it == end || it->first > 0xe8) ? end : it)->second);
        boastUI->AllowUIToOpen(false);
        }
    }
    if (pauseGame)
        Pause(1.0f);
    if (extraText == "")
        g_pPlayerGui->Subtitles.SetText(*Component->PFont, question, "", yesText, noText, 0x24, 0.0f, false, true);
    else
        g_pPlayerGui->Subtitles.SetText(*Component->PFont, question, "", yesText, noText, extraText, 0x24, 0.0f, false, true);
    if (((CThing*)hero.GetPItem()) && !(((CThing*)hero.GetPItem())->LifeFlags & 1) && boastUI)
        boastUI->AllowUIToOpen(true);
}