// CQ_AmbushTradersScript::Intro() -- the Gibbet Hill ambush intro: take the four bandit allies and the hero under scripted control, cast them as BAN1..BAN4 and HERO and play CS_GIBBETHILL_AMBUSH_INTRO (with the quest's cut-scene flag map) in a paused movie sequence with the camera fixed, release all five and set BanditIntroDone
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F0BB10, length 1579 bytes.
// Callees: CBaseObject ctor/dtor/operator= 0x0099A380 / 0x0099A430 / 0x0099A3B0, CCharString(const char*, long) 0x0099EBF0 /
//   ~CCharString 0x0099EAE0, CRT malloc 0x00BFEA0E / free 0x00BFEA14 (cast-map header), operator delete 0x00BFE9BC, cast map
//   operator[] 0x00CD3D2E and _M_erase 0x00CD344A, scripted-thing handle IsScripting 0x00CD23B9 / Release 0x00CD2770, movie
//   PImp dtor 0x006E7AB0, cut-scene runner 0x00CBFB7D (__fastcall); the game script interface (+0x40) slots 8
//   StartScriptingEntity, 70 GetHero, 370 StartMovieSequence, 371 FixMovieSequenceCamera, 379 PauseAllNonScriptedEntities.
// Strings: "HERO" 0x01255174, "BAN1" 0x012D1F04, "BAN2" 0x012D1EFC, "BAN3" 0x012D1EF4, "BAN4" 0x012D241C,
//   "CS_GIBBETHILL_AMBUSH_INTRO" 0x012F828C, "" 0x0122D70E.
// Layout (retail = Ego_r.pdb - 0x14, -4 per preceding 12-byte STLport vector): BanditIntroDone +0x4d, AllBandits +0xa8,
//   CSFlagMap +0x1b8.
// Shape: cast/movie model of the landed CQ_HeroSoulsBriarScript::PlayCutscene 0x00D823A0 (local cast map with malloc'd
//   header and inline destructor, implicit-literal movie name, one named copy of Game for the pause pair); here every
//   handle destructor is inline.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (struct->class; already NScript). Same bytes.
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E
extern "C" void __cdecl free(void* block);              // 0x00BFEA14
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
void operator delete(void* p);                          // 0x00BFE9BC (MSVCR71)
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_NONE };

struct CCPPointerInfo {
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};

template <class T> struct CCountedPointer {
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) {}
    void Release()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
    }
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        CCPPointerInfo* info = other.Info;
        T* data = other.Data;
        if (Info != info) {
            Release();
            Data = data;
            Info = info;
            if (info)
                ++info->RefCount;
        }
        return *this;
    }
    ~CCountedPointer()
    {
        Release();
        Data = 0;
        Info = 0;
    }
};

struct CScriptThingImp;
struct CBaseClass {                                     // vtable 0x01231710
    CBaseClass();                                       // 0x0099A2D0
    virtual ~CBaseClass();                              // 0x0099A2E0
};
struct CScriptThing : CBaseClass {                      // 12 bytes, vtable 0x01238C8C
    CCountedPointer<CScriptThingImp> PImp;              // +0x04
    virtual ~CScriptThing() {}                          // out of line: 0x004AA840
};
template <class T> struct vector {                      // STLport vector (retail 12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T& operator[](unsigned int n) { return *(_M_start + n); }
};

