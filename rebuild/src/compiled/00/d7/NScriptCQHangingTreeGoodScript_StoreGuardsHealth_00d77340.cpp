// CQ_HangingTreeGoodScript::StoreGuardsHealth() -- record the current health of every guard in GuardsHealthVector (stopping early if the script thread is terminating)
// Leaf: StoreGuardsHealth
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D77340, 149 bytes (+ int3 padding).
// Label correction: the manifest's "CEngineWorldMap::BuildAndSaveSea" is propagated. The body sits between the
//   CQ_HangingTreeGoodScript functions (0x00D766C0 SetGuardsHealth, 0x00D773E0 MonitorRegionPresence), uses the quest
//   interface pointer at +0x40 and calls GetHealth on CurrentGuardVector's things, pushing into GuardsHealthVector:
//   it is the getter half of SetGuardsHealth (real name unknown; descriptive).
// Callees: CScriptBase::IsActiveThreadTerminating @ 0x00CB7940 (landed), CGameScriptInterface::GetHealth(const CScriptThing&)
//   slot 264 (+0x420, ghidra_out/cgsi_slots_aligned.tsv), vector<float>::push_back(const float&) @ 0x00568850 (manifest
//   "Vector_PushElement").
// Layout (Ego_r.pdb CQ_HangingTreeGoodScript less 0x14 for the retail CScriptBase, less 4 per earlier retail 12-byte vector):
//   CurrentGuardVector (vector<CScriptThing>, 12-byte things) begin/end +0xac/+0xb0, GuardsHealthVector +0xd0.
// Shape: the guard count `(end - begin) / 12` is re-derived each pass (unsigned compare) and the element offset is
//   strength-reduced (ebx += 12); the terminating test exits without the trailing call; the health float is a named
//   local stored in the `push ecx` frame word and passed by address; a final `if (IsActiveThreadTerminating()) return;`
//   leaves just the call.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
struct CScriptThing {                                       // 12 bytes
    void* _vptr;
    void* m_pObject;
    void* m_pCount;
};

