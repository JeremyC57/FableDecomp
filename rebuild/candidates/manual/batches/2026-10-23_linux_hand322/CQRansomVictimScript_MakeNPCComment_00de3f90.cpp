// CQ_RansomVictimScript::MakeNPCComment(name, npc, comment) -- if the NPC is alive and the comment timer has run out, start a conversation between the NPC and the hero with the NPC's pre- or post-rescue line for this comment; for the father's-guard comment also let the other NPC (the victim when the guard is speaking, else the guard) answer with its response line when it is alive, different from the speaker and the line exists; restart the comment timer and report that a comment was made
// Leaf: MakeNPCComment
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DE3F90, 1263 bytes (int3 padded to 1264); `ret 0Ch`.
// Callees: ~CScriptThing 0x004AA840, CScriptThing copy 0x004ABE90 (and inline), CScriptThing::IsAlive 0x004AB130 (slot 75),
//   CScriptThing::GetDataString 0x004AA900, IsActiveThreadTerminating 0x00CB7940, CCharString 0x0099EBF0 / ~ 0x0099EAE0,
//   operator+ 0x0099F690 (const char*, CCharString) / 0x0099F600 (CCharString, const char*) / 0x0099F570 (CCharString,
//   CCharString) (all __fastcall); impl slot 78 (+0x138) equality; g_pGameScriptInterface 0x0143E8F8 timer slots 89, 90;
//   g_pScriptDef 0x0143E90C (+0x50 comment interval); Game slots 70, 72, 358, 364, 365, 366.
// Layout (retail = Ego_r.pdb - 0x14): RescuedVictim +0x4b, NPCCommentTimer +0x68.
#define THING_INLINE_COPY
#define VEC_DTOR
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
    __forceinline CCountedPointer& operator=(const CCountedPointer& other)
    {
        CCPPointerInfo* info = other.Info;
        T* data = other.Data;
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
    CCountedPointer(const CCountedPointer& o) : Data(o.Data), Info(o.Info) { if (Info) ++Info->RefCount; }
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
struct CScriptThingImpl {
    virtual void i000();
    virtual void i001();
    virtual void i002();
    virtual void i003();
    virtual void i004();
    virtual void i005();
    virtual const C3DVector& GetPos() const;            // imp slot 6 (+0x18)
    virtual void i007(); virtual void i008(); virtual void i009(); virtual void i010(); virtual void i011(); virtual void i012(); virtual void i013(); virtual void i014(); virtual void i015(); virtual void i016(); virtual void i017(); virtual void i018(); virtual void i019(); virtual void i020(); virtual void i021(); virtual void i022(); virtual void i023(); virtual void i024(); virtual void i025(); virtual void i026(); virtual void i027(); virtual void i028(); virtual void i029(); virtual void i030(); virtual void i031(); virtual void i032(); virtual void i033(); virtual void i034(); virtual void i035(); virtual void i036(); virtual void i037(); virtual void i038(); virtual void i039(); virtual void i040(); virtual void i041(); virtual void i042(); virtual void i043(); virtual void i044(); virtual void i045(); virtual void i046(); virtual void i047(); virtual void i048(); virtual void i049(); virtual void i050(); virtual void i051(); virtual void i052(); virtual void i053(); virtual void i054(); virtual void i055(); virtual void i056(); virtual void i057(); virtual void i058(); virtual void i059(); virtual void i060(); virtual void i061(); virtual void i062(); virtual void i063(); virtual void i064(); virtual void i065(); virtual void i066(); virtual void i067(); virtual void i068(); virtual void i069();
    virtual void SetToKillOnLevelUnload(bool kill);     // imp slot 70 (+0x118)
    virtual void i071(); virtual void i072(); virtual void i073(); virtual void i074(); virtual void i075(); virtual void i076(); virtual void i077();
    virtual bool IsEqualTo(const CScriptThingImpl* other) const;   // imp slot 78 (+0x138)
};
extern const C3DVector g_ZeroVector;                    // 0x0143E8E0
struct CScriptThing : CBaseClass {                      // 12 bytes, vtable 0x01238C8C
    virtual void t001(); virtual void t002(); virtual CCharString GetDataString() const;         // slot 3 (+0x0c) 0x004AA900
    virtual void t004(); virtual void t005();
    virtual const C3DVector& GetPos() const             // slot 6 (+0x18) 0x004AA980
    {
        if (!PImp.Data)
            return g_ZeroVector;
        return PImp.Data->GetPos();
    }
    virtual void t007(); virtual void t008(); virtual void t009(); virtual void t010(); virtual void t011(); virtual void t012(); virtual void t013(); virtual void t014(); virtual void t015(); virtual void t016(); virtual void t017(); virtual void t018(); virtual void t019(); virtual void t020(); virtual void t021(); virtual void t022(); virtual void t023(); virtual void t024(); virtual void t025(); virtual void t026(); virtual void t027(); virtual void t028(); virtual void t029(); virtual void t030(); virtual void t031(); virtual void t032(); virtual void t033(); virtual void t034(); virtual void t035(); virtual void t036(); virtual void t037(); virtual void t038(); virtual void t039(); virtual void t040(); virtual void t041(); virtual void t042(); virtual void t043(); virtual void t044(); virtual void t045(); virtual void t046(); virtual void t047(); virtual void t048(); virtual void t049(); virtual void t050(); virtual void t051(); virtual void t052(); virtual void t053(); virtual void t054(); virtual void t055(); virtual void t056(); virtual void t057(); virtual void t058(); virtual void t059(); virtual void t060(); virtual void t061(); virtual void t062(); virtual void t063(); virtual void t064(); virtual void t065(); virtual void t066(); virtual void t067(); virtual void t068(); virtual void t069(); virtual void SetToKillOnLevelUnload(bool kill)      // slot 70 (+0x118) 0x004AB0C0
    {
        if (PImp.Data)
            PImp.Data->SetToKillOnLevelUnload(kill);
    }
    virtual void t071(); virtual void t072(); virtual void t073(); virtual void t074();
    virtual bool IsAlive() const;                       // slot 75 (+0x12c) 0x004AB130
    CCountedPointer<CScriptThingImpl> PImp;             // +0x04 Data, +0x08 Info
#ifdef THING_INLINE_COPY
    CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) {}
#else
    CScriptThing(const CScriptThing& other);            // 0x004ABE90
#endif
    CScriptThing() {}
    __forceinline bool operator==(const CScriptThing& other) const { return PImp.Data->IsEqualTo(other.PImp.Data); }
    __forceinline bool operator!=(const CScriptThing& other) const { return !PImp.Data->IsEqualTo(other.PImp.Data); }
    __forceinline CScriptThing& operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }
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
#ifndef VEC_DTOR
#define VEC_DTOR __forceinline
#endif
    VEC_DTOR ~vector()
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
CCharString __fastcall operator+(const char* left, const CCharString& right);          // 0x0099F690
CCharString __fastcall operator+(const CCharString& left, const char* right);          // 0x0099F600
CCharString __fastcall operator+(const CCharString& left, const CCharString& right);   // 0x0099F570
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
    virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079(); virtual void v080(); virtual void v081(); virtual void v082();
    virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086(); virtual void v087();
    virtual void DeregisterTimer(long timer);   // slot 88 (+0x160)
    virtual void SetTimer(long timer, long time);   // slot 89 (+0x164)
    virtual long GetTimer(long timer) const;   // slot 90 (+0x168)
    virtual void v091(); virtual void v092(); virtual void v093(); virtual void v094(); virtual void v095(); virtual void v096(); virtual void v097(); virtual void v098(); virtual void v099(); virtual void v100();
    virtual void v101(); virtual void v102(); virtual void v103(); virtual void v104(); virtual void v105(); virtual void v106(); virtual void v107(); virtual void v108(); virtual void v109(); virtual void v110();
    virtual void v111(); virtual void v112(); virtual void v113(); virtual void v114(); virtual void v115(); virtual void v116(); virtual void v117(); virtual void v118(); virtual void v119(); virtual void v120();
    virtual void v121(); virtual void v122(); virtual void v123(); virtual void v124(); virtual void v125(); virtual void v126(); virtual void v127(); virtual void v128(); virtual void v129(); virtual void v130();
    virtual void v131(); virtual void v132(); virtual void v133(); virtual void v134(); virtual void v135(); virtual void v136(); virtual void v137(); virtual void v138(); virtual void v139(); virtual void v140();
    virtual void v141(); virtual void v142(); virtual void v143(); virtual void v144(); virtual void v145(); virtual void v146(); virtual void v147(); virtual void v148(); virtual void v149(); virtual void v150();
    virtual void v151(); virtual void v152(); virtual void v153(); virtual void v154(); virtual void v155(); virtual void v156(); virtual void v157(); virtual void v158(); virtual void v159(); virtual void v160();
    virtual void v161(); virtual void v162(); virtual void v163(); virtual void v164(); virtual void v165(); virtual void v166(); virtual void v167(); virtual void v168(); virtual void v169(); virtual void v170();
    virtual void v171(); virtual void v172(); virtual void v173(); virtual void v174(); virtual void v175(); virtual void v176(); virtual void v177(); virtual void v178(); virtual void v179(); virtual void v180();
    virtual void v181(); virtual void v182(); virtual void v183(); virtual void v184(); virtual void v185(); virtual void v186(); virtual void v187(); virtual void v188(); virtual void v189(); virtual void v190();
    virtual void v191(); virtual void v192(); virtual void v193(); virtual void v194(); virtual void v195(); virtual void v196(); virtual void v197(); virtual void v198(); virtual void v199(); virtual void v200();
    virtual void v201(); virtual void v202(); virtual void v203(); virtual void v204(); virtual void v205(); virtual void v206(); virtual void v207(); virtual void v208(); virtual void v209(); virtual void v210();
    virtual void v211(); virtual void v212(); virtual void v213(); virtual void v214(); virtual void v215(); virtual void v216(); virtual void v217(); virtual void v218(); virtual void v219(); virtual void v220();
    virtual void v221(); virtual void v222(); virtual void v223(); virtual void v224(); virtual void v225(); virtual void v226(); virtual void v227(); virtual void v228(); virtual void v229(); virtual void v230();
    virtual void v231(); virtual void v232(); virtual void v233(); virtual void v234(); virtual void v235(); virtual void v236(); virtual void v237(); virtual void v238(); virtual void v239(); virtual void v240();
    virtual void v241(); virtual void v242(); virtual void v243(); virtual void v244(); virtual void v245(); virtual void v246(); virtual void v247(); virtual void v248(); virtual void v249(); virtual void v250();
    virtual void v251(); virtual void v252(); virtual void v253(); virtual void v254(); virtual void v255(); virtual void v256(); virtual void v257(); virtual void v258(); virtual void v259(); virtual void v260();
    virtual void v261(); virtual void v262(); virtual void v263(); virtual void v264(); virtual void v265(); virtual void v266(); virtual void v267(); virtual void v268(); virtual void v269(); virtual void v270();
    virtual void v271(); virtual void v272(); virtual void v273(); virtual void v274(); virtual void v275(); virtual void v276(); virtual void v277(); virtual void v278(); virtual void v279(); virtual void v280();
    virtual void v281(); virtual void v282(); virtual void v283(); virtual void v284(); virtual void v285(); virtual void v286(); virtual void v287(); virtual void v288(); virtual void v289(); virtual void v290();
    virtual void v291(); virtual void v292(); virtual void v293(); virtual void v294(); virtual void v295(); virtual void v296(); virtual void v297(); virtual void v298(); virtual void v299(); virtual void v300();
    virtual void v301(); virtual void v302(); virtual void v303(); virtual void v304(); virtual void v305(); virtual void v306(); virtual void v307(); virtual void v308(); virtual void v309(); virtual void v310();
    virtual void v311(); virtual void v312(); virtual void v313(); virtual void v314(); virtual void v315(); virtual void v316(); virtual void v317(); virtual void v318(); virtual void v319(); virtual void v320();
    virtual void v321(); virtual void v322(); virtual void v323(); virtual void v324(); virtual void v325(); virtual void v326(); virtual void v327(); virtual void v328(); virtual void v329(); virtual void v330();
    virtual void v331(); virtual void v332(); virtual void v333(); virtual void v334(); virtual void v335(); virtual void v336(); virtual void v337(); virtual void v338(); virtual void v339(); virtual void v340();
    virtual void v341(); virtual void v342(); virtual void v343(); virtual void v344(); virtual void v345(); virtual void v346(); virtual void v347(); virtual void v348(); virtual void v349(); virtual void v350();
    virtual void v351(); virtual void v352(); virtual void v353(); virtual void v354(); virtual void v355(); virtual void v356(); virtual void v357();
    virtual bool TextEntryExists(const CCharString& text) const;   // slot 358 (+0x598)
    virtual void v359(); virtual void v360(); virtual void v361(); virtual void v362(); virtual void v363();
    virtual long AddNewConversation(const CScriptThing& speaker, bool a, bool b) const;   // slot 364 (+0x5b0)
    virtual void AddPersonToConversation(long conversation, const CScriptThing& person) const;   // slot 365 (+0x5b4)
    virtual void AddLineToConversation(long conversation, const CCharString& text, bool a, const CScriptThing& speaker, const CScriptThing& listener) const;   // slot 366 (+0x5b8)
};
extern CGameScriptInterface* g_pGameScriptInterface;    // 0x0143E8F8
struct CTimer {
    long Index;
    __forceinline ~CTimer() { g_pGameScriptInterface->DeregisterTimer(Index); }   // 0x00CD4470
    void Set(long time) { g_pGameScriptInterface->SetTimer(Index, time); }
    long Get() const { return g_pGameScriptInterface->GetTimer(Index); }
};
struct CScriptDef {
    unsigned char _pad[0x50];
    long NPCCommentInterval;                            // +0x50
};
extern CScriptDef* g_pScriptDef;                        // 0x0143E90C
struct CScriptBase {                                    // retail 0x40 bytes
    unsigned char _body[0x40];
    bool IsActiveThreadTerminating();                   // 0x00CB7940
};
enum ENPCComment { NPC_COMMENT_VICTIM, NPC_COMMENT_GUARD };
struct CQ_RansomVictimScript : CScriptBase {
    CGameScriptInterface* Game;                         // +0x40
    void*                 PMasterData;                  // +0x44
    bool QuestStartScreened;                            // +0x48
    bool MissionSucceeded;                              // +0x49
    bool MissionFailed;                                 // +0x4a
    bool RescuedVictim;                                 // +0x4b
    unsigned char _pad4c[0x68 - 0x4c];
    CTimer NPCCommentTimer;                             // +0x68
    bool MakeNPCComment(const CCharString& name, const CScriptThing& npc, ENPCComment comment);
};

