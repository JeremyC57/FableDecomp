// CGameCameraManager::ResetFlags() -- clear the "used" flag of the scripted camera spline (TC 0x3F) on every live
// Grade: DIFFER(ours v retail) -- 183v183, the clear() tail only: our build reloads the list header into ebp (this is dead) for the prev store, retail reloads into eax with pop edi/pop esi scheduled between
// available camera point, then empty the camera point list
// Leaf: ResetFlags
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00699130, length 192 bytes, `ret`.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, the TC map lower bound @ 0x0040F020, the element's virtual
// destructor (slot 0, called with 0), free @ 0x00BFEA14 (STLport list nodes).
// Retail layout (engine/CGameCameraManager.h has PDB offsets, AvailableCamPoints +0xB0 there): AvailableCamPoints
// +0xAC = the STLport list's header-node pointer (node: next, prev, CIntelligentPointer<CThing> data at +8);
// CThing TC bitset +0x20 (TC 0x3F = +0x24 bit 31), TC map +0x44, dead bit +0x91; CTCCameraPointScriptedSpline flag
// byte +0x3C.
// Shape: a list iterator loop re-reading end(); IS_THING_ALIVE through two GetPItem calls and a third for the
// out-parameter GetInterface; the inline STLport clear() advances the cursor before destroying and freeing the node
// and re-reads the header pointer after every free.

extern "C" void __cdecl free(void* p);             // 0x00BFEA14

struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];       // +0x20 std::bitset of the interface types present
    struct CInterfaceMap_Retail {
        struct SEntry { long type; void* component; };
        SEntry* first;
        SEntry* last;
        SEntry* LowerBound(const long* type);       // 0x0040F020
    } m_interfaces;                       // +0x44
    unsigned char _pad4c[0x45];
    unsigned char m_dead : 1;             // +0x91 bit 0
    unsigned char m_91_1 : 7;
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap_Retail* map = &m_interfaces;
            CInterfaceMap_Retail::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};

struct CBaseObject;
struct CBaseIntelligentPointer {
    virtual ~CBaseIntelligentPointer();
    void* Info;
    CBaseObject* GetPItem() const;                  // 0x00A01B50
};
template<class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};

struct CTCCameraPointScriptedSpline {
    unsigned char _pad[0x3c];
    bool Used;                            // +0x3C
};

struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template<class T> struct _List_node : _List_node_base {
    T _M_data;
};
template<class T> inline void _Destroy(T* p) { p->~T(); }
template<class T> struct _List_iterator {
    _List_node_base* _M_node;
    _List_iterator(_List_node_base* n) : _M_node(n) {}
    T& operator*() const { return ((_List_node<T>*)_M_node)->_M_data; }
    T* operator->() const { return &((_List_node<T>*)_M_node)->_M_data; }
    _List_iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
    bool operator!=(const _List_iterator& o) const { return _M_node != o._M_node; }
};
template<class T> struct _STLP_alloc_proxy { T _M_data; };
template<class T> struct CRetailList {
    _STLP_alloc_proxy<_List_node_base*> _M_node;
    _List_iterator<T> begin() { return _List_iterator<T>(_M_node._M_data->_M_next); }
    _List_iterator<T> end() { return _List_iterator<T>(_M_node._M_data); }
    void clear()
    {
        _List_node<T>* __cur = (_List_node<T>*)this->_M_node._M_data->_M_next;
        while (__cur != this->_M_node._M_data) {
            _List_node<T>* __tmp = __cur;
            __cur = (_List_node<T>*)__cur->_M_next;
            _Destroy(&__tmp->_M_data);
            free(__tmp);
        }
        this->_M_node._M_data->_M_next = this->_M_node._M_data;
        this->_M_node._M_data->_M_prev = this->_M_node._M_data;
    }
};

class CGameCameraManager {
public:
    virtual void v00();
    unsigned char _pad004[0xa8];
    CRetailList<CIntelligentPointer<CThing> > AvailableCamPoints;   // +0xAC
    void ResetFlags();
};

void CGameCameraManager::ResetFlags()
{
    for (_List_iterator<CIntelligentPointer<CThing> > it = AvailableCamPoints.begin(); it != AvailableCamPoints.end(); ++it) {
        if (it->GetPItem() && !it->GetPItem()->m_dead) {
            CTCCameraPointScriptedSpline* spline;
            if (it->GetPItem()->GetInterface(&spline, 0x3f))
                spline->Used = false;
        }
    }
    AvailableCamPoints.clear();
}