struct CBaseObject {                                    // vtable 0x0129A7D8
    CBaseObject();                                      // 0x0099A380
    virtual ~CBaseObject();                             // 0x0099A430
    CBaseObject& operator=(const CBaseObject& other);   // 0x0099A3B0
    void* IntelligentPointer;                           // +0x04
};
struct CScriptGameResourceObjectBase : CBaseObject {    // vtable 0x0126008C
    virtual ~CScriptGameResourceObjectBase() {}
};
struct CScriptGameResourceObjectScriptedThingBase : CScriptGameResourceObjectBase {   // vtable 0x0127094C
    CCountedPointer<CScriptThingImp> PImp;
    // implicit destructor; out of line: 0x007E74D0
    __forceinline CScriptGameResourceObjectScriptedThingBase& operator=(const CScriptGameResourceObjectScriptedThingBase& other)
    {                                                   // out of line: 0x008ABD10
        CBaseObject::operator=(other);
        PImp = other.PImp;
        return *this;
    }
    bool IsScripting();                                 // 0x00CD23B9
    void Release();                                     // 0x00CD2770
};
struct CMovieImpPointer {
    void* Data;
    void* Info;
    CMovieImpPointer() : Data(0), Info(0) {}
    ~CMovieImpPointer();                                // 0x006E7AB0
};
struct CScriptGameResourceObjectMovieBase : CScriptGameResourceObjectBase {   // vtable 0x01260EF4
    CMovieImpPointer PImp;
};

struct CCutsceneCastMap {                               // map<CCharString, CScriptGameResourceObjectScriptedThingBase> (retail STLport, 12 bytes)
    struct node { unsigned char colour; unsigned char _pad[3]; node* parent; node* left; node* right; unsigned char value[0x24 - 16]; };
    node*         head;
    unsigned long size;
    unsigned char compare;
    __forceinline CCutsceneCastMap()
        : head(0)
    {
        head = (node*)malloc(sizeof(node));
        size = 0;
        head->colour = 0;
        head->parent = 0;
        head->left   = head;
        head->right  = head;
    }
    __forceinline ~CCutsceneCastMap()                   // inline here (out of line elsewhere: 0x00CDBFB0)
    {
        if (size) {
            Erase(head->parent);
            head->left = head;
            head->parent = 0;
            head->right = head;
            size = 0;
        }
        if (head)
            free(head);
    }
    void Erase(node* root);                             // 0x00CD344A
    CScriptGameResourceObjectScriptedThingBase& operator[](const CCharString& key);   // 0x00CD3D2E
};
struct CCutsceneFlagMap {                               // map<CCharString, bool> (retail STLport, 12 bytes)
    void* _body[3];
};
struct CCutsceneArgMap;
void __fastcall RunCutsceneMacro(const CCharString& name, CCutsceneCastMap& cast, CCutsceneFlagMap* flags, CCutsceneArgMap* args, bool a5, bool a6);   // 0x00CBFB7D

