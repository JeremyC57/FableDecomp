// CQ_HangingTreeEvilScript::CleanUp() -- remove the HTE_FreedomFighter things, then (unless the thread is ending) the prisoner and the two guards if they still live
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D6B720, length 922 bytes; called from 0x00D69AD0 and 0x00D69B8B.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CScriptBase::IsActiveThreadTerminating
//   0x00CB7940, free 0x00BFEA14 (inline vector destructor copies), operator delete 0x00BFE9BC / ~CBaseClass 0x0099A2E0
//   (inline ~CScriptThing copies); CScriptThing slot 0 (virtual destructor) and IsAlive (slot 75, inline); through
//   the interface (+0x40): GetThingWithScriptName (72), GetAllThingsWithScriptName (75), RemoveThing (108).
// Strings: "GUARD1" 0x012D2018, "GUARD2" 0x012D2010, "HTE_Prisoner" 0x012D2030, "HTE_FreedomFighter" 0x012D2050.
// Layout (retail = Ego_r.pdb - 0x14): Prisoner +0x80.
// Shape: CScriptThing model of the landed CQ_HangingTreeGoodScript::ReassignLeader 0x00D74F80 (inline IsAlive, empty
//   inline destructor); vector model of the landed CQ_HobbeCaveScript::RescuedBandit 0x00D91740.
extern "C" void __cdecl free(void* p);             // 0x00BFEA14
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
void operator delete(void* p);                          // 0x00BFE9BC (MSVCR71)
struct CCPPointerInfo {                                 // CCountedPointer's shared block
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);          // destroys the pointee
    void* Data;
};
template <class T> struct CCountedPointer {
    T*              Data;
    CCPPointerInfo* Info;
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
struct CScriptThing : CBaseClass {                      // 12 bytes, vtable 0x01238C8C; PImp is the game-side thing
    virtual void t001(); virtual void t002(); virtual void t003(); virtual void t004(); virtual void t005(); virtual void t006(); virtual void t007(); virtual void t008();
    virtual void t009(); virtual void t010(); virtual void t011(); virtual void t012(); virtual void t013(); virtual void t014(); virtual void t015(); virtual void t016();
    virtual void t017(); virtual void t018(); virtual void t019(); virtual void t020(); virtual void t021(); virtual void t022(); virtual void t023(); virtual void t024();
    virtual void t025(); virtual void t026(); virtual void t027(); virtual void t028(); virtual void t029(); virtual void t030(); virtual void t031(); virtual void t032();
    virtual void t033(); virtual void t034(); virtual void t035(); virtual void t036(); virtual void t037(); virtual void t038(); virtual void t039(); virtual void t040();
    virtual void t041(); virtual void t042(); virtual void t043(); virtual void t044(); virtual void t045(); virtual void t046(); virtual void t047(); virtual void t048();
    virtual void t049(); virtual void t050(); virtual void t051(); virtual void t052(); virtual void t053(); virtual void t054(); virtual void t055(); virtual void t056();
    virtual void t057(); virtual void t058(); virtual void t059(); virtual void t060(); virtual void t061(); virtual void t062(); virtual void t063(); virtual void t064();
    virtual void t065(); virtual void t066(); virtual void t067(); virtual void t068(); virtual void t069(); virtual void t070(); virtual void t071(); virtual void t072();
    virtual void t073(); virtual void t074();
    CCountedPointer<CScriptThing> PImp;                 // +0x04 Data, +0x08 Info
    ~CScriptThing() {}                                  // out of line 0x004AA840
    virtual bool IsAlive() const { return PImp.Data && PImp.Data->IsAlive(); }   // slot 75 (+0x12c), out of line 0x004AB130
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
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    T& operator[](unsigned int n) { return _M_start[n]; }
};
}
struct CGameScriptInterface {                       // the quest's game script interface (slots: ghidra_out/cgsi_slots_aligned.tsv)
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
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
    virtual long GetAllThingsWithScriptName(const CCharString& name, std::vector<CScriptThing>& things) const;   // slot 75 (+0x12c) GetAllThingsWithScriptName
    virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079(); virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083();
    virtual void v084(); virtual void v085(); virtual void v086(); virtual void v087(); virtual void v088(); virtual void v089(); virtual void v090(); virtual void v091();
    virtual void v092(); virtual void v093(); virtual void v094(); virtual void v095(); virtual void v096(); virtual void v097(); virtual void v098(); virtual void v099();
    virtual void v100(); virtual void v101(); virtual void v102(); virtual void v103(); virtual void v104(); virtual void v105(); virtual void v106(); virtual void v107();
    virtual void RemoveThing(const CScriptThing& thing, bool a, bool b) const;   // slot 108 (+0x1b0) RemoveThing
};
namespace NScript {
struct CScriptBase {                                    // retail 0x40 bytes
    unsigned char _body[0x40];
    bool IsActiveThreadTerminating();                   // 0x00CB7940
};
struct CQ_HangingTreeEvilScript : CScriptBase {
    CGameScriptInterface* Game;                         // +0x40
    void*                 Data;                         // +0x44
    unsigned char         _pad48[0x80 - 0x48];
    CScriptThing          Prisoner;                     // +0x80
    void CleanUp();
};
void CQ_HangingTreeEvilScript::CleanUp()
{
    CScriptThing guard1 = Game->GetThingWithScriptName("GUARD1");
    CScriptThing guard2 = Game->GetThingWithScriptName("GUARD2");
    CScriptThing prisoner = Game->GetThingWithScriptName("HTE_Prisoner");
    std::vector<CScriptThing> freedomFighters;
    Game->GetAllThingsWithScriptName("HTE_FreedomFighter", freedomFighters);
    for (unsigned int i = 0; i < freedomFighters.size(); ++i) {
        if (IsActiveThreadTerminating())
            return;
        Game->RemoveThing(freedomFighters[i], false, true);
    }
    if (IsActiveThreadTerminating())
        return;
    if (Prisoner.IsAlive())
        Game->RemoveThing(Prisoner, false, true);
    if (guard1.IsAlive())
        Game->RemoveThing(guard1, false, true);
    if (guard2.IsAlive())
        Game->RemoveThing(guard2, false, true);
}
}