struct CGameScriptInterface_Calls {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005();
    virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011();
    virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017();
    virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035();
    virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041();
    virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053();
    virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065();
    virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071();
    virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077();
    virtual void v078(); virtual void v079(); virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083();
    virtual void v084(); virtual void v085(); virtual void v086(); virtual void v087(); virtual void v088(); virtual void v089();
    virtual void v090(); virtual void v091(); virtual void v092(); virtual void v093(); virtual void v094(); virtual void v095();
    virtual void v096(); virtual void v097(); virtual void v098(); virtual void v099(); virtual void v100(); virtual void v101();
    virtual void v102(); virtual void v103(); virtual void v104(); virtual void v105(); virtual void v106(); virtual void v107();
    virtual void v108(); virtual void v109(); virtual void v110(); virtual void v111(); virtual void v112(); virtual void v113();
    virtual void v114(); virtual void v115(); virtual void v116(); virtual void v117(); virtual void v118(); virtual void v119();
    virtual void v120(); virtual void v121(); virtual void v122(); virtual void v123(); virtual void v124(); virtual void v125();
    virtual void v126(); virtual void v127(); virtual void v128(); virtual void v129(); virtual void v130(); virtual void v131();
    virtual void v132(); virtual void v133(); virtual void v134(); virtual void v135(); virtual void v136(); virtual void v137();
    virtual void v138(); virtual void v139(); virtual void v140(); virtual void v141(); virtual void v142(); virtual void v143();
    virtual void v144(); virtual void v145(); virtual void v146(); virtual void v147(); virtual void v148(); virtual void v149();
    virtual void v150(); virtual void v151(); virtual void v152(); virtual void v153(); virtual void v154(); virtual void v155();
    virtual void v156(); virtual void v157(); virtual void v158(); virtual void v159(); virtual void v160(); virtual void v161();
    virtual void v162(); virtual void v163(); virtual void v164(); virtual void v165(); virtual void v166(); virtual void v167();
    virtual void v168(); virtual void v169(); virtual void v170(); virtual void v171(); virtual void v172(); virtual void v173();
    virtual void v174(); virtual void v175(); virtual void v176(); virtual void v177(); virtual void v178(); virtual void v179();
    virtual void v180(); virtual void v181(); virtual void v182(); virtual void v183(); virtual void v184(); virtual void v185();
    virtual void v186(); virtual void v187(); virtual void v188(); virtual void v189(); virtual void v190(); virtual void v191();
    virtual void v192(); virtual void v193(); virtual void v194(); virtual void v195(); virtual void v196(); virtual void v197();
    virtual void v198(); virtual void v199(); virtual void v200(); virtual void v201(); virtual void v202(); virtual void v203();
    virtual void v204(); virtual void v205(); virtual void v206(); virtual void v207(); virtual void v208(); virtual void v209();
    virtual void v210(); virtual void v211(); virtual void v212(); virtual void v213(); virtual void v214(); virtual void v215();
    virtual void v216(); virtual void v217(); virtual void v218(); virtual void v219(); virtual void v220(); virtual void v221();
    virtual void v222(); virtual void v223(); virtual void v224(); virtual void v225(); virtual void v226(); virtual void v227();
    virtual void v228(); virtual void v229(); virtual void v230(); virtual void v231(); virtual void v232(); virtual void v233();
    virtual void v234(); virtual void v235(); virtual void v236(); virtual void v237(); virtual void v238(); virtual void v239();
    virtual void v240(); virtual void v241(); virtual void v242(); virtual void v243(); virtual void v244(); virtual void v245();
    virtual void v246(); virtual void v247(); virtual void v248(); virtual void v249(); virtual void v250(); virtual void v251();
    virtual void v252(); virtual void v253(); virtual void v254(); virtual void v255(); virtual void v256(); virtual void v257();
    virtual void v258(); virtual void v259(); virtual void v260(); virtual void v261(); virtual void v262(); virtual void v263();
    virtual float GetHealth(const CScriptThing& thing) const;   // slot 264 (+0x420)
};

template <class T> struct CRetailVector {        // retail vector: begin, end, capacity
    T* m_pBegin;
    T* m_pEnd;
    T* m_pCapacity;
    unsigned long size() const { return m_pEnd - m_pBegin; }
    T& operator[](unsigned long i) { return m_pBegin[i]; }
    void push_back(const T& value);              // 0x00568850 (float instance)
};

struct CScriptBase {                                        // retail 0x40 bytes (PDB 0x54: debug containers)
    virtual void v00();
    unsigned char _body[0x3c];
    bool IsActiveThreadTerminating();                       // 0x00CB7940
};

struct CQuestScriptBase : CScriptBase {                     // retail 0x48 bytes
    CGameScriptInterface_Calls* GSI;                        // +0x40
    unsigned char _pad44[4];                                // +0x44
};

namespace NScript {
class CQ_HangingTreeGoodScript : public CQuestScriptBase {
public:
    unsigned char               _pad48[0xac - 0x48];
    CRetailVector<CScriptThing> CurrentGuardVector;         // +0xac
    unsigned char               _padb8[0xd0 - 0xb8];        // RegionsWhereSpawned, AreasSpawnedIn
    CRetailVector<float>        GuardsHealthVector;         // +0xd0
    void StoreGuardsHealth();
};
} // namespace NScript
using NScript::CQ_HangingTreeGoodScript;

void NScript::CQ_HangingTreeGoodScript::StoreGuardsHealth()
{
    for (unsigned long i = 0; i < CurrentGuardVector.size(); ++i) {
        if (IsActiveThreadTerminating())
            return;
        float health = GSI->GetHealth(CurrentGuardVector[i]);
        GuardsHealthVector.push_back(health);
    }
    if (IsActiveThreadTerminating())
        return;
}