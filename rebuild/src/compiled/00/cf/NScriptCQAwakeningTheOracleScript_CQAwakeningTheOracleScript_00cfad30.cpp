// NScript::CQ_AwakeningTheOracleScript::CQ_AwakeningTheOracleScript(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- constructor of the Q_AwakeningTheOracle quest script: level-script base, then the empty DiggingPoints vector and the cs_args / cs_chars maps
// Leaf: ??0CQ_AwakeningTheOracleScript@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CFAD30, 158 bytes, `__fastcall`, two stack arguments (`ret 8`), returns this.
//   Its only caller is the quest's Alloc @ 0x00D00590 (`new` 0x98 bytes, then this ctor out of line), the pAllocFunc of
//   the "Q_AwakeningTheOracle" CScriptInfo record (section "S_QATO", stored at 0x00CD596E).
// Label correction: the catalog "CHeroMorphDef_Create" is propagated from the CScriptBase ctor's own wrong label.
// Callees: CScriptBase::CScriptBase() @ 0x00CB8110, malloc @ 0x00BFEA0E (the two maps' header nodes: 0x18 for
//   map<CCharString, CCharString>, 0x24 for map<CCharString, CScriptGameResourceObjectScriptedThingBase>). Vtable 0x012C78B0.
// Layout: Ego_r.pdb NScript::CQ_AwakeningTheOracleScript (184 bytes) with the retail 0x40-byte script base (PDB offsets
//   minus 0x14), 12-byte STLport vector/maps (PDB 16): DiggingPoints +0x74, cs_args +0x80, cs_chars +0x8C, sizeof 0x98.
// Shape: speed TU (no pragma). CLevelScriptBase<T> is a novtable intermediate base (Game +0x40, data +0x44 stored before
//   the vtable). The vector is STLport `vector() : _Vector_base(allocator_type())`, each map the STLport
//   `_Rb_tree(_Compare(), allocator_type())` (header proxy zeroed, allocated, count zeroed, red colour, no parent,
//   left/right at the header, re-read per store); one shared zero register (ebx); both malloc pushes cleaned by one
//   `add esp,8`. The plain-data members stay uninitialised.
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E

class CGameScriptInterfaceBase;
class CScriptDataBase;
class CScriptThing;
class CCharString {                                         // 4 bytes
public:
    void* PStringData;
};
class CScriptGameResourceObjectScriptedThingBase {          // 0x10 bytes, vtable 0x0127094C
public:
    virtual ~CScriptGameResourceObjectScriptedThingBase();
    void* PIntelligentPointer;
    void* PImplementation;
    void* PImplementationInfo;
};

namespace std {
template <class T> class allocator {
public:
    allocator() { }
    template <class U> allocator(const allocator<U>&) { }
    T* allocate(unsigned int n) { return (T*)malloc(n * sizeof(T)); }
};
template <class T> class less {
public:
    bool operator()(const T& a, const T& b) const;
};
template <class V, class Tp, class A> class _STLP_alloc_proxy : public A {
public:
    V _M_data;
    _STLP_alloc_proxy(const A& a, V p) : A(a), _M_data(p) { }
};
struct _Rb_tree_node_base {
    bool                _M_color;                           // +0x00
    _Rb_tree_node_base* _M_parent;                          // +0x04
    _Rb_tree_node_base* _M_left;                            // +0x08
    _Rb_tree_node_base* _M_right;                           // +0x0C
};
template <class V> struct _Rb_tree_node : public _Rb_tree_node_base {
    V _M_value_field;                                       // +0x10
};
template <class V, class A> class _Rb_tree_base {
public:
    typedef _Rb_tree_node<V> _Node;
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_header;
    _Rb_tree_base(const A& a) : _M_header(allocator<_Node>(a), (_Node*)0)
    {
        _M_header._M_data = _M_header.allocate(1);
    }
};
template <class K, class V, class KeyOfValue, class Compare, class A> class _Rb_tree : public _Rb_tree_base<V, A> {
public:
    unsigned int _M_node_count;
    Compare      _M_key_compare;
    _Rb_tree(const Compare& comp, const A& a) : _Rb_tree_base<V, A>(a), _M_node_count(0), _M_key_compare(comp)
    {
        _M_empty_initialize();
    }
    private:
    void _M_empty_initialize()
    {
        this->_M_header._M_data->_M_color = false;          // red
        this->_M_header._M_data->_M_parent = 0;
        this->_M_header._M_data->_M_left = this->_M_header._M_data;
        this->_M_header._M_data->_M_right = this->_M_header._M_data;
    }
    public:
};
template <class T1, class T2> class pair {
public:
    T1 first;
    T2 second;
};
template <class P> struct _Select1st { };
template <class K, class T, class C = less<K>, class A = allocator<pair<const K, T> > > class map {
public:
    typedef pair<const K, T> value_type;
    typedef _Rb_tree<K, value_type, _Select1st<value_type>, C, A> _Rep_type;
    _Rep_type _M_t;
    map() : _M_t(C(), A()) { }
};
template <class T> class _Vector_base {
public:
    T*                                       _M_start;          // +0
    T*                                       _M_finish;         // +4
    _STLP_alloc_proxy<T*, T, allocator<T> >  _M_end_of_storage; // +8
    _Vector_base(const allocator<T>& a) : _M_start(0), _M_finish(0), _M_end_of_storage(a, 0) { }
};
template <class T> class vector : public _Vector_base<T> {
public:
    vector() : _Vector_base<T>(allocator<T>()) { }
};
} // namespace std

class CScriptBase { public:                                        // 0x40 bytes, the quest-script root
    virtual ~CScriptBase();
    unsigned char _body[0x3c];                              // +0x04..+0x3f
    CScriptBase();                                          // 0x00CB8110
};
template <class TData>
struct __declspec(novtable) CLevelScriptBase : CScriptBase {
    CGameScriptInterfaceBase* Game;                         // +0x40
    TData*                    MasterScriptData;             // +0x44
    CLevelScriptBase(CScriptDataBase* data, CGameScriptInterfaceBase& game) : CScriptBase(), Game(&game), MasterScriptData((TData*)data) { }
};

namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_AwakeningTheOracleScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    bool MissionSucceeded;                                  // +0x48
    bool PlayedOpeningCS;
    bool PlayedLostBayCS;
    bool PlayedNecroCS;
    long TabletsCollected;                                  // +0x4c
    bool SpawnedFinalSpotsStuff;                            // +0x50
    long PositionTaken;                                     // +0x54
    bool GivenDiggingSpotsRewards;                          // +0x58
    long GraveNumbersGivenOut;                              // +0x5c
    long FinalSummonersAlive;                               // +0x60
    bool DisplayedBadTabletMessage;                         // +0x64
    bool PlayedOpeningNecroCS;
    long TotalSpawnedCreaturesAlive;                        // +0x68
    bool AutosavedInNW3;                                    // +0x6c
    bool GotY;
    bool GotM;
    bool GotC;
    bool GotA;                                              // +0x70
    std::vector<CScriptThing> DiggingPoints;               // +0x74
    std::map<CCharString, CCharString> cs_args;            // +0x80
    std::map<CCharString, CScriptGameResourceObjectScriptedThingBase> cs_chars;   // +0x8c
    CQ_AwakeningTheOracleScript(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    virtual ~CQ_AwakeningTheOracleScript();
};
CQ_AwakeningTheOracleScript::CQ_AwakeningTheOracleScript(CScriptDataBase* data, CGameScriptInterfaceBase& game)
    : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game)
{
}
}