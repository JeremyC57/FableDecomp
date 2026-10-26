// CWorld::TeleportHeroToHSP(const CCharString& name) -- teleport the hero to the holy site whose ScriptName is name (false when no such site exists)
// Leaf: ?TeleportHeroToHSP@CWorld@@
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x004A0940, 416 bytes, `ret 4` (Ego_r.pdb ?TeleportHeroToHSP@CWorld@@QAE_NABVCCharString@@@Z).
//   Caller: the test-quest activation 0x0061ABFC (CTCBase::GetWorld().TeleportHeroToHSP(quest name)).
// Callees: CWorld::PeekThingManager (virtual slot 4, +0x10; 0x0049E1C0), the CWorld-TU copy of the holy-site copy
//   template 0x004ADCB0 (/Gr, hidden result in ecx; the same body as CPlayer's 0x0048D5C0), CCharString copy 0x0099EC30 /
//   dtor 0x0099EAE0, STLport __find_if 0x0048CC60 with bind2nd(ptr_fun(0x0048BC70), name), CThing::GetScriptThing 0x004C7E50,
//   CPlayerManager::GetMainPlayer 0x00449970 (g_pMainGameComponent 0x013B86A0 +0x1C), CPlayer::GetPlayerCharacter
//   0x00487B70, CGameScriptInterface::EntityTeleportToThing (slot 472, +0x760; CWorld +0x38), ~CBaseClass 0x0099A2E0,
//   operator delete 0x00BFE9BC, free 0x00BFEA14.
// Shape: the landed CPlayer::LocateCharacterStartHolySite 0x00488B20 model (owner local before the vector, the inline
//   find_if / bind2nd / ptr_fun layers) with the STLport 4.6 std classes of this chunk's __find_if row 0x0048CC60 (same
//   callee symbol) and a same-TU noinline __find_if body (the tag homes in the dead `name` slot); CScriptThing
//   temporaries with the inline destructor of the landed CQ_HangingTreeEvilScript::CleanUp 0x00D6B720. The script
//   interface and the player manager are named locals read before the two GetScriptThing calls (retail loads both, and
//   the interface's vtable, ahead of the argument evaluation).
extern "C" void __cdecl free(void* block);         // 0x00BFEA14
void __cdecl operator delete(void* p);                    // 0x00BFE9BC (MSVCR71)

class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);         // 0x0099EC30
    ~CCharString();                                // 0x0099EAE0
};

