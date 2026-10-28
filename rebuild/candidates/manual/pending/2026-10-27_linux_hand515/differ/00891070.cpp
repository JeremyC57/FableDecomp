// CGameScriptInterface::CameraUseCameraPoint(const CScriptThing& cameraPoint, const CScriptThing& subject, float time, long a, long b) const -- script call: forwards to the position/RH-set overload with the live subject's physics position and orientation
// Grade: DIFFER(83v83) -- retail tests PhysicsTC in ecx (`mov ecx,[eax+60h] ; test ecx,ecx ; je`) before `push esi ; mov esi,ecx`; ours loads it straight into esi after the push (one-register placement only)
// Leaf: ?CameraUseCameraPoint@CGameScriptInterface@@UBEXABVCScriptThing@@0MJJ@Z
// Retail: Fable.exe @ 0x00891070, 83 bytes, thiscall `ret 14h`.
// Identity (proven): retail CGameScriptInterface vtable 0x01260F0C slot 413 holds this body; slot 413 is
//   ?CameraUseCameraPoint@CGameScriptInterface@@UBEXABVCScriptThing@@0MJJ@Z (ghidra_out/cgsi_slots_aligned.tsv); it calls
//   slot 411 (+0x66c) ?CameraUseCameraPoint@CGameScriptInterface@@UBEXABVCScriptThing@@PBVC3DVector@@PBVCRightHandedSet@@MJJ@Z.
// Body: t = subject.GetThing() (CScriptThing slot 11); if t, not dead (+0x91 bit 0) and its PhysicsTC (+0x60; Ego_r.pdb
//   CThing::PhysicsTC +0x70, retail -0x10 after the 4-byte STLport containers) is set: forward cameraPoint,
//   &PhysicsTC->GetPosition() (inline, Position +0x0c; FableWin.pdb ?GetPosition@CTCPhysicsBase@@QBEABVC3DVector@@XZ),
//   &PhysicsTC->GetRHSet() (virtual slot 72, +0x120; ?GetRHSet@CTCPhysicsBase@@UBEABVCRightHandedSet@@XZ), time, a, b.
//   Retail calls GetRHSet before it loads any argument, so the RH set is a named local ahead of the call; the PhysicsTC
//   null test is the third term of the guard (`mov ecx,[eax+60h] ; test ; je` before `push esi`) and the component is
//   then re-read into a local. The two long parameter
//   names are not in the name sets (descriptive).
class C3DVector { public: float X, Y, Z; };
class CRightHandedSet;
class CTCPhysicsBase {
public:
    virtual void _v000(); virtual void _v001(); virtual void _v002(); virtual void _v003(); virtual void _v004(); virtual void _v005(); virtual void _v006(); virtual void _v007();
    virtual void _v008(); virtual void _v009(); virtual void _v010(); virtual void _v011(); virtual void _v012(); virtual void _v013(); virtual void _v014(); virtual void _v015();
    virtual void _v016(); virtual void _v017(); virtual void _v018(); virtual void _v019(); virtual void _v020(); virtual void _v021(); virtual void _v022(); virtual void _v023();
    virtual void _v024(); virtual void _v025(); virtual void _v026(); virtual void _v027(); virtual void _v028(); virtual void _v029(); virtual void _v030(); virtual void _v031();
    virtual void _v032(); virtual void _v033(); virtual void _v034(); virtual void _v035(); virtual void _v036(); virtual void _v037(); virtual void _v038(); virtual void _v039();
    virtual void _v040(); virtual void _v041(); virtual void _v042(); virtual void _v043(); virtual void _v044(); virtual void _v045(); virtual void _v046(); virtual void _v047();
    virtual void _v048(); virtual void _v049(); virtual void _v050(); virtual void _v051(); virtual void _v052(); virtual void _v053(); virtual void _v054(); virtual void _v055();
    virtual void _v056(); virtual void _v057(); virtual void _v058(); virtual void _v059(); virtual void _v060(); virtual void _v061(); virtual void _v062(); virtual void _v063();
    virtual void _v064(); virtual void _v065(); virtual void _v066(); virtual void _v067(); virtual void _v068(); virtual void _v069(); virtual void _v070(); virtual void _v071();
    virtual const CRightHandedSet& GetRHSet() const;   // slot 72 (+0x120)
    unsigned char _pad04[0x08];
    C3DVector Position;                                  // +0x0c
    const C3DVector& GetPosition() const { return Position; }
};
class CThing {
public:
    unsigned char _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;                           // +0x60
    unsigned char _pad64[0x91 - 0x64];
    unsigned char Dead : 1;                              // +0x91 bit 0
    bool IsDead() const { return Dead; }
};
class CScriptThing {
public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005();
    virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010();
    virtual CThing* GetThing() const;                    // slot 11 (+0x2c)
};
class CCharString;
class CGameScriptInterface {
public:
    virtual void _v000(); virtual void _v001(); virtual void _v002(); virtual void _v003(); virtual void _v004(); virtual void _v005(); virtual void _v006(); virtual void _v007();
    virtual void _v008(); virtual void _v009(); virtual void _v010(); virtual void _v011(); virtual void _v012(); virtual void _v013(); virtual void _v014(); virtual void _v015();
    virtual void _v016(); virtual void _v017(); virtual void _v018(); virtual void _v019(); virtual void _v020(); virtual void _v021(); virtual void _v022(); virtual void _v023();
    virtual void _v024(); virtual void _v025(); virtual void _v026(); virtual void _v027(); virtual void _v028(); virtual void _v029(); virtual void _v030(); virtual void _v031();
    virtual void _v032(); virtual void _v033(); virtual void _v034(); virtual void _v035(); virtual void _v036(); virtual void _v037(); virtual void _v038(); virtual void _v039();
    virtual void _v040(); virtual void _v041(); virtual void _v042(); virtual void _v043(); virtual void _v044(); virtual void _v045(); virtual void _v046(); virtual void _v047();
    virtual void _v048(); virtual void _v049(); virtual void _v050(); virtual void _v051(); virtual void _v052(); virtual void _v053(); virtual void _v054(); virtual void _v055();
    virtual void _v056(); virtual void _v057(); virtual void _v058(); virtual void _v059(); virtual void _v060(); virtual void _v061(); virtual void _v062(); virtual void _v063();
    virtual void _v064(); virtual void _v065(); virtual void _v066(); virtual void _v067(); virtual void _v068(); virtual void _v069(); virtual void _v070(); virtual void _v071();
    virtual void _v072(); virtual void _v073(); virtual void _v074(); virtual void _v075(); virtual void _v076(); virtual void _v077(); virtual void _v078(); virtual void _v079();
    virtual void _v080(); virtual void _v081(); virtual void _v082(); virtual void _v083(); virtual void _v084(); virtual void _v085(); virtual void _v086(); virtual void _v087();
    virtual void _v088(); virtual void _v089(); virtual void _v090(); virtual void _v091(); virtual void _v092(); virtual void _v093(); virtual void _v094(); virtual void _v095();
    virtual void _v096(); virtual void _v097(); virtual void _v098(); virtual void _v099(); virtual void _v100(); virtual void _v101(); virtual void _v102(); virtual void _v103();
    virtual void _v104(); virtual void _v105(); virtual void _v106(); virtual void _v107(); virtual void _v108(); virtual void _v109(); virtual void _v110(); virtual void _v111();
    virtual void _v112(); virtual void _v113(); virtual void _v114(); virtual void _v115(); virtual void _v116(); virtual void _v117(); virtual void _v118(); virtual void _v119();
    virtual void _v120(); virtual void _v121(); virtual void _v122(); virtual void _v123(); virtual void _v124(); virtual void _v125(); virtual void _v126(); virtual void _v127();
    virtual void _v128(); virtual void _v129(); virtual void _v130(); virtual void _v131(); virtual void _v132(); virtual void _v133(); virtual void _v134(); virtual void _v135();
    virtual void _v136(); virtual void _v137(); virtual void _v138(); virtual void _v139(); virtual void _v140(); virtual void _v141(); virtual void _v142(); virtual void _v143();
    virtual void _v144(); virtual void _v145(); virtual void _v146(); virtual void _v147(); virtual void _v148(); virtual void _v149(); virtual void _v150(); virtual void _v151();
    virtual void _v152(); virtual void _v153(); virtual void _v154(); virtual void _v155(); virtual void _v156(); virtual void _v157(); virtual void _v158(); virtual void _v159();
    virtual void _v160(); virtual void _v161(); virtual void _v162(); virtual void _v163(); virtual void _v164(); virtual void _v165(); virtual void _v166(); virtual void _v167();
    virtual void _v168(); virtual void _v169(); virtual void _v170(); virtual void _v171(); virtual void _v172(); virtual void _v173(); virtual void _v174(); virtual void _v175();
    virtual void _v176(); virtual void _v177(); virtual void _v178(); virtual void _v179(); virtual void _v180(); virtual void _v181(); virtual void _v182(); virtual void _v183();
    virtual void _v184(); virtual void _v185(); virtual void _v186(); virtual void _v187(); virtual void _v188(); virtual void _v189(); virtual void _v190(); virtual void _v191();
    virtual void _v192(); virtual void _v193(); virtual void _v194(); virtual void _v195(); virtual void _v196(); virtual void _v197(); virtual void _v198(); virtual void _v199();
    virtual void _v200(); virtual void _v201(); virtual void _v202(); virtual void _v203(); virtual void _v204(); virtual void _v205(); virtual void _v206(); virtual void _v207();
    virtual void _v208(); virtual void _v209(); virtual void _v210(); virtual void _v211(); virtual void _v212(); virtual void _v213(); virtual void _v214(); virtual void _v215();
    virtual void _v216(); virtual void _v217(); virtual void _v218(); virtual void _v219(); virtual void _v220(); virtual void _v221(); virtual void _v222(); virtual void _v223();
    virtual void _v224(); virtual void _v225(); virtual void _v226(); virtual void _v227(); virtual void _v228(); virtual void _v229(); virtual void _v230(); virtual void _v231();
    virtual void _v232(); virtual void _v233(); virtual void _v234(); virtual void _v235(); virtual void _v236(); virtual void _v237(); virtual void _v238(); virtual void _v239();
    virtual void _v240(); virtual void _v241(); virtual void _v242(); virtual void _v243(); virtual void _v244(); virtual void _v245(); virtual void _v246(); virtual void _v247();
    virtual void _v248(); virtual void _v249(); virtual void _v250(); virtual void _v251(); virtual void _v252(); virtual void _v253(); virtual void _v254(); virtual void _v255();
    virtual void _v256(); virtual void _v257(); virtual void _v258(); virtual void _v259(); virtual void _v260(); virtual void _v261(); virtual void _v262(); virtual void _v263();
    virtual void _v264(); virtual void _v265(); virtual void _v266(); virtual void _v267(); virtual void _v268(); virtual void _v269(); virtual void _v270(); virtual void _v271();
    virtual void _v272(); virtual void _v273(); virtual void _v274(); virtual void _v275(); virtual void _v276(); virtual void _v277(); virtual void _v278(); virtual void _v279();
    virtual void _v280(); virtual void _v281(); virtual void _v282(); virtual void _v283(); virtual void _v284(); virtual void _v285(); virtual void _v286(); virtual void _v287();
    virtual void _v288(); virtual void _v289(); virtual void _v290(); virtual void _v291(); virtual void _v292(); virtual void _v293(); virtual void _v294(); virtual void _v295();
    virtual void _v296(); virtual void _v297(); virtual void _v298(); virtual void _v299(); virtual void _v300(); virtual void _v301(); virtual void _v302(); virtual void _v303();
    virtual void _v304(); virtual void _v305(); virtual void _v306(); virtual void _v307(); virtual void _v308(); virtual void _v309(); virtual void _v310(); virtual void _v311();
    virtual void _v312(); virtual void _v313(); virtual void _v314(); virtual void _v315(); virtual void _v316(); virtual void _v317(); virtual void _v318(); virtual void _v319();
    virtual void _v320(); virtual void _v321(); virtual void _v322(); virtual void _v323(); virtual void _v324(); virtual void _v325(); virtual void _v326(); virtual void _v327();
    virtual void _v328(); virtual void _v329(); virtual void _v330(); virtual void _v331(); virtual void _v332(); virtual void _v333(); virtual void _v334(); virtual void _v335();
    virtual void _v336(); virtual void _v337(); virtual void _v338(); virtual void _v339(); virtual void _v340(); virtual void _v341(); virtual void _v342(); virtual void _v343();
    virtual void _v344(); virtual void _v345(); virtual void _v346(); virtual void _v347(); virtual void _v348(); virtual void _v349(); virtual void _v350(); virtual void _v351();
    virtual void _v352(); virtual void _v353(); virtual void _v354(); virtual void _v355(); virtual void _v356(); virtual void _v357(); virtual void _v358(); virtual void _v359();
    virtual void _v360(); virtual void _v361(); virtual void _v362(); virtual void _v363(); virtual void _v364(); virtual void _v365(); virtual void _v366(); virtual void _v367();
    virtual void _v368(); virtual void _v369(); virtual void _v370(); virtual void _v371(); virtual void _v372(); virtual void _v373(); virtual void _v374(); virtual void _v375();
    virtual void _v376(); virtual void _v377(); virtual void _v378(); virtual void _v379(); virtual void _v380(); virtual void _v381(); virtual void _v382(); virtual void _v383();
    virtual void _v384(); virtual void _v385(); virtual void _v386(); virtual void _v387(); virtual void _v388(); virtual void _v389(); virtual void _v390(); virtual void _v391();
    virtual void _v392(); virtual void _v393(); virtual void _v394(); virtual void _v395(); virtual void _v396(); virtual void _v397(); virtual void _v398(); virtual void _v399();
    virtual void _v400(); virtual void _v401(); virtual void _v402(); virtual void _v403(); virtual void _v404(); virtual void _v405(); virtual void _v406(); virtual void _v407();
    virtual void _v408(); virtual void _v409(); virtual void _v410();
    virtual void CameraUseCameraPoint(const CCharString& cameraPointName, const CScriptThing& subject, float time, long a, long b) const;   // slot 414 (+0x678): VC7.1 lays an overload group out in reverse declaration order
    virtual void CameraUseCameraPoint(const CScriptThing& cameraPoint, const CScriptThing& subject, float time, long a, long b) const;   // slot 413 (+0x674)
    virtual void CameraUseCameraPoint(const CCharString& cameraPointName, const C3DVector* position, const CRightHandedSet* rhSet, float time, long a, long b) const;   // slot 412 (+0x670)
    virtual void CameraUseCameraPoint(const CScriptThing& cameraPoint, const C3DVector* position, const CRightHandedSet* rhSet, float time, long a, long b) const;   // slot 411 (+0x66c)
};
void CGameScriptInterface::CameraUseCameraPoint(const CScriptThing& cameraPoint, const CScriptThing& subject, float time, long a, long b) const
{
    CThing* t = subject.GetThing();
    if (t && !t->IsDead() && t->PhysicsTC)
    {
        CTCPhysicsBase* physics = t->PhysicsTC;
        const CRightHandedSet& rhSet = physics->GetRHSet();
        CameraUseCameraPoint(cameraPoint, &physics->GetPosition(), &rhSet, time, a, b);
    }
}
