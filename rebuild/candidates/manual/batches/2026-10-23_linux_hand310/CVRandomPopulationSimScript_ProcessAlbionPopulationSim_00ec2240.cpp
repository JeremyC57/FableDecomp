// CV_RandomPopulationSimScript::ProcessAlbionPopulationSim() -- each time a region loads (outside the Greatwood/Fisherman regions while Q_BountyHunt runs) look up its REGION_<NAME> def, set the current population region and copy the def's three population maxima; the high-detail sim is active unless all three are zero
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EC2240, 0x2DF bytes, plain `ret`.
// Callees: CCharString() 0x0099E4B0 / (const char*, long) 0x0099EBF0 / (const CCharString&) 0x0099EC30 / ToUpper 0x0099EE20 /
//   ~ 0x0099EAE0, operator+ 0x0099F570 (__fastcall, hidden result in ecx), Compare 0x00411570 and
//   CDefinitionManager::GetDef<T> 0x00CDE280 (both DEFINED here under auto_inline(off): with their bodies in the TU VC7.1
//   knows neither leaks or writes the locals, so it keeps the region's PData and the def pointer in esi across the
//   following calls as retail does; declared extern they are reloaded after every call), GFGetPDefinitionManager
//   0x00CD3EF0, GetPDefFromInstantiationName 0x009ADA40, SetCurrentPopulationRegionFromRegionName 0x00EC2520 (the
//   manifest's CScriptBase_ProcessAlbionPopulationSim_Filter), IsActiveThreadTerminating 0x00CB7940; Game slots 7
//   NewScriptFrame, 22 MsgOnRegionLoaded, 284 IsQuestActive. The def's type name is not recovered (MaxPopulation[3] +0x3c).
// Layout (retail = Ego_r.pdb - 0x14): Game +0x40, HighDetailSimulationActive +0x58, PopulationSim[3] +0x78.
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
struct CCharStringData {                        // 17 bytes
    CBasicStringChar Data;                      // +0x00
    long             NoRefs;                    // +0x0d
};
#pragma pack(pop)
struct CCharString {
    CCharStringData* PStringData;
    CCharString();                                          // 0x0099E4B0
    CCharString(const char* text, long length = -1);        // 0x0099EBF0
    CCharString(const CCharString& other);                  // 0x0099EC30
    ~CCharString();                                         // 0x0099EAE0
    CCharString ToUpper();                                  // 0x0099EE20
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
CCharString __fastcall operator+(const CCharString& a, const CCharString& b);   // 0x0099F570
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                                 // slot 1
};
struct CRefCounted {                                        // second base: the count sits at +4 behind the vptr
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CDefBase : CDefVirtual, CRefCounted {
    void Release()
    {
        if (--m_count == 0)
            Destroy();
    }
    void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
struct CRegionDef : CDefBase {                              // the REGION_<name> def (type name not recovered)
    unsigned char _pad08[0x3c - 0x08];
    long MaxPopulation[3];                                  // +0x3c bandits / villagers / guards
};
struct CDefPointer {
    CRegionDef* m_p;
    CDefPointer() : m_p(0) { }
    ~CDefPointer()
    {
        if (m_p)
            m_p->Release();
    }
    CDefPointer& operator=(const CDefPointer& other)
    {
        if (m_p != other.m_p) {
            if (m_p) {
                m_p->Release();
                m_p = 0;
            }
            m_p = other.m_p;
            if (m_p)
                ++m_p->m_count;
        }
        return *this;
    }
    CRegionDef* operator->() const { return m_p; }
};
struct CDefinitionManager {
    CDefPointer GetPDefFromInstantiationName(const CCharString& name);   // 0x009ADA40 (hidden result pointer)
    bool GetDef(const CCharString& name, CDefPointer& out);   // 0x00CDE280 (body below: the template's header definition)
};
#pragma auto_inline(off)
bool CDefinitionManager::GetDef(const CCharString& name, CDefPointer& out)
{
    CDefPointer def = GetPDefFromInstantiationName(name);
    if (def.m_p) {
        out = def;
        return true;
    }
    return false;
}
#pragma auto_inline(on)
CDefinitionManager* GFGetPDefinitionManager();              // 0x00CD3EF0
struct CGameScriptInterface {                       // retail vtable 0x01260F0C (slots: ghidra_out/cgsi_slots_aligned.tsv)
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
    virtual void NewScriptFrame(void) const;   // slot 7 (+0x1c) 0x006E7410
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017();
    virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021();
    virtual bool MsgOnRegionLoaded(CCharString&) const;   // slot 22 (+0x58) 0x008933F0
    virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031(); virtual void v032();
    virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041(); virtual void v042();
    virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052();
    virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062();
    virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071(); virtual void v072();
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
    virtual void v283();
    virtual bool IsQuestActive(const CCharString&) const;   // slot 284 (+0x470) 0x00892F40
};
struct CScriptBase {                                        // retail 0x40 bytes
    virtual void v00();
    unsigned char _body[0x3c];
    bool IsActiveThreadTerminating();                       // 0x00CB7940
};
struct CPopulationSim {
    long MaxPopulation;
};
struct CV_RandomPopulationSimScript : CScriptBase {
    CGameScriptInterface* Game;                             // +0x40
    void*                 PMasterData;                      // +0x44
    float CurrentPopulationScale[4];                        // +0x48
    bool  HighDetailSimulationActive;                       // +0x58
    long  CurrentPopulationRegion;                          // +0x5c
    long  CreaturesAlive[3];                                // +0x60
    unsigned long BanditCreatureGeneratorIndex;             // +0x6c
    unsigned long VillagerCreatureGeneratorIndex;           // +0x70
    unsigned long GuardCreatureGeneratorIndex;              // +0x74
    CPopulationSim PopulationSim[3];                        // +0x78
    void SetCurrentPopulationRegionFromRegionName(CCharString& region);   // 0x00EC2520
    void ProcessAlbionPopulationSim();
};

void CV_RandomPopulationSimScript::ProcessAlbionPopulationSim()
{
    CCharString region;
    while (!IsActiveThreadTerminating()) {
        if (Game->MsgOnRegionLoaded(region)) {
            if (IsActiveThreadTerminating())
                return;
            bool bountyHuntRegion = false;
            if (Game->IsQuestActive("Q_BountyHunt")) {
                if (IsActiveThreadTerminating())
                    return;
                if (region == "GreatwoodEntrance" || region == "GreatwoodLake" || region == "Fisherman") {
                    if (IsActiveThreadTerminating())
                        return;
                    bountyHuntRegion = true;
                }
            }
            HighDetailSimulationActive = false;
            CDefPointer def;
            CCharString defName = CCharString("REGION_") + CCharString(region).ToUpper();
            if (!bountyHuntRegion && GFGetPDefinitionManager()->GetDef(defName, def)) {
                if (IsActiveThreadTerminating())
                    return;
                SetCurrentPopulationRegionFromRegionName(region);
                PopulationSim[0].MaxPopulation = def->MaxPopulation[0];
                PopulationSim[1].MaxPopulation = def->MaxPopulation[1];
                PopulationSim[2].MaxPopulation = def->MaxPopulation[2];
                if (PopulationSim[0].MaxPopulation == 0 && PopulationSim[1].MaxPopulation == 0 && PopulationSim[2].MaxPopulation == 0) {
                    if (IsActiveThreadTerminating())
                        return;
                    HighDetailSimulationActive = false;
                } else {
                    if (IsActiveThreadTerminating())
                        return;
                    HighDetailSimulationActive = true;
                }
            }
        }
        Game->NewScriptFrame();
    }
}