class CCPPointerInfo { public:
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T> class CCountedPointer { public:
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
class CBaseClass {
public:
    CBaseClass();                                  // 0x0099A2D0
    virtual ~CBaseClass();                         // 0x0099A2E0
};
class CScriptThing : public CBaseClass {           // 12 bytes, vtable 0x01238C8C
public:
    CCountedPointer<CScriptThing> PImp;            // +0x04 Data, +0x08 Info
    ~CScriptThing() {}                             // out of line 0x004AA840
};

class CThing {
public:
    CScriptThing GetScriptThing();                 // 0x004C7E50
};
class CPlayer {
public:
    CThing* GetPlayerCharacter();                  // 0x00487B70
};
class CPlayerManager {
public:
    CPlayer* GetMainPlayer();                      // 0x00449970
};
class CMainGameComponent {
public:
    unsigned char   _pad00[0x1c];
    CPlayerManager* PlayerManager;                 // +0x1C
};
extern CMainGameComponent* g_pMainGameComponent;   // 0x013B86A0

class CGameScriptInterface {
public:
    virtual void g000();
    virtual void g001();
    virtual void g002();
    virtual void g003();
    virtual void g004();
    virtual void g005();
    virtual void g006();
    virtual void g007();
    virtual void g008();
    virtual void g009();
    virtual void g010();
    virtual void g011();
    virtual void g012();
    virtual void g013();
    virtual void g014();
    virtual void g015();
    virtual void g016();
    virtual void g017();
    virtual void g018();
    virtual void g019();
    virtual void g020();
    virtual void g021();
    virtual void g022();
    virtual void g023();
    virtual void g024();
    virtual void g025();
    virtual void g026();
    virtual void g027();
    virtual void g028();
    virtual void g029();
    virtual void g030();
    virtual void g031();
    virtual void g032();
    virtual void g033();
    virtual void g034();
    virtual void g035();
    virtual void g036();
    virtual void g037();
    virtual void g038();
    virtual void g039();
    virtual void g040();
    virtual void g041();
    virtual void g042();
    virtual void g043();
    virtual void g044();
    virtual void g045();
    virtual void g046();
    virtual void g047();
    virtual void g048();
    virtual void g049();
    virtual void g050();
    virtual void g051();
    virtual void g052();
    virtual void g053();
    virtual void g054();
    virtual void g055();
    virtual void g056();
    virtual void g057();
    virtual void g058();
    virtual void g059();
    virtual void g060();
    virtual void g061();
    virtual void g062();
    virtual void g063();
    virtual void g064();
    virtual void g065();
    virtual void g066();
    virtual void g067();
    virtual void g068();
    virtual void g069();
    virtual void g070();
    virtual void g071();
    virtual void g072();
    virtual void g073();
    virtual void g074();
    virtual void g075();
    virtual void g076();
    virtual void g077();
    virtual void g078();
    virtual void g079();
    virtual void g080();
    virtual void g081();
    virtual void g082();
    virtual void g083();
    virtual void g084();
    virtual void g085();
    virtual void g086();
    virtual void g087();
    virtual void g088();
    virtual void g089();
    virtual void g090();
    virtual void g091();
    virtual void g092();
    virtual void g093();
    virtual void g094();
    virtual void g095();
    virtual void g096();
    virtual void g097();
    virtual void g098();
    virtual void g099();
    virtual void g100();
    virtual void g101();
    virtual void g102();
    virtual void g103();
    virtual void g104();
    virtual void g105();
    virtual void g106();
    virtual void g107();
    virtual void g108();
    virtual void g109();
    virtual void g110();
    virtual void g111();
    virtual void g112();
    virtual void g113();
    virtual void g114();
    virtual void g115();
    virtual void g116();
    virtual void g117();
    virtual void g118();
    virtual void g119();
    virtual void g120();
    virtual void g121();
    virtual void g122();
    virtual void g123();
    virtual void g124();
    virtual void g125();
    virtual void g126();
    virtual void g127();
    virtual void g128();
    virtual void g129();
    virtual void g130();
    virtual void g131();
    virtual void g132();
    virtual void g133();
    virtual void g134();
    virtual void g135();
    virtual void g136();
    virtual void g137();
    virtual void g138();
    virtual void g139();
    virtual void g140();
    virtual void g141();
    virtual void g142();
    virtual void g143();
    virtual void g144();
    virtual void g145();
    virtual void g146();
    virtual void g147();
    virtual void g148();
    virtual void g149();
    virtual void g150();
    virtual void g151();
    virtual void g152();
    virtual void g153();
    virtual void g154();
    virtual void g155();
    virtual void g156();
    virtual void g157();
    virtual void g158();
    virtual void g159();
    virtual void g160();
    virtual void g161();
    virtual void g162();
    virtual void g163();
    virtual void g164();
    virtual void g165();
    virtual void g166();
    virtual void g167();
    virtual void g168();
    virtual void g169();
    virtual void g170();
    virtual void g171();
    virtual void g172();
    virtual void g173();
    virtual void g174();
    virtual void g175();
    virtual void g176();
    virtual void g177();
    virtual void g178();
    virtual void g179();
    virtual void g180();
    virtual void g181();
    virtual void g182();
    virtual void g183();
    virtual void g184();
    virtual void g185();
    virtual void g186();
    virtual void g187();
    virtual void g188();
    virtual void g189();
    virtual void g190();
    virtual void g191();
    virtual void g192();
    virtual void g193();
    virtual void g194();
    virtual void g195();
    virtual void g196();
    virtual void g197();
    virtual void g198();
    virtual void g199();
    virtual void g200();
    virtual void g201();
    virtual void g202();
    virtual void g203();
    virtual void g204();
    virtual void g205();
    virtual void g206();
    virtual void g207();
    virtual void g208();
    virtual void g209();
    virtual void g210();
    virtual void g211();
    virtual void g212();
    virtual void g213();
    virtual void g214();
    virtual void g215();
    virtual void g216();
    virtual void g217();
    virtual void g218();
    virtual void g219();
    virtual void g220();
    virtual void g221();
    virtual void g222();
    virtual void g223();
    virtual void g224();
    virtual void g225();
    virtual void g226();
    virtual void g227();
    virtual void g228();
    virtual void g229();
    virtual void g230();
    virtual void g231();
    virtual void g232();
    virtual void g233();
    virtual void g234();
    virtual void g235();
    virtual void g236();
    virtual void g237();
    virtual void g238();
    virtual void g239();
    virtual void g240();
    virtual void g241();
    virtual void g242();
    virtual void g243();
    virtual void g244();
    virtual void g245();
    virtual void g246();
    virtual void g247();
    virtual void g248();
    virtual void g249();
    virtual void g250();
    virtual void g251();
    virtual void g252();
    virtual void g253();
    virtual void g254();
    virtual void g255();
    virtual void g256();
    virtual void g257();
    virtual void g258();
    virtual void g259();
    virtual void g260();
    virtual void g261();
    virtual void g262();
    virtual void g263();
    virtual void g264();
    virtual void g265();
    virtual void g266();
    virtual void g267();
    virtual void g268();
    virtual void g269();
    virtual void g270();
    virtual void g271();
    virtual void g272();
    virtual void g273();
    virtual void g274();
    virtual void g275();
    virtual void g276();
    virtual void g277();
    virtual void g278();
    virtual void g279();
    virtual void g280();
    virtual void g281();
    virtual void g282();
    virtual void g283();
    virtual void g284();
    virtual void g285();
    virtual void g286();
    virtual void g287();
    virtual void g288();
    virtual void g289();
    virtual void g290();
    virtual void g291();
    virtual void g292();
    virtual void g293();
    virtual void g294();
    virtual void g295();
    virtual void g296();
    virtual void g297();
    virtual void g298();
    virtual void g299();
    virtual void g300();
    virtual void g301();
    virtual void g302();
    virtual void g303();
    virtual void g304();
    virtual void g305();
    virtual void g306();
    virtual void g307();
    virtual void g308();
    virtual void g309();
    virtual void g310();
    virtual void g311();
    virtual void g312();
    virtual void g313();
    virtual void g314();
    virtual void g315();
    virtual void g316();
    virtual void g317();
    virtual void g318();
    virtual void g319();
    virtual void g320();
    virtual void g321();
    virtual void g322();
    virtual void g323();
    virtual void g324();
    virtual void g325();
    virtual void g326();
    virtual void g327();
    virtual void g328();
    virtual void g329();
    virtual void g330();
    virtual void g331();
    virtual void g332();
    virtual void g333();
    virtual void g334();
    virtual void g335();
    virtual void g336();
    virtual void g337();
    virtual void g338();
    virtual void g339();
    virtual void g340();
    virtual void g341();
    virtual void g342();
    virtual void g343();
    virtual void g344();
    virtual void g345();
    virtual void g346();
    virtual void g347();
    virtual void g348();
    virtual void g349();
    virtual void g350();
    virtual void g351();
    virtual void g352();
    virtual void g353();
    virtual void g354();
    virtual void g355();
    virtual void g356();
    virtual void g357();
    virtual void g358();
    virtual void g359();
    virtual void g360();
    virtual void g361();
    virtual void g362();
    virtual void g363();
    virtual void g364();
    virtual void g365();
    virtual void g366();
    virtual void g367();
    virtual void g368();
    virtual void g369();
    virtual void g370();
    virtual void g371();
    virtual void g372();
    virtual void g373();
    virtual void g374();
    virtual void g375();
    virtual void g376();
    virtual void g377();
    virtual void g378();
    virtual void g379();
    virtual void g380();
    virtual void g381();
    virtual void g382();
    virtual void g383();
    virtual void g384();
    virtual void g385();
    virtual void g386();
    virtual void g387();
    virtual void g388();
    virtual void g389();
    virtual void g390();
    virtual void g391();
    virtual void g392();
    virtual void g393();
    virtual void g394();
    virtual void g395();
    virtual void g396();
    virtual void g397();
    virtual void g398();
    virtual void g399();
    virtual void g400();
    virtual void g401();
    virtual void g402();
    virtual void g403();
    virtual void g404();
    virtual void g405();
    virtual void g406();
    virtual void g407();
    virtual void g408();
    virtual void g409();
    virtual void g410();
    virtual void g411();
    virtual void g412();
    virtual void g413();
    virtual void g414();
    virtual void g415();
    virtual void g416();
    virtual void g417();
    virtual void g418();
    virtual void g419();
    virtual void g420();
    virtual void g421();
    virtual void g422();
    virtual void g423();
    virtual void g424();
    virtual void g425();
    virtual void g426();
    virtual void g427();
    virtual void g428();
    virtual void g429();
    virtual void g430();
    virtual void g431();
    virtual void g432();
    virtual void g433();
    virtual void g434();
    virtual void g435();
    virtual void g436();
    virtual void g437();
    virtual void g438();
    virtual void g439();
    virtual void g440();
    virtual void g441();
    virtual void g442();
    virtual void g443();
    virtual void g444();
    virtual void g445();
    virtual void g446();
    virtual void g447();
    virtual void g448();
    virtual void g449();
    virtual void g450();
    virtual void g451();
    virtual void g452();
    virtual void g453();
    virtual void g454();
    virtual void g455();
    virtual void g456();
    virtual void g457();
    virtual void g458();
    virtual void g459();
    virtual void g460();
    virtual void g461();
    virtual void g462();
    virtual void g463();
    virtual void g464();
    virtual void g465();
    virtual void g466();
    virtual void g467();
    virtual void g468();
    virtual void g469();
    virtual void g470();
    virtual void g471();
    virtual void EntityTeleportToThing(const CScriptThing& thing, const CScriptThing& target, bool a) const;   // slot 472 (+0x760)
};

template <class T>
struct CRetailVector {                             // STLport vector<T>: start, finish, end of storage
    T* m_pStart;
    T* m_pFinish;
    T* m_pEndOfStorage;
    CRetailVector() : m_pStart(0), m_pFinish(0), m_pEndOfStorage(0) { }
    ~CRetailVector()
    {
        if (m_pStart)
            free(m_pStart);
    }
    T* begin() const { return m_pStart; }
    T* end() const { return m_pFinish; }
};
template <class C> struct back_insert_iterator {
    C* container;
    explicit back_insert_iterator(C& c) : container(&c) { }
};
template <class C> inline back_insert_iterator<C> back_inserter(C& c) { return back_insert_iterator<C>(c); }

struct SThingNode { SThingNode* _M_next; SThingNode* _M_prev; CThing* _M_data; };
struct SThingListIterator {
    SThingNode* _M_node;
    SThingListIterator(SThingNode* node) : _M_node(node) { }
};
struct CThingList {
    SThingNode* _M_node;
    SThingListIterator begin() const { return SThingListIterator(_M_node->_M_next); }
    SThingListIterator end() const { return SThingListIterator(_M_node); }
};
struct CThingListOwner {
    unsigned char _pad00[0x8c];
    CThingList    HolySites;                       // +0x8c
};
class CThingManager {
public:
    unsigned char    _pad00[4];
    CThingListOwner* Owner;                        // +0x04
};
// 0x004ADCB0: copies the list's things (those whose +0xD4 flag is set) to the output iterator (/Gr template)
template <class It, class Out> Out __fastcall CopyHolySites(It first, It last, Out out);

namespace std {
struct input_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { };
struct bidirectional_iterator_tag : public forward_iterator_tag { };
struct random_access_iterator_tag : public bidirectional_iterator_tag { random_access_iterator_tag() { } };

template <class _Arg1, class _Arg2, class _Result>
struct binary_function {
    typedef _Arg1 first_argument_type;
    typedef _Arg2 second_argument_type;
    typedef _Result result_type;
};
template <class _Arg1, class _Arg2, class _Result>
class pointer_to_binary_function : public binary_function<_Arg1, _Arg2, _Result> {
protected:
    _Result (*_M_ptr)(_Arg1, _Arg2);
public:
    pointer_to_binary_function() { }
    explicit pointer_to_binary_function(_Result (*__x)(_Arg1, _Arg2)) : _M_ptr(__x) { }
    _Result operator()(_Arg1 __x, _Arg2 __y) const { return _M_ptr(__x, __y); }
};
template <class _Arg1, class _Arg2, class _Result>
inline pointer_to_binary_function<_Arg1, _Arg2, _Result> ptr_fun(_Result (*__x)(_Arg1, _Arg2))
{
    return pointer_to_binary_function<_Arg1, _Arg2, _Result>(__x);
}
template <class _Operation>
class binder2nd {
protected:
    _Operation op;
    typename _Operation::second_argument_type value;
public:
    binder2nd(const _Operation& __x, const typename _Operation::second_argument_type& __y) : op(__x), value(__y) { }
    typename _Operation::result_type operator()(typename _Operation::first_argument_type __x) const
    {
        return op(__x, value);
    }
};
template <class _Operation, class _Tp>
inline binder2nd<_Operation> bind2nd(const _Operation& __fn, const _Tp& __x)
{
    typedef typename _Operation::second_argument_type _Arg2_type;
    return binder2nd<_Operation>(__fn, _Arg2_type(__x));
}

#pragma auto_inline(off)
// 0x0048CC60 (the chunk's own __find_if row, same symbol): a same-TU noinline body so VC7.1 homes the iterator tag
// in the dead `name` argument slot, as retail.
template <class _RandomAccessIter, class _Predicate>
__declspec(noinline) _RandomAccessIter __find_if(_RandomAccessIter __first, _RandomAccessIter __last, _Predicate __pred,
                                                 const random_access_iterator_tag&)
{
    int __trip_count = (__last - __first) >> 2;
    for ( ; __trip_count > 0 ; --__trip_count) {
        if (__pred(*__first)) return __first;
        ++__first;
        if (__pred(*__first)) return __first;
        ++__first;
        if (__pred(*__first)) return __first;
        ++__first;
        if (__pred(*__first)) return __first;
        ++__first;
    }
    switch (__last - __first) {
    case 3:
        if (__pred(*__first)) return __first;
        ++__first;
    case 2:
        if (__pred(*__first)) return __first;
        ++__first;
    case 1:
        if (__pred(*__first)) return __first;
        ++__first;
    case 0:
    default:
        return __last;
    }
}
#pragma auto_inline(on)
template <class _Tp> inline random_access_iterator_tag __ITERATOR_CATEGORY(_Tp* const&) { return random_access_iterator_tag(); }
template <class _InputIter, class _Predicate>
inline _InputIter find_if(_InputIter __first, _InputIter __last, _Predicate __pred)
{
    return __find_if(__first, __last, __pred, __ITERATOR_CATEGORY(__first));
}
}

bool IsThingScriptNamed(CThing* thing, CCharString name);   // 0x0048BC70 (/Gr: __fastcall)

class CWorld {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual const CThingManager& PeekThingManager() const;   // slot 4 (+0x10) 0x0049E1C0
    unsigned char         _pad04[0x38 - 4];
    CGameScriptInterface* PGameScriptInterface_ptr;          // +0x38
    bool TeleportHeroToHSP(const CCharString& name);
};

bool CWorld::TeleportHeroToHSP(const CCharString& name)
{
    CThingListOwner* owner = PeekThingManager().Owner;
    CRetailVector<CThing*> sites;
    CopyHolySites(owner->HolySites.begin(), owner->HolySites.end(), back_inserter(sites));
    CThing** found = std::find_if(sites.begin(), sites.end(), std::bind2nd(std::ptr_fun(&IsThingScriptNamed), name));
    if (found != sites.end()) {
        CGameScriptInterface* gsi = PGameScriptInterface_ptr;
        CPlayerManager* players = g_pMainGameComponent->PlayerManager;
        gsi->EntityTeleportToThing(players->GetMainPlayer()->GetPlayerCharacter()->GetScriptThing(),
                                   (*found)->GetScriptThing(), false);
        return true;
    }
    return false;
}