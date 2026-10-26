// CQ_AmbushTradersScript::WatchForRegionClear() -- collect the bandit allies, convoy members and phase 1 / phase 3 guards and archers, then wait until the convoy spy is dead and every one of them is dead, setting RegionClear
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F09CD0, length 1947 bytes.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, ~CBaseClass 0x0099A2E0, operator delete
//   0x00BFE9BC, free 0x00BFEA14, AreAllThingsInVectorDead 0x00CBED00 (__fastcall), CScriptBase::IsActiveThreadTerminating
//   0x00CB7940; CScriptThing slots 0 (virtual dtor, the inline ~vector) and 75 IsAlive (inline on the local spy: PImp.Data's
//   slot 75); the game script interface (+0x40) slots 7 NewScriptFrame, 72 GetThingWithScriptName, 75
//   GetAllThingsWithScriptName.
// Strings: "AT_BanditAlly" 0x012F7D78, "AT_ConvoyMember" 0x012F7CF8, "AT_Phase3Guard" 0x012F7D18, "AT_Phase3Archer"
//   0x012F7D08, "AT_Phase1Guard" 0x012F7D38, "AT_Phase1Archer" 0x012F7D28, "AT_ConvoySpy" 0x012F7CE8.
// Layout (retail = Ego_r.pdb - 0x14): RegionClear +0x64.
// Shape: generated script code; the wait is `for (;;) { if (terminating) return; ... }` (VC duplicates the head test after
//   the NewScriptFrame); every exit expands the spy's and the six vectors' destructors inline (~vector and ~CScriptThing
//   are __forceinline; vector model of the landed CV_TempleOfLightScript::WatchForAcolyteExit 0x00EDD060).
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (struct->class; already NScript). Same bytes.
extern "C" void __cdecl free(void* block);              // 0x00BFEA14
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
void operator delete(void* p);                          // 0x00BFE9BC (MSVCR71)
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
#define SLOTS_2_74 \
    virtual void t002(); virtual void t003(); virtual void t004(); virtual void t005(); virtual void t006(); virtual void t007(); \
    virtual void t008(); virtual void t009(); virtual void t010(); virtual void t011(); virtual void t012(); virtual void t013(); virtual void t014(); virtual void t015(); \
    virtual void t016(); virtual void t017(); virtual void t018(); virtual void t019(); virtual void t020(); virtual void t021(); virtual void t022(); virtual void t023(); \
    virtual void t024(); virtual void t025(); virtual void t026(); virtual void t027(); virtual void t028(); virtual void t029(); virtual void t030(); virtual void t031(); \
    virtual void t032(); virtual void t033(); virtual void t034(); virtual void t035(); virtual void t036(); virtual void t037(); virtual void t038(); virtual void t039(); \
    virtual void t040(); virtual void t041(); virtual void t042(); virtual void t043(); virtual void t044(); virtual void t045(); virtual void t046(); virtual void t047(); \
    virtual void t048(); virtual void t049(); virtual void t050(); virtual void t051(); virtual void t052(); virtual void t053(); virtual void t054(); virtual void t055(); \
    virtual void t056(); virtual void t057(); virtual void t058(); virtual void t059(); virtual void t060(); virtual void t061(); virtual void t062(); virtual void t063(); \
    virtual void t064(); virtual void t065(); virtual void t066(); virtual void t067(); virtual void t068(); virtual void t069(); virtual void t070(); virtual void t071(); \
    virtual void t072(); virtual void t073(); virtual void t074();
struct CScriptThingImpl {                               // the game-side object behind a CScriptThing (PImp)
    virtual void t000(); virtual void t001(); SLOTS_2_74
    virtual bool IsAlive() const;                       // slot 75 (+0x12c)
};
class CBaseClass { public:                                     // vtable 0x01231710
    CBaseClass();                                       // 0x0099A2D0
    virtual ~CBaseClass();                              // 0x0099A2E0
};
class CScriptThing : public CBaseClass { public:                      // 12 bytes, vtable 0x01238C8C
    virtual void t001(); SLOTS_2_74
    virtual bool IsAlive() const { return PImp.Data && PImp.Data->IsAlive(); }   // slot 75 (+0x12c), out of line 0x004AB130
    CCountedPointer<CScriptThingImpl> PImp;             // +0x04
    CScriptThing() {}
    __forceinline ~CScriptThing() {}                    // out-of-line copy 0x004AA840
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
};
class CGameScriptInterface { public:                       // the quest's game script interface (slots: ghidra_out/cgsi_slots_aligned.tsv)
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
    virtual void NewScriptFrame() const;   // slot 7 (+0x1c) NewScriptFrame
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055();
    virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063();
    virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071();
    virtual CScriptThing GetThingWithScriptName(const CCharString& name) const;   // slot 72 (+0x120) GetThingWithScriptName
    virtual void v073(); virtual void v074();
    virtual long GetAllThingsWithScriptName(const CCharString& name, vector<CScriptThing>& things) const;   // slot 75 (+0x12c) GetAllThingsWithScriptName
};
bool __fastcall AreAllThingsInVectorDead(vector<CScriptThing>* things);   // 0x00CBED00
namespace NScript {
struct CScriptBase {                                    // retail 0x40 bytes
    unsigned char _body[0x40];
    bool IsActiveThreadTerminating();                   // 0x00CB7940
};
class CQ_AmbushTradersScript : public CScriptBase {
public:
    CGameScriptInterface* Game;                         // +0x40
    void*                 Data;                         // +0x44
    unsigned char         _pad48[0x64 - 0x48];
    bool                  RegionClear;                  // +0x64
    void WatchForRegionClear();
};
void CQ_AmbushTradersScript::WatchForRegionClear()
{
    vector<CScriptThing> banditAllies;
    vector<CScriptThing> convoyMembers;
    vector<CScriptThing> phase1Guards;
    vector<CScriptThing> phase1Archers;
    vector<CScriptThing> phase3Guards;
    vector<CScriptThing> phase3Archers;
    Game->GetAllThingsWithScriptName("AT_BanditAlly", banditAllies);
    Game->GetAllThingsWithScriptName("AT_ConvoyMember", convoyMembers);
    Game->GetAllThingsWithScriptName("AT_Phase3Guard", phase3Guards);
    Game->GetAllThingsWithScriptName("AT_Phase3Archer", phase3Archers);
    Game->GetAllThingsWithScriptName("AT_Phase1Guard", phase1Guards);
    Game->GetAllThingsWithScriptName("AT_Phase1Archer", phase1Archers);
    CScriptThing spy = Game->GetThingWithScriptName("AT_ConvoySpy");
    for (;;) {
        if (IsActiveThreadTerminating())
            return;
        RegionClear = !spy.IsAlive() && AreAllThingsInVectorDead(&banditAllies) && AreAllThingsInVectorDead(&convoyMembers)
            && AreAllThingsInVectorDead(&phase3Guards) && AreAllThingsInVectorDead(&phase3Archers)
            && AreAllThingsInVectorDead(&phase1Guards) && AreAllThingsInVectorDead(&phase1Archers);
        if (RegionClear)
            break;
        Game->NewScriptFrame();
    }
    if (IsActiveThreadTerminating())
        return;
    if (IsActiveThreadTerminating())
        return;
}
}