struct CGameScriptInterface {                       // the quest's game script interface (slots: ghidra_out/cgsi_slots_aligned.tsv)
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual bool StartScriptingEntity(const CScriptThing& thing, CScriptGameResourceObjectScriptedThingBase& object, EScriptAIPriority priority);   // slot 8 (+0x20) StartScriptingEntity
    virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016();
    virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024();
    virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031(); virtual void v032();
    virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040();
    virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048();
    virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056();
    virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064();
    virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069();
    virtual CScriptThing& GetHero() const;   // slot 70 (+0x118) GetHero
    virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078();
    virtual void v079(); virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086();
    virtual void v087(); virtual void v088(); virtual void v089(); virtual void v090(); virtual void v091(); virtual void v092(); virtual void v093(); virtual void v094();
    virtual void v095(); virtual void v096(); virtual void v097(); virtual void v098(); virtual void v099(); virtual void v100(); virtual void v101(); virtual void v102();
    virtual void v103(); virtual void v104(); virtual void v105(); virtual void v106(); virtual void v107(); virtual void v108(); virtual void v109(); virtual void v110();
    virtual void v111(); virtual void v112(); virtual void v113(); virtual void v114(); virtual void v115(); virtual void v116(); virtual void v117(); virtual void v118();
    virtual void v119(); virtual void v120(); virtual void v121(); virtual void v122(); virtual void v123(); virtual void v124(); virtual void v125(); virtual void v126();
    virtual void v127(); virtual void v128(); virtual void v129(); virtual void v130(); virtual void v131(); virtual void v132(); virtual void v133(); virtual void v134();
    virtual void v135(); virtual void v136(); virtual void v137(); virtual void v138(); virtual void v139(); virtual void v140(); virtual void v141(); virtual void v142();
    virtual void v143(); virtual void v144(); virtual void v145(); virtual void v146(); virtual void v147(); virtual void v148(); virtual void v149(); virtual void v150();
    virtual void v151(); virtual void v152(); virtual void v153(); virtual void v154(); virtual void v155(); virtual void v156(); virtual void v157(); virtual void v158();
    virtual void v159(); virtual void v160(); virtual void v161(); virtual void v162(); virtual void v163(); virtual void v164(); virtual void v165(); virtual void v166();
    virtual void v167(); virtual void v168(); virtual void v169(); virtual void v170(); virtual void v171(); virtual void v172(); virtual void v173(); virtual void v174();
    virtual void v175(); virtual void v176(); virtual void v177(); virtual void v178(); virtual void v179(); virtual void v180(); virtual void v181(); virtual void v182();
    virtual void v183(); virtual void v184(); virtual void v185(); virtual void v186(); virtual void v187(); virtual void v188(); virtual void v189(); virtual void v190();
    virtual void v191(); virtual void v192(); virtual void v193(); virtual void v194(); virtual void v195(); virtual void v196(); virtual void v197(); virtual void v198();
    virtual void v199(); virtual void v200(); virtual void v201(); virtual void v202(); virtual void v203(); virtual void v204(); virtual void v205(); virtual void v206();
    virtual void v207(); virtual void v208(); virtual void v209(); virtual void v210(); virtual void v211(); virtual void v212(); virtual void v213(); virtual void v214();
    virtual void v215(); virtual void v216(); virtual void v217(); virtual void v218(); virtual void v219(); virtual void v220(); virtual void v221(); virtual void v222();
    virtual void v223(); virtual void v224(); virtual void v225(); virtual void v226(); virtual void v227(); virtual void v228(); virtual void v229(); virtual void v230();
    virtual void v231(); virtual void v232(); virtual void v233(); virtual void v234(); virtual void v235(); virtual void v236(); virtual void v237(); virtual void v238();
    virtual void v239(); virtual void v240(); virtual void v241(); virtual void v242(); virtual void v243(); virtual void v244(); virtual void v245(); virtual void v246();
    virtual void v247(); virtual void v248(); virtual void v249(); virtual void v250(); virtual void v251(); virtual void v252(); virtual void v253(); virtual void v254();
    virtual void v255(); virtual void v256(); virtual void v257(); virtual void v258(); virtual void v259(); virtual void v260(); virtual void v261(); virtual void v262();
    virtual void v263(); virtual void v264(); virtual void v265(); virtual void v266(); virtual void v267(); virtual void v268(); virtual void v269(); virtual void v270();
    virtual void v271(); virtual void v272(); virtual void v273(); virtual void v274(); virtual void v275(); virtual void v276(); virtual void v277(); virtual void v278();
    virtual void v279(); virtual void v280(); virtual void v281(); virtual void v282(); virtual void v283(); virtual void v284(); virtual void v285(); virtual void v286();
    virtual void v287(); virtual void v288(); virtual void v289(); virtual void v290(); virtual void v291(); virtual void v292(); virtual void v293(); virtual void v294();
    virtual void v295(); virtual void v296(); virtual void v297(); virtual void v298(); virtual void v299(); virtual void v300(); virtual void v301(); virtual void v302();
    virtual void v303(); virtual void v304(); virtual void v305(); virtual void v306(); virtual void v307(); virtual void v308(); virtual void v309(); virtual void v310();
    virtual void v311(); virtual void v312(); virtual void v313(); virtual void v314(); virtual void v315(); virtual void v316(); virtual void v317(); virtual void v318();
    virtual void v319(); virtual void v320(); virtual void v321(); virtual void v322(); virtual void v323(); virtual void v324(); virtual void v325(); virtual void v326();
    virtual void v327(); virtual void v328(); virtual void v329(); virtual void v330(); virtual void v331(); virtual void v332(); virtual void v333(); virtual void v334();
    virtual void v335(); virtual void v336(); virtual void v337(); virtual void v338(); virtual void v339(); virtual void v340(); virtual void v341(); virtual void v342();
    virtual void v343(); virtual void v344(); virtual void v345(); virtual void v346(); virtual void v347(); virtual void v348(); virtual void v349(); virtual void v350();
    virtual void v351(); virtual void v352(); virtual void v353(); virtual void v354(); virtual void v355(); virtual void v356(); virtual void v357(); virtual void v358();
    virtual void v359(); virtual void v360(); virtual void v361(); virtual void v362(); virtual void v363(); virtual void v364(); virtual void v365(); virtual void v366();
    virtual void v367(); virtual void v368(); virtual void v369();
    virtual void StartMovieSequence(const CCharString& name, CScriptGameResourceObjectMovieBase& movie);   // slot 370 (+0x5c8) StartMovieSequence
    virtual void FixMovieSequenceCamera(bool fix) const;   // slot 371 (+0x5cc) FixMovieSequenceCamera
    virtual void v372(); virtual void v373(); virtual void v374(); virtual void v375(); virtual void v376(); virtual void v377(); virtual void v378();
    virtual void PauseAllNonScriptedEntities(bool pause);   // slot 379 (+0x5ec) PauseAllNonScriptedEntities
};

