// CQ_OakValeBanditRaidScript::PopulateLevel() -- on a restart top the bandits up at the free restart points, otherwise clear the old bandits and spawn one at every bandit spawn point; attach each new bandit to the village, then refresh the hero's follower list
// Leaf: PopulateLevel
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DC0300, 1328 bytes.
// Callees: CCharString 0x0099EBF0 / ~ 0x0099EAE0 / default ctor 0x0099E4B0 / Format 0x0099F1F0, free 0x00BFEA14
//   (inline vector destructors), operator delete 0x00BFE9BC / ~CBaseClass 0x0099A2E0 (inline ~CScriptThing),
//   vector<CScriptThing>::erase 0x008AC9D0, CleanupBandits 0x00DC0830; CScriptThing::GetPos (slot 6); through the
//   interface (+0x40): GetHero (70), GetThingWithScriptName (72), GetAllThingsWithScriptName (75), CreateCreature (91),
//   GetFollowingEntityList (231), EntityAttachToVillage (492).
// Layout (retail = Ego_r.pdb - 0x14, the vector 12 bytes so -0x18 after it): BanditsAwareOfPlayer +0x48,
//   HerosFollowers +0x4c, OpenGates +0x58, FirstSetup +0x59, BanditsStopChasing +0x70, NoPersistence +0x74.
// Shape: vector/CScriptThing model of the landed CQ_HangingTreeEvilScript::CleanUp 0x00D6B720.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator=(const char* text);           // 0x0099EFE0
};
struct CWideString {
    void* PStringData;
    CWideString(const unsigned short* text);            // 0x0099B6B0
    ~CWideString();                                     // 0x0099B510
};
extern "C" void __cdecl free(void* p);                  // 0x00BFEA14
void operator delete(void* p);                          // 0x00BFE9BC (MSVCR71)
struct CCPPointerInfo {                                 // CCountedPointer's shared block
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);          // destroys the pointee
    void* Data;
};
template <class T> struct CCountedPointer {
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) {}
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
struct CBaseClass {                                     // polymorphic root of the script handles
    CBaseClass();                                       // 0x0099A2D0
    virtual ~CBaseClass();                              // 0x0099A2E0
};
struct C3DVector { float X, Y, Z; };
struct CScriptThingImpl;
struct CScriptThing : CBaseClass {                      // 12 bytes, vtable 0x01238C8C
    virtual void t001(); virtual void t002(); virtual void t003(); virtual void t004(); virtual void t005();
    virtual const C3DVector& GetPos() const;            // slot 6 (+0x18) 0x004AA980
    CCountedPointer<CScriptThingImpl> PImp;             // +0x04 Data, +0x08 Info
    CScriptThing(const CScriptThing& other);            // 0x004ABE90
    ~CScriptThing() {}
};
namespace std {
template<class T> inline void _Destroy(T* first, T* last)
{
    for (; first != last; ++first)
        first->~T();
}
template<class T> class allocator { };
template<class T, class A = allocator<T> > class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    __forceinline ~vector()
    {
        _Destroy(_M_start, _M_finish);
        if (_M_start)
            free(_M_start);
    }
    T* erase(T* first, T* last);                        // 0x008AC9D0 for CScriptThing
    void clear() { erase(_M_start, _M_finish); }
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    T& operator[](unsigned int n) { return _M_start[n]; }
};
}
struct CGameScriptInterface {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009();
    virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049();
    virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069();
    virtual CScriptThing& GetHero();   // slot 70 (+0x118)
    virtual void v071();
    virtual CScriptThing GetThingWithScriptName(const CCharString& name) const;   // slot 72 (+0x120)
    virtual void v073(); virtual void v074();
    virtual long GetAllThingsWithScriptName(const CCharString& name, std::vector<CScriptThing>& things) const;   // slot 75 (+0x12c)
    virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079(); virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085();
    virtual void v086(); virtual void v087(); virtual void v088(); virtual void v089(); virtual void v090();
    virtual CScriptThing CreateCreature(const CCharString& def, const C3DVector& pos, const CCharString& scriptName, bool a) const;   // slot 91 (+0x16c)
    virtual void v092(); virtual void v093(); virtual void v094(); virtual void v095(); virtual void v096(); virtual void v097(); virtual void v098(); virtual void v099(); virtual void v100(); virtual void v101();
    virtual void v102(); virtual void v103(); virtual void v104(); virtual void v105(); virtual void v106(); virtual void v107(); virtual void v108(); virtual void v109(); virtual void v110(); virtual void v111();
    virtual void v112(); virtual void v113(); virtual void v114(); virtual void v115(); virtual void v116(); virtual void v117(); virtual void v118(); virtual void v119(); virtual void v120(); virtual void v121();
    virtual void v122(); virtual void v123(); virtual void v124(); virtual void v125(); virtual void v126(); virtual void v127(); virtual void v128(); virtual void v129(); virtual void v130(); virtual void v131();
    virtual void v132(); virtual void v133(); virtual void v134(); virtual void v135(); virtual void v136(); virtual void v137(); virtual void v138(); virtual void v139(); virtual void v140(); virtual void v141();
    virtual void v142(); virtual void v143(); virtual void v144(); virtual void v145(); virtual void v146(); virtual void v147(); virtual void v148(); virtual void v149(); virtual void v150(); virtual void v151();
    virtual void v152(); virtual void v153(); virtual void v154(); virtual void v155(); virtual void v156(); virtual void v157(); virtual void v158(); virtual void v159(); virtual void v160(); virtual void v161();
    virtual void v162(); virtual void v163(); virtual void v164(); virtual void v165(); virtual void v166(); virtual void v167(); virtual void v168(); virtual void v169(); virtual void v170(); virtual void v171();
    virtual void v172(); virtual void v173(); virtual void v174(); virtual void v175(); virtual void v176(); virtual void v177(); virtual void v178(); virtual void v179(); virtual void v180(); virtual void v181();
    virtual void v182(); virtual void v183(); virtual void v184(); virtual void v185(); virtual void v186(); virtual void v187(); virtual void v188(); virtual void v189(); virtual void v190(); virtual void v191();
    virtual void v192(); virtual void v193(); virtual void v194(); virtual void v195(); virtual void v196(); virtual void v197(); virtual void v198(); virtual void v199(); virtual void v200(); virtual void v201();
    virtual void v202(); virtual void v203(); virtual void v204(); virtual void v205(); virtual void v206(); virtual void v207(); virtual void v208(); virtual void v209(); virtual void v210(); virtual void v211();
    virtual void v212(); virtual void v213(); virtual void v214(); virtual void v215(); virtual void v216(); virtual void v217(); virtual void v218(); virtual void v219(); virtual void v220(); virtual void v221();
    virtual void v222(); virtual void v223(); virtual void v224(); virtual void v225(); virtual void v226(); virtual void v227(); virtual void v228(); virtual void v229(); virtual void v230();
    virtual bool GetFollowingEntityList(const CScriptThing& thing, std::vector<CScriptThing>& things) const;   // slot 231 (+0x39c)
    virtual void v232(); virtual void v233(); virtual void v234(); virtual void v235(); virtual void v236(); virtual void v237(); virtual void v238(); virtual void v239(); virtual void v240(); virtual void v241();
    virtual void v242(); virtual void v243(); virtual void v244(); virtual void v245(); virtual void v246(); virtual void v247(); virtual void v248(); virtual void v249(); virtual void v250(); virtual void v251();
    virtual void v252(); virtual void v253(); virtual void v254(); virtual void v255(); virtual void v256(); virtual void v257(); virtual void v258(); virtual void v259(); virtual void v260(); virtual void v261();
    virtual void v262(); virtual void v263(); virtual void v264(); virtual void v265(); virtual void v266(); virtual void v267(); virtual void v268(); virtual void v269(); virtual void v270(); virtual void v271();
    virtual void v272(); virtual void v273(); virtual void v274(); virtual void v275(); virtual void v276(); virtual void v277(); virtual void v278(); virtual void v279(); virtual void v280(); virtual void v281();
    virtual void v282(); virtual void v283(); virtual void v284(); virtual void v285(); virtual void v286(); virtual void v287(); virtual void v288(); virtual void v289(); virtual void v290(); virtual void v291();
    virtual void v292(); virtual void v293(); virtual void v294(); virtual void v295(); virtual void v296(); virtual void v297(); virtual void v298(); virtual void v299(); virtual void v300(); virtual void v301();
    virtual void v302(); virtual void v303(); virtual void v304(); virtual void v305(); virtual void v306(); virtual void v307(); virtual void v308(); virtual void v309(); virtual void v310(); virtual void v311();
    virtual void v312(); virtual void v313(); virtual void v314(); virtual void v315(); virtual void v316(); virtual void v317(); virtual void v318(); virtual void v319(); virtual void v320(); virtual void v321();
    virtual void v322(); virtual void v323(); virtual void v324(); virtual void v325(); virtual void v326(); virtual void v327(); virtual void v328(); virtual void v329(); virtual void v330(); virtual void v331();
    virtual void v332(); virtual void v333(); virtual void v334(); virtual void v335(); virtual void v336(); virtual void v337(); virtual void v338(); virtual void v339(); virtual void v340(); virtual void v341();
    virtual void v342(); virtual void v343(); virtual void v344(); virtual void v345(); virtual void v346(); virtual void v347(); virtual void v348(); virtual void v349(); virtual void v350(); virtual void v351();
    virtual void v352(); virtual void v353(); virtual void v354(); virtual void v355(); virtual void v356(); virtual void v357(); virtual void v358(); virtual void v359(); virtual void v360(); virtual void v361();
    virtual void v362(); virtual void v363(); virtual void v364(); virtual void v365(); virtual void v366(); virtual void v367(); virtual void v368(); virtual void v369(); virtual void v370(); virtual void v371();
    virtual void v372(); virtual void v373(); virtual void v374(); virtual void v375(); virtual void v376(); virtual void v377(); virtual void v378(); virtual void v379(); virtual void v380(); virtual void v381();
    virtual void v382(); virtual void v383(); virtual void v384(); virtual void v385(); virtual void v386(); virtual void v387(); virtual void v388(); virtual void v389(); virtual void v390(); virtual void v391();
    virtual void v392(); virtual void v393(); virtual void v394(); virtual void v395(); virtual void v396(); virtual void v397(); virtual void v398(); virtual void v399(); virtual void v400(); virtual void v401();
    virtual void v402(); virtual void v403(); virtual void v404(); virtual void v405(); virtual void v406(); virtual void v407(); virtual void v408(); virtual void v409(); virtual void v410(); virtual void v411();
    virtual void v412(); virtual void v413(); virtual void v414(); virtual void v415(); virtual void v416(); virtual void v417(); virtual void v418(); virtual void v419(); virtual void v420(); virtual void v421();
    virtual void v422(); virtual void v423(); virtual void v424(); virtual void v425(); virtual void v426(); virtual void v427(); virtual void v428(); virtual void v429(); virtual void v430(); virtual void v431();
    virtual void v432(); virtual void v433(); virtual void v434(); virtual void v435(); virtual void v436(); virtual void v437(); virtual void v438(); virtual void v439(); virtual void v440(); virtual void v441();
    virtual void v442(); virtual void v443(); virtual void v444(); virtual void v445(); virtual void v446(); virtual void v447(); virtual void v448(); virtual void v449(); virtual void v450(); virtual void v451();
    virtual void v452(); virtual void v453(); virtual void v454(); virtual void v455(); virtual void v456(); virtual void v457(); virtual void v458(); virtual void v459(); virtual void v460(); virtual void v461();
    virtual void v462(); virtual void v463(); virtual void v464(); virtual void v465(); virtual void v466(); virtual void v467(); virtual void v468(); virtual void v469(); virtual void v470(); virtual void v471();
    virtual void v472(); virtual void v473(); virtual void v474(); virtual void v475(); virtual void v476(); virtual void v477(); virtual void v478(); virtual void v479(); virtual void v480(); virtual void v481();
    virtual void v482(); virtual void v483(); virtual void v484(); virtual void v485(); virtual void v486(); virtual void v487(); virtual void v488(); virtual void v489(); virtual void v490(); virtual void v491();
    virtual void EntityAttachToVillage(const CScriptThing& thing, const CScriptThing& village) const;   // slot 492 (+0x7b0)
};
struct CCharStringFmt : CCharString {
    CCharStringFmt();                                   // 0x0099E4B0
    void __cdecl Format(const char* fmt, ...);          // 0x0099F1F0
};
struct CScriptBase {                                     // retail 0x40 bytes
    unsigned char _body[0x40];
};
namespace NScript {
class CQ_OakValeBanditRaidScript : public CScriptBase {
public:
    CGameScriptInterface*       Game;                   // +0x40
    void*                       PMasterData;            // +0x44
    bool                        BanditsAwareOfPlayer;   // +0x48
    std::vector<CScriptThing>   HerosFollowers;         // +0x4c
    bool                        OpenGates;              // +0x58
    bool                        FirstSetup;             // +0x59
    unsigned char               _pad5a[0x70 - 0x5a];
    long                        BanditsStopChasing;     // +0x70
    bool                        NoPersistence;          // +0x74
    bool PopulateLevel();
    void CleanupBandits();                              // 0x00DC0830
};
} // namespace NScript
using NScript::CQ_OakValeBanditRaidScript;

