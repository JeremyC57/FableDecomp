// CQ_AmbushTradersScript::WatchForQuestSuccess() -- wait for Deserted Farm, for the spy contact and for Bowerstone Jail; then, while the quest is not finished and Bowerstone Posh is loaded, finish the quest once the convoy spy (AT_ConvoySpy) follows the hero; when the quest is finished and the success speech is done, mark the quest succeeded
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F0A470, length 1127 bytes.
// Callees: CBaseClass ctor/dtor 0x0099A2D0 / 0x0099A2E0, CCharString() 0x0099E4B0 / CCharString(const char*, long) 0x0099EBF0 /
//   ~CCharString 0x0099EAE0, Compare 0x00411570 (the inlined CCharString == const char*), operator delete 0x00BFE9BC, free
//   0x00BFEA14, ~CScriptThing 0x004AA840, vector<CScriptThing>::~vector 0x008AC970 (COMDAT-folded, propagated label
//   "~vector<CConversation>"), CScriptBase::IsActiveThreadTerminating 0x00CB7940; CScriptThing slots 0 (virtual dtor),
//   1 GetName, 75 IsAlive; the game script interface (+0x40) slots 7, 12, 70, 72, 231.
// Strings: "DesertedFarm" 0x012C7778, "BowerstoneJail" 0x012C7788, "AT_ConvoySpy" 0x012F7CE8, "BowerstonePosh" 0x012C6C6C.
// Layout (retail = Ego_r.pdb - 0x14): QuestFinished +0x49, QuestSucceeded +0x4a, SpyFollowing +0x5b, SpyFollowingHero +0x5c,
//   SpyContactMade +0x5f, QuestSuccessSpeechComplete +0x65.
// Shape: generated script code (`if (IsActiveThreadTerminating()) return;` after every statement); inline CCharString == of
//   the landed CQ_BountyHuntScript 0x00D151D0; destructors as the landed CMeleeThunder::Main 0x00D58080 (inline only at the
//   first wait loop's return, every later exit under in-body inline_depth(0) calls the out-of-line copies). ~vector and the
//   CCharString == are __forceinline: without it the first exit calls ~vector and the == is called out of line.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (struct->class; already NScript). Same bytes.
#include <string.h>
#pragma intrinsic(strcmp)
extern "C" void __cdecl free(void* block);              // 0x00BFEA14
void operator delete(void* p);                          // 0x00BFE9BC (MSVCR71)
long __fastcall Compare_00411570(const char* a, const char* b);   // 0x00411570
#pragma pack(push, 1)
struct CBasicStringChar {                               // CBasicString<char>, 13 bytes
    char*         PData;                                // +0x00
    unsigned long StringLength;                         // +0x04
    unsigned long DataLength : 31;                      // +0x08
    bool          UseFastExtend : 1;                    // +0x0c
    bool operator==(const char* text) const { return Compare_00411570(PData, text) ? false : true; }
};
class CCharStringData { public:                                // 17 bytes
    CBasicStringChar Data;                              // +0x00
    long             NoRefs;                            // +0x0d
};
#pragma pack(pop)
class CCharString { public:
    CCharStringData* PStringData;
    CCharString();                                      // 0x0099E4B0
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
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
class CCPPointerInfo { public:                                 // CCountedPointer's shared block
    long RefCount;                                      // +0
    void (__fastcall* DeleteFunc)(void* data);          // +4
    void* Data;                                         // +8
};
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) {}
    __forceinline ~CCountedPointer()
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
struct CScriptThingImpl;
class CBaseClass { public:                                     // vtable 0x01231710
    CBaseClass();                                       // 0x0099A2D0
    virtual ~CBaseClass();                              // 0x0099A2E0
};
class CScriptThing : public CBaseClass { public:                      // 12 bytes, vtable 0x01238C8C
    virtual const CCharString& GetName() const;         // slot 1 (+0x04) 0x004AA8B0
    virtual void t002(); virtual void t003(); virtual void t004(); virtual void t005(); virtual void t006(); virtual void t007();
    virtual void t008(); virtual void t009(); virtual void t010(); virtual void t011(); virtual void t012(); virtual void t013(); virtual void t014(); virtual void t015();
    virtual void t016(); virtual void t017(); virtual void t018(); virtual void t019(); virtual void t020(); virtual void t021(); virtual void t022(); virtual void t023();
    virtual void t024(); virtual void t025(); virtual void t026(); virtual void t027(); virtual void t028(); virtual void t029(); virtual void t030(); virtual void t031();
    virtual void t032(); virtual void t033(); virtual void t034(); virtual void t035(); virtual void t036(); virtual void t037(); virtual void t038(); virtual void t039();
    virtual void t040(); virtual void t041(); virtual void t042(); virtual void t043(); virtual void t044(); virtual void t045(); virtual void t046(); virtual void t047();
    virtual void t048(); virtual void t049(); virtual void t050(); virtual void t051(); virtual void t052(); virtual void t053(); virtual void t054(); virtual void t055();
    virtual void t056(); virtual void t057(); virtual void t058(); virtual void t059(); virtual void t060(); virtual void t061(); virtual void t062(); virtual void t063();
    virtual void t064(); virtual void t065(); virtual void t066(); virtual void t067(); virtual void t068(); virtual void t069(); virtual void t070(); virtual void t071();
    virtual void t072(); virtual void t073(); virtual void t074();
    virtual bool IsAlive() const;                       // slot 75 (+0x12c) 0x004AB130
    CCountedPointer<CScriptThingImpl> PImp;             // +0x04
    CScriptThing() {}
    ~CScriptThing() {}                                  // out-of-line copy 0x004AA840
};
template <class T> inline void _Destroy(T* first, T* last)
{
    for (; first != last; ++first)
        first->~T();
}
template <class T> struct vector {                      // STLport vector (retail 12 bytes), malloc_alloc storage
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    __forceinline ~vector()                             // out-of-line copy 0x008AC970
    {
        _Destroy(_M_start, _M_finish);
        if (_M_start)
            free(_M_start);
    }
    unsigned long size() const { return _M_finish - _M_start; }
    T& operator[](unsigned long i) { return _M_start[i]; }
    void pop_back() { --_M_finish; _M_finish->~T(); }
};
class CGameScriptInterface { public:                       // the quest's game script interface (slots: ghidra_out/cgsi_slots_aligned.tsv)
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
    virtual void NewScriptFrame() const;   // slot 7 (+0x1c) NewScriptFrame
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011();
    virtual bool IsRegionLoaded(const CCharString& region) const;   // slot 12 (+0x30) IsRegionLoaded
    virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020();
    virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028();
    virtual void v029(); virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036();
    virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044();
    virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052();
    virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060();
    virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068();
    virtual void v069();
    virtual CScriptThing& GetHero();   // slot 70 (+0x118) GetHero
    virtual void v071();
    virtual CScriptThing GetThingWithScriptName(const CCharString& name) const;   // slot 72 (+0x120) GetThingWithScriptName
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
    virtual void v225(); virtual void v226(); virtual void v227(); virtual void v228(); virtual void v229(); virtual void v230();
    virtual bool GetFollowingEntityList(const CScriptThing& thing, vector<CScriptThing>& followers) const;   // slot 231 (+0x39c) GetFollowingEntityList
};
namespace NScript {
}   // namespace NScript (CScriptBase is global in retail)
class CScriptBase { public:                                    // retail 0x40 bytes
    unsigned char _body[0x40];
    bool IsActiveThreadTerminating();                   // 0x00CB7940
};
namespace NScript {
class CQ_AmbushTradersScript : public CScriptBase {
public:
    CGameScriptInterface* Game;                         // +0x40
    void*                 Data;                         // +0x44
    bool QuestFailed;                                   // +0x48
    bool QuestFinished;                                 // +0x49
    bool QuestSucceeded;                                // +0x4a
    bool QuestOver;                                     // +0x4b
    bool RegionInvalid;                                 // +0x4c
    bool BanditIntroDone;                               // +0x4d
    bool TraderEscaped;                                 // +0x4e
    bool NoEnemiesLeft;                                 // +0x4f
    bool PhaseComplete;                                 // +0x50
    bool ConvoyTrigger2;                                // +0x51
    bool AllBanditsDead;                                // +0x52
    bool AllTradersDead;                                // +0x53
    bool AllGuardsDead;                                 // +0x54
    bool ContactDead;                                   // +0x55
    bool ContactAttackedByHero;                         // +0x56
    bool HeroDead;                                      // +0x57
    bool SpyDead;                                       // +0x58
    bool SentryAttackedByHero;                          // +0x59
    bool SpySeenBySentry;                               // +0x5a
    bool SpyFollowing;                                  // +0x5b
    bool SpyFollowingHero;                              // +0x5c
    bool SpyHeroProximity;                              // +0x5d
    bool SpyWanderReset;                                // +0x5e
    bool SpyContactMade;                                // +0x5f
    bool SpyEscaped;                                    // +0x60
    bool SpyExited;                                     // +0x61
    bool SpyReachedNoEntryMarker;                       // +0x62
    bool AnyTraderEscaped;                              // +0x63
    bool RegionClear;                                   // +0x64
    bool QuestSuccessSpeechComplete;                    // +0x65
    void WatchForQuestSuccess();
};
void CQ_AmbushTradersScript::WatchForQuestSuccess()
{
    CScriptThing thing;                                 // declared but never used: retail constructs and destroys it
    vector<CScriptThing> followers;
    CCharString name;                                   // likewise unused
    while (!Game->IsRegionLoaded("DesertedFarm")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        {
#pragma inline_depth(0)
            return;
#pragma inline_depth()
        }
    while (!SpyContactMade) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            {
#pragma inline_depth(0)
                return;
#pragma inline_depth()
            }
    }
    if (IsActiveThreadTerminating())
        {
#pragma inline_depth(0)
            return;
#pragma inline_depth()
        }
    while (!Game->IsRegionLoaded("BowerstoneJail")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            {
#pragma inline_depth(0)
                return;
#pragma inline_depth()
            }
    }
    if (IsActiveThreadTerminating())
        {
#pragma inline_depth(0)
            return;
#pragma inline_depth()
        }
    CScriptThing spy = Game->GetThingWithScriptName("AT_ConvoySpy");
    if (IsActiveThreadTerminating())
        {
#pragma inline_depth(0)
            return;
#pragma inline_depth()
        }
    for (;;) {
        if (Game->IsRegionLoaded("BowerstonePosh") && !QuestFinished) {
            if (IsActiveThreadTerminating())
                {
#pragma inline_depth(0)
                    return;
#pragma inline_depth()
                }
            while (followers.size() != 0)
                followers.pop_back();
            Game->GetFollowingEntityList(Game->GetHero(), followers);
            long count = followers.size();
            if (count > 0) {
                if (IsActiveThreadTerminating())
                    {
#pragma inline_depth(0)
                        return;
#pragma inline_depth()
                    }
                SpyFollowing = false;
                for (long i = 0; i < count; ++i) {
                    if (followers[i].IsAlive() && followers[i].GetName() == "AT_ConvoySpy") {
                        if (IsActiveThreadTerminating())
                            {
#pragma inline_depth(0)
                                return;
#pragma inline_depth()
                            }
                        SpyFollowing = true;
                        break;
                    }
                }
            }
            if (SpyFollowingHero || SpyFollowing) {
                if (IsActiveThreadTerminating())
                    {
#pragma inline_depth(0)
                        return;
#pragma inline_depth()
                    }
                QuestFinished = true;
            }
        }
        if (QuestFinished && QuestSuccessSpeechComplete)
            break;
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            {
#pragma inline_depth(0)
                return;
#pragma inline_depth()
            }
    }
    if (IsActiveThreadTerminating())
        {
#pragma inline_depth(0)
            return;
#pragma inline_depth()
        }
    QuestSucceeded = true;
    if (IsActiveThreadTerminating())
        {
#pragma inline_depth(0)
            return;
#pragma inline_depth()
        }
    {
#pragma inline_depth(0)
        return;
#pragma inline_depth()
    }
}
}