namespace NScript {
struct CScriptBase {                                    // retail 0x40 bytes
    unsigned char _body[0x40];
};
class CQ_AmbushTradersScript : public CScriptBase {
public:
    CGameScriptInterface* Game;                         // +0x40
    void*                 PMasterData;                  // +0x44
    unsigned char         _pad48[0x4d - 0x48];
    bool                  BanditIntroDone;              // +0x4d
    unsigned char         _pad4e[0xa8 - 0x4e];
    vector<CScriptThing>  AllBandits;                   // +0xa8
    unsigned char         _padb4[0x1b8 - 0xb4];
    CCutsceneFlagMap      CSFlagMap;                    // +0x1b8
    void Intro();
};

void CQ_AmbushTradersScript::Intro()
{
    CScriptGameResourceObjectScriptedThingBase bandit1;
    CScriptGameResourceObjectScriptedThingBase bandit2;
    CScriptGameResourceObjectScriptedThingBase bandit3;
    CScriptGameResourceObjectScriptedThingBase bandit4;
    Game->StartScriptingEntity(AllBandits[0], bandit1, (EScriptAIPriority)4);
    Game->StartScriptingEntity(AllBandits[1], bandit2, (EScriptAIPriority)4);
    Game->StartScriptingEntity(AllBandits[2], bandit3, (EScriptAIPriority)4);
    Game->StartScriptingEntity(AllBandits[3], bandit4, (EScriptAIPriority)4);
    CScriptGameResourceObjectScriptedThingBase hero;
    Game->StartScriptingEntity(Game->GetHero(), hero, (EScriptAIPriority)4);
    {
        CCutsceneCastMap cast;
        cast["HERO"] = hero;
        cast["BAN1"] = bandit1;
        cast["BAN2"] = bandit2;
        cast["BAN3"] = bandit3;
        cast["BAN4"] = bandit4;
        CScriptGameResourceObjectMovieBase movie;
        Game->StartMovieSequence("", movie);
        CGameScriptInterface* game = Game;
        game->PauseAllNonScriptedEntities(true);
        Game->FixMovieSequenceCamera(true);
        RunCutsceneMacro("CS_GIBBETHILL_AMBUSH_INTRO", cast, &CSFlagMap, 0, false, true);
        Game->FixMovieSequenceCamera(false);
        game->PauseAllNonScriptedEntities(false);
    }
    if (bandit1.IsScripting())
        bandit1.Release();
    if (bandit2.IsScripting())
        bandit2.Release();
    if (bandit3.IsScripting())
        bandit3.Release();
    if (bandit4.IsScripting())
        bandit4.Release();
    if (hero.IsScripting())
        hero.Release();
    BanditIntroDone = true;
}
}