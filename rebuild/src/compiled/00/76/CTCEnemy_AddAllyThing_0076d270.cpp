// CTCEnemy::AddAllyThing(const CThing& thing) -- push a new intelligent pointer to the thing onto the AllyThings list (+0x14)
// Leaf: ?AddAllyThing@CTCEnemy@@QAEXABVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0076d270, 122 bytes, thiscall, `ret 4`.
// Identity (proven): Ego_r.pdb ?AddAllyThing@CTCEnemy@@QAEXABVCThing@@@Z and CTCEnemy::AllyThings (list<CIntelligentPointer<const CThing> >, retail +0x14 after the flags and EnemyThings); the temporary's vtable 0x01237F1C is CIntelligentPointer<const CThing>; callers CGameScriptInterface::ImpSetEntityAsAllyOfThing 0x0089C100, CTCSummonSpell::CreateMinion 0x0079ACC0, CTCGhostSword::AddSword 0x007C57C0; bound only as the stand-in ?AddAlly@CTCAllies_Retail@@....
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer 0x00A01B10, SetPItem 0x00A01B90, GetPItem 0x00A01B50,
//   ~CBaseIntelligentPointer 0x00A01C10, malloc 0x00BFEA0E.
// Shape (family L039): `AllyThings.push_back(CIntelligentPointer<const CThing>(&thing))` -- the temporary (vtable 0x01237F1C) in the frame,
//   STLport _M_create_node (malloc 16, placement copy-construct with the null test) and the four-store link before the
//   sentinel, then the temporary's implicit destructor (the base one, out of line).
extern "C" void* __cdecl malloc(unsigned int size);        // 0x00BFEA0E
inline void* __cdecl operator new(unsigned int, void* where) { return where; }
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                            // Ego_r.pdb: vptr + PData, 8 bytes
public:
    CBaseIntelligentPointer();                            // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                   // 0x00A01C10
    CBaseObject* GetPItem() const;                        // 0x00A01B50
    void SetPItem(CBaseObject* item);                     // 0x00A01B90
    CBaseObjectPointer* PData;                            // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // vtable 0x01237F1C
public:
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.CBaseIntelligentPointer::GetPItem()); }
};
namespace std {
template <class _Tp> class allocator {
public:
    allocator() { }
    _Tp* allocate(unsigned int __n) { return (_Tp*)malloc(__n * sizeof(_Tp)); }
};
template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Tp _M_data;
};
struct _List_node_base {
    _List_node_base* _M_next;                             // +0x0
    _List_node_base* _M_prev;                             // +0x4
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;                                          // +0x8
};
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp, class _Alloc> class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    _STLP_alloc_proxy<_List_node_base*, allocator<_Node> > _M_node;   // +0x00: the sentinel node
};
template <class _Tp, class _Alloc = allocator<_Tp> > class list : public _List_base<_Tp, _Alloc> {
public:
    typedef _List_node<_Tp> _Node;
    protected:
    _Node* _M_create_node(const _Tp& __x)
    {
        _Node* __p = this->_M_node.allocate(1);
        _Construct(&__p->_M_data, __x);
        return __p;
    }
    public:
    void _M_insert(_List_node_base* __n, const _Tp& __x)
    {
        _Node* __tmp = _M_create_node(__x);
        _List_node_base* __p = __n->_M_prev;
        __tmp->_M_next = __n;
        __tmp->_M_prev = __p;
        __p->_M_next = __tmp;
        __n->_M_prev = __tmp;
    }
    void push_back(const _Tp& __x) { _M_insert(this->_M_node._M_data, __x); }
};
}
class CThing;
class CTCBase {                                            // Ego_r.pdb, 12 bytes
public:
    virtual ~CTCBase();
    CThing* ParentThing;                                  // +0x04
    bool ToBeRemoved;                                     // +0x08
    bool IsInGlobalUpdateTCs;                             // +0x09
};
class CBaseObject { };
class CThing : public CBaseObject { };
class CTCEnemy : public CTCBase {
public:
    bool FriendsWithEverythingFlag;                       // +0x0C
    bool EnableFollowersEnemyProxy;                       // +0x0D
    std::list<CIntelligentPointer<const CThing> > EnemyThings;   // +0x10
    std::list<CIntelligentPointer<const CThing> > AllyThings;    // +0x14
    void AddAllyThing(const CThing& thing);
};
void CTCEnemy::AddAllyThing(const CThing& thing)
{
    AllyThings.push_back(CIntelligentPointer<const CThing>(&thing));
}