bool NScript::CQ_OakValeBanditRaidScript::PopulateLevel()
{
    bool spawned = false;
    BanditsAwareOfPlayer = false;
    if (!FirstSetup && OpenGates) {
        std::vector<CScriptThing> restarts;
        Game->GetAllThingsWithScriptName("OakValeRestart", restarts);
        CScriptThing village = Game->GetThingWithScriptName("OVBR_Village");
        std::vector<CScriptThing> bandits;
        Game->GetAllThingsWithScriptName("OakValeBanditRaidBandit", bandits);
        if (!NoPersistence)
            BanditsStopChasing = bandits.size();
        NoPersistence = false;
        for (unsigned int i = 0; i < restarts.size() - bandits.size(); ++i) {
            spawned = true;
            CScriptThing bandit = Game->CreateCreature("CREATURE_BANDIT_GRUNT", restarts[i].GetPos(), "OakValeBanditRaidBandit", false);
            Game->EntityAttachToVillage(bandit, village);
        }
    } else {
        CleanupBandits();
        std::vector<CScriptThing> spawns;
        Game->GetAllThingsWithScriptName("M_BanditSpawn", spawns);
        CScriptThing village = Game->GetThingWithScriptName("OVBR_Village");
        FirstSetup = false;
        for (unsigned int i = 0; i < spawns.size(); ++i) {
            CScriptThing bandit = Game->CreateCreature("CREATURE_BANDIT_GRUNT", spawns[i].GetPos(), "OakValeBanditRaidBandit", false);
            Game->EntityAttachToVillage(bandit, village);
        }
    }
    HerosFollowers.clear();
    Game->GetFollowingEntityList(Game->GetHero(), HerosFollowers);
    CCharStringFmt msg;
    msg.Format("Found %d followers", HerosFollowers.size());
    return spawned;
}