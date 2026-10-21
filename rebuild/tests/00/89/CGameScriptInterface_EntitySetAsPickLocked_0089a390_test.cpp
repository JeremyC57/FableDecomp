#include <cstdio>
class C3DVector {
public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};
struct CTCBase;
class CInterfaceMap {                                    // CThing::Components, CVectorMap<ETCInterfaceType, CTCBase*> (retail 8 bytes)
public:
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                // 0x0040F020
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long UsingComponents[9];                    // +0x20 bitset<274>
    CInterfaceMap Components;                            // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char Dead : 1;                              // +0x91 bit 0
    bool IsDead() const { return Dead; }
    bool HasTC(long type) const
    {
        if (UsingComponents[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    template <class T> __forceinline bool GetTC(T** out)
    {
        if (!HasTC(T::INTERFACE_TYPE))
            return false;
        long type = T::INTERFACE_TYPE;
        CInterfaceMap* map = &Components;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || T::INTERFACE_TYPE < it->type)
            it = map->last;
        *out = (T*)it->component;
        return true;
    }
};
class CThingPlayerCreature : public CThing { };
class CPlayer {
public:
    CThingPlayerCreature* GetPPlayerCharacter() const;   // 0x00487DC0
};
class CPlayerManager {
public:
    CPlayer& GetMainPlayer();                            // 0x00449970
};
namespace NEntityEvents {
class CEventBase {
public:
    virtual ~CEventBase();
    long Data04;
};
class CEventItemUsed : public CEventBase {               // vtable 0x0125C820, 0x10 bytes
public:
    void* ItemUser[2];                                   // +0x08 (thing pointer, set by 0x00A01B10 / 0x00A01B90)
    CEventItemUsed(CThing& user);                        // 0x006A0750
};
}
class CTCEntityEvents {
public:
    enum { INTERFACE_TYPE = 0x42 };
    void AddEvent(NEntityEvents::CEventBase* event);     // 0x008113C0
};
enum EMessageEventType { MESSAGE_EVENT_0x46 = 0x46 };
class CMessageEventManager {
public:
    void* AddMessage(EMessageEventType type, unsigned long duration, const C3DVector& pos, CThing* from, CThing* to,
                     float a, float b, CThing* extra);   // 0x00687540
};
class CWorld {
public:
    unsigned char         _pad00[0x60];
    CMessageEventManager* PMessageEventManager;          // +0x60 (CCountedPointer data)
};
class CScriptThing {
public:
    virtual void _s000() const; virtual void _s001() const; virtual void _s002() const; virtual void _s003() const; virtual void _s004() const; virtual void _s005() const;
    virtual void _s006() const; virtual void _s007() const; virtual void _s008() const; virtual void _s009() const; virtual void _s010() const;
    virtual CThing* GetThing() const;                    // slot 11 (+0x2c)
    virtual void _s012() const; virtual void _s013() const; virtual void _s014() const; virtual void _s015() const; virtual void _s016() const; virtual void _s017() const;
    virtual void _s018() const; virtual void _s019() const; virtual void _s020() const; virtual void _s021() const; virtual void _s022() const; virtual void _s023() const;
    virtual void _s024() const; virtual void _s025() const; virtual void _s026() const; virtual void _s027() const; virtual void _s028() const; virtual void _s029() const;
    virtual void _s030() const; virtual void _s031() const; virtual void _s032() const; virtual void _s033() const; virtual void _s034() const; virtual void _s035() const;
    virtual void _s036() const; virtual void _s037() const; virtual void _s038() const; virtual void _s039() const; virtual void _s040() const; virtual void _s041() const;
    virtual void _s042() const; virtual void _s043() const; virtual void _s044() const; virtual void _s045() const; virtual void _s046() const; virtual void _s047() const;
    virtual void _s048() const; virtual void _s049() const; virtual void _s050() const; virtual void _s051() const; virtual void _s052() const; virtual void _s053() const;
    virtual void _s054() const; virtual void _s055() const; virtual void _s056() const; virtual void _s057() const; virtual void _s058() const; virtual void _s059() const;
    virtual void _s060() const; virtual void _s061() const; virtual void _s062() const; virtual void _s063() const; virtual void _s064() const; virtual void _s065() const;
    virtual void _s066() const; virtual void _s067() const; virtual void _s068() const; virtual void _s069() const; virtual void _s070() const; virtual void _s071() const;
    virtual void _s072() const; virtual void _s073() const; virtual void _s074() const;
    virtual bool IsValid() const;                        // slot 75 (+0x12c)
};
class CGameScriptInterface {
public:
    virtual void _v000() const; virtual void _v001() const; virtual void _v002() const; virtual void _v003() const; virtual void _v004() const; virtual void _v005() const;
    virtual void _v006() const; virtual void _v007() const; virtual void _v008() const; virtual void _v009() const; virtual void _v010() const; virtual void _v011() const;
    virtual void _v012() const; virtual void _v013() const; virtual void _v014() const; virtual void _v015() const; virtual void _v016() const; virtual void _v017() const;
    virtual void _v018() const; virtual void _v019() const; virtual void _v020() const; virtual void _v021() const; virtual void _v022() const; virtual void _v023() const;
    virtual void _v024() const; virtual void _v025() const; virtual void _v026() const; virtual void _v027() const; virtual void _v028() const; virtual void _v029() const;
    virtual void _v030() const; virtual void _v031() const; virtual void _v032() const; virtual void _v033() const; virtual void _v034() const; virtual void _v035() const;
    virtual void _v036() const; virtual void _v037() const; virtual void _v038() const; virtual void _v039() const; virtual void _v040() const; virtual void _v041() const;
    virtual void _v042() const; virtual void _v043() const; virtual void _v044() const; virtual void _v045() const; virtual void _v046() const; virtual void _v047() const;
    virtual void _v048() const; virtual void _v049() const; virtual void _v050() const; virtual void _v051() const; virtual void _v052() const; virtual void _v053() const;
    virtual void _v054() const; virtual void _v055() const; virtual void _v056() const; virtual void _v057() const; virtual void _v058() const; virtual void _v059() const;
    virtual void _v060() const; virtual void _v061() const; virtual void _v062() const; virtual void _v063() const; virtual void _v064() const; virtual void _v065() const;
    virtual void _v066() const; virtual void _v067() const; virtual void _v068() const; virtual void _v069() const; virtual void _v070() const; virtual void _v071() const;
    virtual void _v072() const; virtual void _v073() const; virtual void _v074() const; virtual void _v075() const; virtual void _v076() const; virtual void _v077() const;
    virtual void _v078() const; virtual void _v079() const; virtual void _v080() const; virtual void _v081() const; virtual void _v082() const; virtual void _v083() const;
    virtual void _v084() const; virtual void _v085() const; virtual void _v086() const; virtual void _v087() const; virtual void _v088() const; virtual void _v089() const;
    virtual void _v090() const; virtual void _v091() const; virtual void _v092() const; virtual void _v093() const; virtual void _v094() const; virtual void _v095() const;
    virtual void _v096() const; virtual void _v097() const; virtual void _v098() const; virtual void _v099() const; virtual void _v100() const; virtual void _v101() const;
    virtual void _v102() const; virtual void _v103() const; virtual void _v104() const; virtual void _v105() const; virtual void _v106() const; virtual void _v107() const;
    virtual void _v108() const; virtual void _v109() const; virtual void _v110() const; virtual void _v111() const; virtual void _v112() const; virtual void _v113() const;
    virtual void _v114() const; virtual void _v115() const; virtual void _v116() const; virtual void _v117() const; virtual void _v118() const; virtual void _v119() const;
    virtual void _v120() const; virtual void _v121() const; virtual void _v122() const; virtual void _v123() const; virtual void _v124() const; virtual void _v125() const;
    virtual void _v126() const; virtual void _v127() const; virtual void _v128() const; virtual void _v129() const; virtual void _v130() const; virtual void _v131() const;
    virtual void _v132() const; virtual void _v133() const; virtual void _v134() const; virtual void _v135() const; virtual void _v136() const; virtual void _v137() const;
    virtual void _v138() const; virtual void _v139() const; virtual void _v140() const; virtual void _v141() const; virtual void _v142() const; virtual void _v143() const;
    virtual void _v144() const; virtual void _v145() const; virtual void _v146() const; virtual void _v147() const; virtual void _v148() const; virtual void _v149() const;
    virtual void _v150() const; virtual void _v151() const; virtual void _v152() const; virtual void _v153() const; virtual void _v154() const; virtual void _v155() const;
    virtual void _v156() const; virtual void _v157() const; virtual void _v158() const; virtual void _v159() const; virtual void _v160() const; virtual void _v161() const;
    virtual void _v162() const; virtual void _v163() const; virtual void _v164() const; virtual void _v165() const; virtual void _v166() const; virtual void _v167() const;
    virtual void _v168() const; virtual void _v169() const; virtual void _v170() const; virtual void _v171() const; virtual void _v172() const; virtual void _v173() const;
    virtual void _v174() const; virtual void _v175() const; virtual void _v176() const; virtual void _v177() const; virtual void _v178() const; virtual void _v179() const;
    virtual void _v180() const; virtual void _v181() const; virtual void _v182() const; virtual void _v183() const; virtual void _v184() const; virtual void _v185() const;
    virtual void _v186() const; virtual void _v187() const; virtual void _v188() const; virtual void _v189() const; virtual void _v190() const; virtual void _v191() const;
    virtual void _v192() const; virtual void _v193() const; virtual void _v194() const; virtual void _v195() const; virtual void _v196() const; virtual void _v197() const;
    virtual void _v198() const; virtual void _v199() const; virtual void _v200() const; virtual void _v201() const; virtual void _v202() const; virtual void _v203() const;
    virtual void _v204() const; virtual void _v205() const; virtual void _v206() const; virtual void _v207() const; virtual void _v208() const; virtual void _v209() const;
    virtual void _v210() const; virtual void _v211() const; virtual void _v212() const; virtual void _v213() const; virtual void _v214() const; virtual void _v215() const;
    virtual void _v216() const; virtual void _v217() const; virtual void _v218() const; virtual void _v219() const; virtual void _v220() const; virtual void _v221() const;
    virtual void _v222() const; virtual void _v223() const; virtual void _v224() const; virtual void _v225() const; virtual void _v226() const; virtual void _v227() const;
    virtual void _v228() const; virtual void _v229() const; virtual void _v230() const; virtual void _v231() const; virtual void _v232() const; virtual void _v233() const;
    virtual void _v234() const; virtual void _v235() const; virtual void _v236() const; virtual void _v237() const; virtual void _v238() const; virtual void _v239() const;
    virtual void _v240() const; virtual void _v241() const; virtual void _v242() const; virtual void _v243() const; virtual void _v244() const; virtual void _v245() const;
    virtual void _v246() const; virtual void _v247() const; virtual void _v248() const; virtual void _v249() const; virtual void _v250() const; virtual void _v251() const;
    virtual void _v252() const; virtual void _v253() const; virtual void _v254() const; virtual void _v255() const; virtual void _v256() const; virtual void _v257() const;
    virtual void _v258() const; virtual void _v259() const; virtual void _v260() const; virtual void _v261() const; virtual void _v262() const; virtual void _v263() const;
    virtual void _v264() const; virtual void _v265() const; virtual void _v266() const; virtual void _v267() const; virtual void _v268() const; virtual void _v269() const;
    virtual void _v270() const; virtual void _v271() const; virtual void _v272() const; virtual void _v273() const; virtual void _v274() const; virtual void _v275() const;
    virtual void _v276() const; virtual void _v277() const; virtual void _v278() const; virtual void _v279() const; virtual void _v280() const; virtual void _v281() const;
    virtual void _v282() const; virtual void _v283() const; virtual void _v284() const; virtual void _v285() const; virtual void _v286() const; virtual void _v287() const;
    virtual void _v288() const; virtual void _v289() const; virtual void _v290() const; virtual void _v291() const; virtual void _v292() const; virtual void _v293() const;
    virtual void _v294() const; virtual void _v295() const; virtual void _v296() const; virtual void _v297() const; virtual void _v298() const; virtual void _v299() const;
    virtual void _v300() const; virtual void _v301() const; virtual void _v302() const; virtual void _v303() const; virtual void _v304() const; virtual void _v305() const;
    virtual void _v306() const; virtual void _v307() const; virtual void _v308() const; virtual void _v309() const; virtual void _v310() const; virtual void _v311() const;
    virtual void _v312() const; virtual void _v313() const; virtual void _v314() const; virtual void _v315() const; virtual void _v316() const; virtual void _v317() const;
    virtual void _v318() const; virtual void _v319() const; virtual void _v320() const; virtual void _v321() const; virtual void _v322() const; virtual void _v323() const;
    virtual void _v324() const; virtual void _v325() const; virtual void _v326() const; virtual void _v327() const; virtual void _v328() const; virtual void _v329() const;
    virtual void _v330() const; virtual void _v331() const; virtual void _v332() const; virtual void _v333() const; virtual void _v334() const; virtual void _v335() const;
    virtual void _v336() const; virtual void _v337() const; virtual void _v338() const; virtual void _v339() const; virtual void _v340() const; virtual void _v341() const;
    virtual void _v342() const; virtual void _v343() const; virtual void _v344() const; virtual void _v345() const; virtual void _v346() const; virtual void _v347() const;
    virtual void _v348() const; virtual void _v349() const; virtual void _v350() const; virtual void _v351() const; virtual void _v352() const; virtual void _v353() const;
    virtual void _v354() const; virtual void _v355() const; virtual void _v356() const; virtual void _v357() const; virtual void _v358() const; virtual void _v359() const;
    virtual void _v360() const; virtual void _v361() const; virtual void _v362() const; virtual void _v363() const; virtual void _v364() const; virtual void _v365() const;
    virtual void _v366() const; virtual void _v367() const; virtual void _v368() const; virtual void _v369() const; virtual void _v370() const; virtual void _v371() const;
    virtual void _v372() const; virtual void _v373() const; virtual void _v374() const; virtual void _v375() const; virtual void _v376() const; virtual void _v377() const;
    virtual void _v378() const; virtual void _v379() const; virtual void _v380() const; virtual void _v381() const; virtual void _v382() const; virtual void _v383() const;
    virtual void _v384() const; virtual void _v385() const; virtual void _v386() const; virtual void _v387() const; virtual void _v388() const; virtual void _v389() const;
    virtual void _v390() const; virtual void _v391() const; virtual void _v392() const; virtual void _v393() const; virtual void _v394() const; virtual void _v395() const;
    virtual void _v396() const; virtual void _v397() const; virtual void _v398() const; virtual void _v399() const; virtual void _v400() const; virtual void _v401() const;
    virtual void _v402() const; virtual void _v403() const; virtual void _v404() const; virtual void _v405() const; virtual void _v406() const; virtual void _v407() const;
    virtual void _v408() const; virtual void _v409() const; virtual void _v410() const; virtual void _v411() const; virtual void _v412() const; virtual void _v413() const;
    virtual void _v414() const; virtual void _v415() const; virtual void _v416() const; virtual void _v417() const; virtual void _v418() const; virtual void _v419() const;
    virtual void _v420() const; virtual void _v421() const; virtual void _v422() const; virtual void _v423() const; virtual void _v424() const;
    virtual void OpenDoor(const CScriptThing& thing) const;   // slot 425 (+0x6a4)
    virtual void _v426() const; virtual void _v427() const; virtual void _v428() const;
    virtual void JamDoor(const CScriptThing& thing) const;    // slot 429 (+0x6b4)
    CWorld*         World;                               // +0x04
    void*           Component;                           // +0x08
    void*           DisplayEngine;                       // +0x0c
    void*           DefinitionManager;                   // +0x10
    CPlayerManager* PlayerManager;                       // +0x14
    void EntitySetAsPickLocked(const CScriptThing& thing) const;
};
void CGameScriptInterface::EntitySetAsPickLocked(const CScriptThing& thing) const
{
    if (!thing.IsValid())
        return;
    OpenDoor(thing);
    JamDoor(thing);
    CThing* door = thing.GetThing();
    World->PMessageEventManager->AddMessage(MESSAGE_EVENT_0x46, 50, C3DVector(0.0f, 0.0f, 0.0f), 0, 0, 0.0f, 0.0f, door);
    CThingPlayerCreature* hero = PlayerManager->GetMainPlayer().GetPPlayerCharacter();
    CTCEntityEvents* events;
    if (hero && !hero->IsDead() && door->GetTC(&events))
        events->AddEvent(new NEntityEvents::CEventItemUsed(*hero));
}

#include <stdio.h>
#include <new>
void CScriptThing::_s000() const {}
void CScriptThing::_s001() const {}
void CScriptThing::_s002() const {}
void CScriptThing::_s003() const {}
void CScriptThing::_s004() const {}
void CScriptThing::_s005() const {}
void CScriptThing::_s006() const {}
void CScriptThing::_s007() const {}
void CScriptThing::_s008() const {}
void CScriptThing::_s009() const {}
void CScriptThing::_s010() const {}
void CScriptThing::_s012() const {}
void CScriptThing::_s013() const {}
void CScriptThing::_s014() const {}
void CScriptThing::_s015() const {}
void CScriptThing::_s016() const {}
void CScriptThing::_s017() const {}
void CScriptThing::_s018() const {}
void CScriptThing::_s019() const {}
void CScriptThing::_s020() const {}
void CScriptThing::_s021() const {}
void CScriptThing::_s022() const {}
void CScriptThing::_s023() const {}
void CScriptThing::_s024() const {}
void CScriptThing::_s025() const {}
void CScriptThing::_s026() const {}
void CScriptThing::_s027() const {}
void CScriptThing::_s028() const {}
void CScriptThing::_s029() const {}
void CScriptThing::_s030() const {}
void CScriptThing::_s031() const {}
void CScriptThing::_s032() const {}
void CScriptThing::_s033() const {}
void CScriptThing::_s034() const {}
void CScriptThing::_s035() const {}
void CScriptThing::_s036() const {}
void CScriptThing::_s037() const {}
void CScriptThing::_s038() const {}
void CScriptThing::_s039() const {}
void CScriptThing::_s040() const {}
void CScriptThing::_s041() const {}
void CScriptThing::_s042() const {}
void CScriptThing::_s043() const {}
void CScriptThing::_s044() const {}
void CScriptThing::_s045() const {}
void CScriptThing::_s046() const {}
void CScriptThing::_s047() const {}
void CScriptThing::_s048() const {}
void CScriptThing::_s049() const {}
void CScriptThing::_s050() const {}
void CScriptThing::_s051() const {}
void CScriptThing::_s052() const {}
void CScriptThing::_s053() const {}
void CScriptThing::_s054() const {}
void CScriptThing::_s055() const {}
void CScriptThing::_s056() const {}
void CScriptThing::_s057() const {}
void CScriptThing::_s058() const {}
void CScriptThing::_s059() const {}
void CScriptThing::_s060() const {}
void CScriptThing::_s061() const {}
void CScriptThing::_s062() const {}
void CScriptThing::_s063() const {}
void CScriptThing::_s064() const {}
void CScriptThing::_s065() const {}
void CScriptThing::_s066() const {}
void CScriptThing::_s067() const {}
void CScriptThing::_s068() const {}
void CScriptThing::_s069() const {}
void CScriptThing::_s070() const {}
void CScriptThing::_s071() const {}
void CScriptThing::_s072() const {}
void CScriptThing::_s073() const {}
void CScriptThing::_s074() const {}
void CGameScriptInterface::_v000() const {}
void CGameScriptInterface::_v001() const {}
void CGameScriptInterface::_v002() const {}
void CGameScriptInterface::_v003() const {}
void CGameScriptInterface::_v004() const {}
void CGameScriptInterface::_v005() const {}
void CGameScriptInterface::_v006() const {}
void CGameScriptInterface::_v007() const {}
void CGameScriptInterface::_v008() const {}
void CGameScriptInterface::_v009() const {}
void CGameScriptInterface::_v010() const {}
void CGameScriptInterface::_v011() const {}
void CGameScriptInterface::_v012() const {}
void CGameScriptInterface::_v013() const {}
void CGameScriptInterface::_v014() const {}
void CGameScriptInterface::_v015() const {}
void CGameScriptInterface::_v016() const {}
void CGameScriptInterface::_v017() const {}
void CGameScriptInterface::_v018() const {}
void CGameScriptInterface::_v019() const {}
void CGameScriptInterface::_v020() const {}
void CGameScriptInterface::_v021() const {}
void CGameScriptInterface::_v022() const {}
void CGameScriptInterface::_v023() const {}
void CGameScriptInterface::_v024() const {}
void CGameScriptInterface::_v025() const {}
void CGameScriptInterface::_v026() const {}
void CGameScriptInterface::_v027() const {}
void CGameScriptInterface::_v028() const {}
void CGameScriptInterface::_v029() const {}
void CGameScriptInterface::_v030() const {}
void CGameScriptInterface::_v031() const {}
void CGameScriptInterface::_v032() const {}
void CGameScriptInterface::_v033() const {}
void CGameScriptInterface::_v034() const {}
void CGameScriptInterface::_v035() const {}
void CGameScriptInterface::_v036() const {}
void CGameScriptInterface::_v037() const {}
void CGameScriptInterface::_v038() const {}
void CGameScriptInterface::_v039() const {}
void CGameScriptInterface::_v040() const {}
void CGameScriptInterface::_v041() const {}
void CGameScriptInterface::_v042() const {}
void CGameScriptInterface::_v043() const {}
void CGameScriptInterface::_v044() const {}
void CGameScriptInterface::_v045() const {}
void CGameScriptInterface::_v046() const {}
void CGameScriptInterface::_v047() const {}
void CGameScriptInterface::_v048() const {}
void CGameScriptInterface::_v049() const {}
void CGameScriptInterface::_v050() const {}
void CGameScriptInterface::_v051() const {}
void CGameScriptInterface::_v052() const {}
void CGameScriptInterface::_v053() const {}
void CGameScriptInterface::_v054() const {}
void CGameScriptInterface::_v055() const {}
void CGameScriptInterface::_v056() const {}
void CGameScriptInterface::_v057() const {}
void CGameScriptInterface::_v058() const {}
void CGameScriptInterface::_v059() const {}
void CGameScriptInterface::_v060() const {}
void CGameScriptInterface::_v061() const {}
void CGameScriptInterface::_v062() const {}
void CGameScriptInterface::_v063() const {}
void CGameScriptInterface::_v064() const {}
void CGameScriptInterface::_v065() const {}
void CGameScriptInterface::_v066() const {}
void CGameScriptInterface::_v067() const {}
void CGameScriptInterface::_v068() const {}
void CGameScriptInterface::_v069() const {}
void CGameScriptInterface::_v070() const {}
void CGameScriptInterface::_v071() const {}
void CGameScriptInterface::_v072() const {}
void CGameScriptInterface::_v073() const {}
void CGameScriptInterface::_v074() const {}
void CGameScriptInterface::_v075() const {}
void CGameScriptInterface::_v076() const {}
void CGameScriptInterface::_v077() const {}
void CGameScriptInterface::_v078() const {}
void CGameScriptInterface::_v079() const {}
void CGameScriptInterface::_v080() const {}
void CGameScriptInterface::_v081() const {}
void CGameScriptInterface::_v082() const {}
void CGameScriptInterface::_v083() const {}
void CGameScriptInterface::_v084() const {}
void CGameScriptInterface::_v085() const {}
void CGameScriptInterface::_v086() const {}
void CGameScriptInterface::_v087() const {}
void CGameScriptInterface::_v088() const {}
void CGameScriptInterface::_v089() const {}
void CGameScriptInterface::_v090() const {}
void CGameScriptInterface::_v091() const {}
void CGameScriptInterface::_v092() const {}
void CGameScriptInterface::_v093() const {}
void CGameScriptInterface::_v094() const {}
void CGameScriptInterface::_v095() const {}
void CGameScriptInterface::_v096() const {}
void CGameScriptInterface::_v097() const {}
void CGameScriptInterface::_v098() const {}
void CGameScriptInterface::_v099() const {}
void CGameScriptInterface::_v100() const {}
void CGameScriptInterface::_v101() const {}
void CGameScriptInterface::_v102() const {}
void CGameScriptInterface::_v103() const {}
void CGameScriptInterface::_v104() const {}
void CGameScriptInterface::_v105() const {}
void CGameScriptInterface::_v106() const {}
void CGameScriptInterface::_v107() const {}
void CGameScriptInterface::_v108() const {}
void CGameScriptInterface::_v109() const {}
void CGameScriptInterface::_v110() const {}
void CGameScriptInterface::_v111() const {}
void CGameScriptInterface::_v112() const {}
void CGameScriptInterface::_v113() const {}
void CGameScriptInterface::_v114() const {}
void CGameScriptInterface::_v115() const {}
void CGameScriptInterface::_v116() const {}
void CGameScriptInterface::_v117() const {}
void CGameScriptInterface::_v118() const {}
void CGameScriptInterface::_v119() const {}
void CGameScriptInterface::_v120() const {}
void CGameScriptInterface::_v121() const {}
void CGameScriptInterface::_v122() const {}
void CGameScriptInterface::_v123() const {}
void CGameScriptInterface::_v124() const {}
void CGameScriptInterface::_v125() const {}
void CGameScriptInterface::_v126() const {}
void CGameScriptInterface::_v127() const {}
void CGameScriptInterface::_v128() const {}
void CGameScriptInterface::_v129() const {}
void CGameScriptInterface::_v130() const {}
void CGameScriptInterface::_v131() const {}
void CGameScriptInterface::_v132() const {}
void CGameScriptInterface::_v133() const {}
void CGameScriptInterface::_v134() const {}
void CGameScriptInterface::_v135() const {}
void CGameScriptInterface::_v136() const {}
void CGameScriptInterface::_v137() const {}
void CGameScriptInterface::_v138() const {}
void CGameScriptInterface::_v139() const {}
void CGameScriptInterface::_v140() const {}
void CGameScriptInterface::_v141() const {}
void CGameScriptInterface::_v142() const {}
void CGameScriptInterface::_v143() const {}
void CGameScriptInterface::_v144() const {}
void CGameScriptInterface::_v145() const {}
void CGameScriptInterface::_v146() const {}
void CGameScriptInterface::_v147() const {}
void CGameScriptInterface::_v148() const {}
void CGameScriptInterface::_v149() const {}
void CGameScriptInterface::_v150() const {}
void CGameScriptInterface::_v151() const {}
void CGameScriptInterface::_v152() const {}
void CGameScriptInterface::_v153() const {}
void CGameScriptInterface::_v154() const {}
void CGameScriptInterface::_v155() const {}
void CGameScriptInterface::_v156() const {}
void CGameScriptInterface::_v157() const {}
void CGameScriptInterface::_v158() const {}
void CGameScriptInterface::_v159() const {}
void CGameScriptInterface::_v160() const {}
void CGameScriptInterface::_v161() const {}
void CGameScriptInterface::_v162() const {}
void CGameScriptInterface::_v163() const {}
void CGameScriptInterface::_v164() const {}
void CGameScriptInterface::_v165() const {}
void CGameScriptInterface::_v166() const {}
void CGameScriptInterface::_v167() const {}
void CGameScriptInterface::_v168() const {}
void CGameScriptInterface::_v169() const {}
void CGameScriptInterface::_v170() const {}
void CGameScriptInterface::_v171() const {}
void CGameScriptInterface::_v172() const {}
void CGameScriptInterface::_v173() const {}
void CGameScriptInterface::_v174() const {}
void CGameScriptInterface::_v175() const {}
void CGameScriptInterface::_v176() const {}
void CGameScriptInterface::_v177() const {}
void CGameScriptInterface::_v178() const {}
void CGameScriptInterface::_v179() const {}
void CGameScriptInterface::_v180() const {}
void CGameScriptInterface::_v181() const {}
void CGameScriptInterface::_v182() const {}
void CGameScriptInterface::_v183() const {}
void CGameScriptInterface::_v184() const {}
void CGameScriptInterface::_v185() const {}
void CGameScriptInterface::_v186() const {}
void CGameScriptInterface::_v187() const {}
void CGameScriptInterface::_v188() const {}
void CGameScriptInterface::_v189() const {}
void CGameScriptInterface::_v190() const {}
void CGameScriptInterface::_v191() const {}
void CGameScriptInterface::_v192() const {}
void CGameScriptInterface::_v193() const {}
void CGameScriptInterface::_v194() const {}
void CGameScriptInterface::_v195() const {}
void CGameScriptInterface::_v196() const {}
void CGameScriptInterface::_v197() const {}
void CGameScriptInterface::_v198() const {}
void CGameScriptInterface::_v199() const {}
void CGameScriptInterface::_v200() const {}
void CGameScriptInterface::_v201() const {}
void CGameScriptInterface::_v202() const {}
void CGameScriptInterface::_v203() const {}
void CGameScriptInterface::_v204() const {}
void CGameScriptInterface::_v205() const {}
void CGameScriptInterface::_v206() const {}
void CGameScriptInterface::_v207() const {}
void CGameScriptInterface::_v208() const {}
void CGameScriptInterface::_v209() const {}
void CGameScriptInterface::_v210() const {}
void CGameScriptInterface::_v211() const {}
void CGameScriptInterface::_v212() const {}
void CGameScriptInterface::_v213() const {}
void CGameScriptInterface::_v214() const {}
void CGameScriptInterface::_v215() const {}
void CGameScriptInterface::_v216() const {}
void CGameScriptInterface::_v217() const {}
void CGameScriptInterface::_v218() const {}
void CGameScriptInterface::_v219() const {}
void CGameScriptInterface::_v220() const {}
void CGameScriptInterface::_v221() const {}
void CGameScriptInterface::_v222() const {}
void CGameScriptInterface::_v223() const {}
void CGameScriptInterface::_v224() const {}
void CGameScriptInterface::_v225() const {}
void CGameScriptInterface::_v226() const {}
void CGameScriptInterface::_v227() const {}
void CGameScriptInterface::_v228() const {}
void CGameScriptInterface::_v229() const {}
void CGameScriptInterface::_v230() const {}
void CGameScriptInterface::_v231() const {}
void CGameScriptInterface::_v232() const {}
void CGameScriptInterface::_v233() const {}
void CGameScriptInterface::_v234() const {}
void CGameScriptInterface::_v235() const {}
void CGameScriptInterface::_v236() const {}
void CGameScriptInterface::_v237() const {}
void CGameScriptInterface::_v238() const {}
void CGameScriptInterface::_v239() const {}
void CGameScriptInterface::_v240() const {}
void CGameScriptInterface::_v241() const {}
void CGameScriptInterface::_v242() const {}
void CGameScriptInterface::_v243() const {}
void CGameScriptInterface::_v244() const {}
void CGameScriptInterface::_v245() const {}
void CGameScriptInterface::_v246() const {}
void CGameScriptInterface::_v247() const {}
void CGameScriptInterface::_v248() const {}
void CGameScriptInterface::_v249() const {}
void CGameScriptInterface::_v250() const {}
void CGameScriptInterface::_v251() const {}
void CGameScriptInterface::_v252() const {}
void CGameScriptInterface::_v253() const {}
void CGameScriptInterface::_v254() const {}
void CGameScriptInterface::_v255() const {}
void CGameScriptInterface::_v256() const {}
void CGameScriptInterface::_v257() const {}
void CGameScriptInterface::_v258() const {}
void CGameScriptInterface::_v259() const {}
void CGameScriptInterface::_v260() const {}
void CGameScriptInterface::_v261() const {}
void CGameScriptInterface::_v262() const {}
void CGameScriptInterface::_v263() const {}
void CGameScriptInterface::_v264() const {}
void CGameScriptInterface::_v265() const {}
void CGameScriptInterface::_v266() const {}
void CGameScriptInterface::_v267() const {}
void CGameScriptInterface::_v268() const {}
void CGameScriptInterface::_v269() const {}
void CGameScriptInterface::_v270() const {}
void CGameScriptInterface::_v271() const {}
void CGameScriptInterface::_v272() const {}
void CGameScriptInterface::_v273() const {}
void CGameScriptInterface::_v274() const {}
void CGameScriptInterface::_v275() const {}
void CGameScriptInterface::_v276() const {}
void CGameScriptInterface::_v277() const {}
void CGameScriptInterface::_v278() const {}
void CGameScriptInterface::_v279() const {}
void CGameScriptInterface::_v280() const {}
void CGameScriptInterface::_v281() const {}
void CGameScriptInterface::_v282() const {}
void CGameScriptInterface::_v283() const {}
void CGameScriptInterface::_v284() const {}
void CGameScriptInterface::_v285() const {}
void CGameScriptInterface::_v286() const {}
void CGameScriptInterface::_v287() const {}
void CGameScriptInterface::_v288() const {}
void CGameScriptInterface::_v289() const {}
void CGameScriptInterface::_v290() const {}
void CGameScriptInterface::_v291() const {}
void CGameScriptInterface::_v292() const {}
void CGameScriptInterface::_v293() const {}
void CGameScriptInterface::_v294() const {}
void CGameScriptInterface::_v295() const {}
void CGameScriptInterface::_v296() const {}
void CGameScriptInterface::_v297() const {}
void CGameScriptInterface::_v298() const {}
void CGameScriptInterface::_v299() const {}
void CGameScriptInterface::_v300() const {}
void CGameScriptInterface::_v301() const {}
void CGameScriptInterface::_v302() const {}
void CGameScriptInterface::_v303() const {}
void CGameScriptInterface::_v304() const {}
void CGameScriptInterface::_v305() const {}
void CGameScriptInterface::_v306() const {}
void CGameScriptInterface::_v307() const {}
void CGameScriptInterface::_v308() const {}
void CGameScriptInterface::_v309() const {}
void CGameScriptInterface::_v310() const {}
void CGameScriptInterface::_v311() const {}
void CGameScriptInterface::_v312() const {}
void CGameScriptInterface::_v313() const {}
void CGameScriptInterface::_v314() const {}
void CGameScriptInterface::_v315() const {}
void CGameScriptInterface::_v316() const {}
void CGameScriptInterface::_v317() const {}
void CGameScriptInterface::_v318() const {}
void CGameScriptInterface::_v319() const {}
void CGameScriptInterface::_v320() const {}
void CGameScriptInterface::_v321() const {}
void CGameScriptInterface::_v322() const {}
void CGameScriptInterface::_v323() const {}
void CGameScriptInterface::_v324() const {}
void CGameScriptInterface::_v325() const {}
void CGameScriptInterface::_v326() const {}
void CGameScriptInterface::_v327() const {}
void CGameScriptInterface::_v328() const {}
void CGameScriptInterface::_v329() const {}
void CGameScriptInterface::_v330() const {}
void CGameScriptInterface::_v331() const {}
void CGameScriptInterface::_v332() const {}
void CGameScriptInterface::_v333() const {}
void CGameScriptInterface::_v334() const {}
void CGameScriptInterface::_v335() const {}
void CGameScriptInterface::_v336() const {}
void CGameScriptInterface::_v337() const {}
void CGameScriptInterface::_v338() const {}
void CGameScriptInterface::_v339() const {}
void CGameScriptInterface::_v340() const {}
void CGameScriptInterface::_v341() const {}
void CGameScriptInterface::_v342() const {}
void CGameScriptInterface::_v343() const {}
void CGameScriptInterface::_v344() const {}
void CGameScriptInterface::_v345() const {}
void CGameScriptInterface::_v346() const {}
void CGameScriptInterface::_v347() const {}
void CGameScriptInterface::_v348() const {}
void CGameScriptInterface::_v349() const {}
void CGameScriptInterface::_v350() const {}
void CGameScriptInterface::_v351() const {}
void CGameScriptInterface::_v352() const {}
void CGameScriptInterface::_v353() const {}
void CGameScriptInterface::_v354() const {}
void CGameScriptInterface::_v355() const {}
void CGameScriptInterface::_v356() const {}
void CGameScriptInterface::_v357() const {}
void CGameScriptInterface::_v358() const {}
void CGameScriptInterface::_v359() const {}
void CGameScriptInterface::_v360() const {}
void CGameScriptInterface::_v361() const {}
void CGameScriptInterface::_v362() const {}
void CGameScriptInterface::_v363() const {}
void CGameScriptInterface::_v364() const {}
void CGameScriptInterface::_v365() const {}
void CGameScriptInterface::_v366() const {}
void CGameScriptInterface::_v367() const {}
void CGameScriptInterface::_v368() const {}
void CGameScriptInterface::_v369() const {}
void CGameScriptInterface::_v370() const {}
void CGameScriptInterface::_v371() const {}
void CGameScriptInterface::_v372() const {}
void CGameScriptInterface::_v373() const {}
void CGameScriptInterface::_v374() const {}
void CGameScriptInterface::_v375() const {}
void CGameScriptInterface::_v376() const {}
void CGameScriptInterface::_v377() const {}
void CGameScriptInterface::_v378() const {}
void CGameScriptInterface::_v379() const {}
void CGameScriptInterface::_v380() const {}
void CGameScriptInterface::_v381() const {}
void CGameScriptInterface::_v382() const {}
void CGameScriptInterface::_v383() const {}
void CGameScriptInterface::_v384() const {}
void CGameScriptInterface::_v385() const {}
void CGameScriptInterface::_v386() const {}
void CGameScriptInterface::_v387() const {}
void CGameScriptInterface::_v388() const {}
void CGameScriptInterface::_v389() const {}
void CGameScriptInterface::_v390() const {}
void CGameScriptInterface::_v391() const {}
void CGameScriptInterface::_v392() const {}
void CGameScriptInterface::_v393() const {}
void CGameScriptInterface::_v394() const {}
void CGameScriptInterface::_v395() const {}
void CGameScriptInterface::_v396() const {}
void CGameScriptInterface::_v397() const {}
void CGameScriptInterface::_v398() const {}
void CGameScriptInterface::_v399() const {}
void CGameScriptInterface::_v400() const {}
void CGameScriptInterface::_v401() const {}
void CGameScriptInterface::_v402() const {}
void CGameScriptInterface::_v403() const {}
void CGameScriptInterface::_v404() const {}
void CGameScriptInterface::_v405() const {}
void CGameScriptInterface::_v406() const {}
void CGameScriptInterface::_v407() const {}
void CGameScriptInterface::_v408() const {}
void CGameScriptInterface::_v409() const {}
void CGameScriptInterface::_v410() const {}
void CGameScriptInterface::_v411() const {}
void CGameScriptInterface::_v412() const {}
void CGameScriptInterface::_v413() const {}
void CGameScriptInterface::_v414() const {}
void CGameScriptInterface::_v415() const {}
void CGameScriptInterface::_v416() const {}
void CGameScriptInterface::_v417() const {}
void CGameScriptInterface::_v418() const {}
void CGameScriptInterface::_v419() const {}
void CGameScriptInterface::_v420() const {}
void CGameScriptInterface::_v421() const {}
void CGameScriptInterface::_v422() const {}
void CGameScriptInterface::_v423() const {}
void CGameScriptInterface::_v424() const {}
void CGameScriptInterface::_v426() const {}
void CGameScriptInterface::_v427() const {}
void CGameScriptInterface::_v428() const {}
static int g_log[16]; static int g_n;
static void L(int x) { if (g_n < 16) g_log[g_n++] = x; }
static CThing g_door;
static bool g_valid;
CThing* CScriptThing::GetThing() const { L(3); return &g_door; }
bool CScriptThing::IsValid() const { L(0); return g_valid; }
void CGameScriptInterface::OpenDoor(const CScriptThing& t) const { L(1); }
void CGameScriptInterface::JamDoor(const CScriptThing& t) const { L(2); }
static int g_msgType; static unsigned long g_msgDur; static float g_px = 9; static CThing* g_msgExtra; static CThing* g_msgFrom = (CThing*)1;
void* CMessageEventManager::AddMessage(EMessageEventType ty, unsigned long d, const C3DVector& p, CThing* f, CThing* to, float a, float b, CThing* e)
{ L(4); g_msgType = ty; g_msgDur = d; g_px = p.X + p.Y + p.Z + a + b; g_msgExtra = e; g_msgFrom = f; return 0; }
static CThingPlayerCreature g_hero; static CThingPlayerCreature* g_heroPtr;
static CPlayer g_player;
CPlayer& CPlayerManager::GetMainPlayer() { return g_player; }
CThingPlayerCreature* CPlayer::GetPPlayerCharacter() const { return g_heroPtr; }
static CInterfaceMap::SEntry g_entries[2];
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) { return type && *type == 0x42 ? &g_entries[1] : last; }
static CTCEntityEvents* g_tc = (CTCEntityEvents*)0x1234;
static NEntityEvents::CEventBase* g_added; static int g_adds; static CThing* g_user;
void CTCEntityEvents::AddEvent(NEntityEvents::CEventBase* e) { L(5); g_added = e; ++g_adds; }
NEntityEvents::CEventBase::~CEventBase() {}
NEntityEvents::CEventItemUsed::CEventItemUsed(CThing& u) { g_user = &u; }
static CWorld g_world; static CMessageEventManager g_mem; static CPlayerManager g_pm;
static CGameScriptInterface g_gsi; static CScriptThing g_st;
int main()
{
    g_world.PMessageEventManager = &g_mem; g_gsi.World = &g_world; g_gsi.PlayerManager = &g_pm;
    g_entries[0].type = 0x10; g_entries[1].type = 0x42; g_entries[1].component = (CTCBase*)g_tc;
    g_door.Components.first = g_entries; g_door.Components.last = g_entries + 2;
    g_valid = false;
    g_gsi.EntitySetAsPickLocked(g_st);
    bool ok = g_n == 1;
    g_n = 0; g_valid = true; g_heroPtr = 0;
    g_gsi.EntitySetAsPickLocked(g_st);         // no hero: door opened/jammed, message posted, no event
    ok = ok && g_n == 5 && g_log[1] == 1 && g_log[2] == 2 && g_log[3] == 3 && g_log[4] == 4 && g_msgType == 0x46 && g_msgDur == 50
        && g_px == 0.0f && g_msgExtra == &g_door && g_msgFrom == 0 && g_adds == 0;
    g_n = 0; g_heroPtr = &g_hero;
    g_gsi.EntitySetAsPickLocked(g_st);         // door lacks the TC bit: no event
    ok = ok && g_adds == 0;
    g_door.UsingComponents[2] = 4;
    g_hero.Dead = 1; g_gsi.EntitySetAsPickLocked(g_st);   // dead hero: no event
    ok = ok && g_adds == 0;
    g_hero.Dead = 0; g_gsi.EntitySetAsPickLocked(g_st);
    ok = ok && g_adds == 1 && g_added != 0 && g_user == &g_hero;
    printf(ok ? "0089a390_TEST PASS\n" : "0089a390_TEST FAIL\n");
    return ok ? 0 : 1;
}