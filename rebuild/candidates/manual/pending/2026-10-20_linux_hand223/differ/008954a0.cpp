// Grade: DIFFER(276v274) -- only the def release register: retail `mov ecx,[esp+14h] ; test ecx,ecx ; je ; dec [ecx+4] ; jne ; mov edx,[ecx]`, ours loads eax and adds `mov ecx,eax` before the slot-1 call
// CGameScriptInterface::EntityFollowThing(const CScriptThing& follower, const CScriptThing& target, float distance, bool keepUp) const -- when both script things are alive, let the follower follow the hero and post an EXPRESSION_FOLLOW "expression performed to" event from the target to the follower's entity-event component
// Leaf: EntityFollowThing
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008954A0, length 274 bytes (+ int3 padding), `ret 10h`.
// Signature from Ego_r.pdb: ?EntityFollowThing@CGameScriptInterface@@UBEXABVCScriptThing@@0M_N@Z.
// Callees: CScriptThing slots 75 IsAlive and 11 GetThing, CGameDefinitionManager::Get @ 0x0044C6B0 (__fastcall),
//   CDefinitionManager::GetDef<CExpressionDef>(const CCharString&, CDefPointer*) @ 0x00593666, CCharString(const char*,
//   long) @ 0x0099EBF0 ("EXPRESSION_FOLLOW" @ 0x012757EC) / ~CCharString @ 0x0099EAE0, the TC map lower bound @ 0x0040F020,
//   this interface's slot 235 EntitySetAsAllowedToFollowHero (cgsi_slots_aligned.tsv), operator new @ 0x00BFEA1A (0x20),
//   __ftol2 @ 0x00BFEA70, NEntityEvents::CEventExpressionPerformedTo(CThingCreatureBase*, long, const CDefPointer&, long,
//   long) @ 0x0084B010 (landed), CTCEntityEvents::AddEvent @ 0x008113C0, the def's slot 1 on release.
// Layout: retail CThing interface bitset +0x20 (TC 0x42 = +0x28 bit 2), map +0x44.
// Shape: the target thing is parked in the `push ecx` frame word, the follower thing kept in esi; the def pointer and
//   the expression name live in the dead `follower`/`target` argument slots (the name in its own block); the component
//   lookup is the HasTC + one-ternary GetTC form; the def is released through the pointee's Release (`dec [ecx+4]`).
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
void* __cdecl operator new(unsigned int size);          // 0x00BFEA1A
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                     // +0x20
    CInterfaceMap m_interfaces;                         // +0x44
    bool HasTC(long type) const { if (m_presentBits[type >> 5] & (1UL << (type & 31))) return true; return false; }
    template <class T> __forceinline T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
};
class CThingCreatureBase : public CThing {};
struct CScriptThing {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010();
    virtual CThing* GetThing() const;                   // slot 11 (+0x2c)
    virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027();
    virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035();
    virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043();
    virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051();
    virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067();
    virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074();
    virtual bool IsAlive() const;                       // slot 75 (+0x12c)
    void* PImp[2];
};
struct CExpressionDef {
    virtual void v00();
    virtual void Destroy();                             // slot 1
    long RefCount;                                      // +0x04
};
static __forceinline bool DecRefToZero(long& count) { return --count == 0; }
struct CDefPointerExpressionDef {
    CExpressionDef* Object;
    CDefPointerExpressionDef() : Object(0) {}
    ~CDefPointerExpressionDef() { CExpressionDef* p = Object; if (p) { if (DecRefToZero(p->RefCount)) p->Destroy(); } }
};
struct CGameDefinitionManager {
    static CGameDefinitionManager& __fastcall Get();    // 0x0044C6B0
    bool GetDef(const CCharString& name, CDefPointerExpressionDef* out);   // 0x00593666
};
namespace NEntityEvents {
struct CEventBase {};
struct CEventExpressionPerformedTo : CEventBase {       // 0x20 bytes
    unsigned char _body[0x20];
    CEventExpressionPerformedTo(CThingCreatureBase* performer, long frame, const CDefPointerExpressionDef& expression,
                                long distance, long keepUp);   // 0x0084B010
};
}
struct CTCEntityEvents {
    void AddEvent(NEntityEvents::CEventBase* event);    // 0x008113C0
};
struct CGameScriptInterface_Base {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055();
    virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063();
    virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071();
    virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079();
    virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086(); virtual void v087();
    virtual void v088(); virtual void v089(); virtual void v090(); virtual void v091(); virtual void v092(); virtual void v093(); virtual void v094(); virtual void v095();
    virtual void v096(); virtual void v097(); virtual void v098(); virtual void v099(); virtual void v100(); virtual void v101(); virtual void v102(); virtual void v103();
    virtual void v104(); virtual void v105(); virtual void v106(); virtual void v107(); virtual void v108(); virtual void v109(); virtual void v110(); virtual void v111();
    virtual void v112(); virtual void v113(); virtual void v114(); virtual void v115(); virtual void v116(); virtual void v117(); virtual void v118(); virtual void v119();
    virtual void v120(); virtual void v121(); virtual void v122(); virtual void v123(); virtual void v124(); virtual void v125(); virtual void v126(); virtual void v127();
    virtual void v128(); virtual void v129(); virtual void v130(); virtual void v131(); virtual void v132(); virtual void v133(); virtual void v134(); virtual void v135();
    virtual void v136(); virtual void v137(); virtual void v138(); virtual void v139(); virtual void v140(); virtual void v141(); virtual void v142(); virtual void v143();
    virtual void v144(); virtual void v145(); virtual void v146(); virtual void v147(); virtual void v148(); virtual void v149(); virtual void v150(); virtual void v151();
    virtual void v152(); virtual void v153(); virtual void v154(); virtual void v155(); virtual void v156(); virtual void v157(); virtual void v158(); virtual void v159();
    virtual void v160(); virtual void v161(); virtual void v162(); virtual void v163(); virtual void v164(); virtual void v165(); virtual void v166(); virtual void v167();
    virtual void v168(); virtual void v169(); virtual void v170(); virtual void v171(); virtual void v172(); virtual void v173(); virtual void v174(); virtual void v175();
    virtual void v176(); virtual void v177(); virtual void v178(); virtual void v179(); virtual void v180(); virtual void v181(); virtual void v182(); virtual void v183();
    virtual void v184(); virtual void v185(); virtual void v186(); virtual void v187(); virtual void v188(); virtual void v189(); virtual void v190(); virtual void v191();
    virtual void v192(); virtual void v193(); virtual void v194(); virtual void v195(); virtual void v196(); virtual void v197(); virtual void v198(); virtual void v199();
    virtual void v200(); virtual void v201(); virtual void v202(); virtual void v203(); virtual void v204(); virtual void v205(); virtual void v206(); virtual void v207();
    virtual void v208(); virtual void v209(); virtual void v210(); virtual void v211(); virtual void v212(); virtual void v213(); virtual void v214(); virtual void v215();
    virtual void v216(); virtual void v217(); virtual void v218(); virtual void v219(); virtual void v220(); virtual void v221(); virtual void v222(); virtual void v223();
    virtual void v224(); virtual void v225(); virtual void v226(); virtual void v227(); virtual void v228(); virtual void v229(); virtual void v230(); virtual void v231();
    virtual void v232(); virtual void v233(); virtual void v234();
    virtual void EntitySetAsAllowedToFollowHero(const CScriptThing& thing, bool allowed) const;   // slot 235 (+0x3ac)
};
struct CGameScriptInterface : CGameScriptInterface_Base {
    void EntityFollowThing(const CScriptThing& follower, const CScriptThing& target, float distance, bool keepUp) const;
};

void CGameScriptInterface::EntityFollowThing(const CScriptThing& follower, const CScriptThing& target, float distance,
                                             bool keepUp) const
{
    if (!follower.IsAlive())
        return;
    if (!target.IsAlive())
        return;
    CThing* targetThing = target.GetThing();
    CThing* followerThing = follower.GetThing();
    CDefPointerExpressionDef def;
    {
        CCharString name("EXPRESSION_FOLLOW", -1);
        CGameDefinitionManager::Get().GetDef(name, &def);
    }
    if (followerThing->HasTC(0x42)) {
        CTCEntityEvents* events = followerThing->GetTC<CTCEntityEvents>(0x42);
        EntitySetAsAllowedToFollowHero(follower, true);
        events->AddEvent(new NEntityEvents::CEventExpressionPerformedTo((CThingCreatureBase*)targetThing, 0, def,
                                                                        (long)distance, keepUp));
    }
}