bool CQ_RansomVictimScript::MakeNPCComment(const CCharString& name, const CScriptThing& npc, ENPCComment comment)
{
    CScriptThing speaker = npc;
    const CScriptThing& thing = speaker;
    if (!thing.IsAlive()) {
        if (IsActiveThreadTerminating())
            return false;
        return false;
    }
    if (NPCCommentTimer.Get() > 0) {
        if (IsActiveThreadTerminating())
            return false;
        return false;
    }
    long conversation = Game->AddNewConversation(speaker, false, false);
    Game->AddPersonToConversation(conversation, Game->GetHero());
    if (RescuedVictim) {
        if (IsActiveThreadTerminating())
            return false;
        Game->AddLineToConversation(conversation, "TEXT_QST_B06_POST_RESC_" + speaker.GetDataString() + "_" + name, false, speaker, Game->GetHero());
    } else {
        if (IsActiveThreadTerminating())
            return false;
        Game->AddLineToConversation(conversation, "TEXT_QST_B06_PRE_RESC_" + speaker.GetDataString() + "_" + name, false, speaker, Game->GetHero());
    }
    if (comment == NPC_COMMENT_GUARD) {
        if (IsActiveThreadTerminating())
            return false;
        CScriptThing guard = Game->GetThingWithScriptName("VictimsFatherGuard");
        CScriptThing other = (guard.IsAlive() && speaker == guard) ? Game->GetThingWithScriptName("Victim") : Game->GetThingWithScriptName("VictimsFatherGuard");
        if (other.IsAlive() && other != speaker && Game->TextEntryExists("TEXT_QST_B06_PRE_RESC_" + other.GetDataString() + "_" + name + "_RESPONSE")) {
            if (IsActiveThreadTerminating())
                return false;
            Game->AddPersonToConversation(conversation, other);
            Game->AddLineToConversation(conversation, "TEXT_QST_B06_PRE_RESC_" + other.GetDataString() + "_" + name + "_RESPONSE", false, other, Game->GetHero());
        }
    }
    NPCCommentTimer.Set(g_pScriptDef->NPCCommentInterval);
    return true;
}
