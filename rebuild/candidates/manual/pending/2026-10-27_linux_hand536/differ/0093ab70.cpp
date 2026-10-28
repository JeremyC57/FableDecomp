// CAIStateGroup_BanditSitRoundCampfire::GetPNearCampfire() const -- the nearest live free bonfire (class type 5) among a copy of the creature's village campfires; failing that, the nearest campfire with space within 22 units (GetPBestThing)
// Grade: DIFFER(511v504) -- residue: register roles in the campfire loop (retail: this in ebx, the copied list head in edi, the iterator and best in the frame, the fire in ebp; ours: this in ebp, iterator node in ebx, fire in edi) and frame 0x2c v 0x30; the HasTC byte spill reused as the __false_type tag now matches | tried: 2026-10-21 value-initialised / named tag, iterator objects; 2026-10-28 real-name rewrite (PeekTC<const CTCVillageMember> with the out-of-line const LowerBound 0x004365B0 and the key temporary, CTCVillageMember::GetPVillage, STLport list<CIntelligentPointer<CThing> > copy through the landed 0x00618100 model with the `__tmp` head, CClass::GetType, CIsFreeBonfire with an empty user ctor), a functor local, an iterator declared before the loop, a creature-position reference | next: STLport's real list copy ctor shape (`_List_base(const allocator&)` + `insert(begin(), x.begin(), x.end())` through the range-insert wrapper) so the head stays in edi across the dispatch call
// Leaf: ?GetPNearCampfire@CAIStateGroup_BanditSitRoundCampfire@@ABEPAVCThingObject@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0093AB70, 504 bytes, thiscall, plain `ret`.
// Name: Ego_r.pdb `?GetPNearCampfire@CAIStateGroup_BanditSitRoundCampfire@@ABEPAVCThingObject@@XZ` (private const).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound(const&) const 0x004365B0 (landed leaf; inline PeekTC 0x23),
//   CTCVillageMember::GetPVillage() const 0x007EE540, malloc 0x00BFEA0E / free 0x00BFEA14 (STLport node allocator),
//   std::list<CIntelligentPointer<CThing> >::_M_insert_dispatch<_List_const_iterator> 0x00618100 (landed) and the list clear
//   0x004B8050, CBaseIntelligentPointer::GetPItem 0x00A01B50, CIsFreeBonfire::operator()(const CThingObject&) const 0x0093B5E0,
//   CThing::GetPos 0x004C73D0, CAIStateGroupBase::PeekThingSearchTools 0x008FD000, CThingSearchTools::GetPBestThing<
//   CIsNearCampfireWithSpace, CThingCompare_Nearest, CThingObject> 0x0093B640 (Ego_r spellings).
// Layout: CTCVillage::VillageCampfires +0x44 (Ego_r +0x7C; STLport lists are 4 bytes), read through FableWin's inline
//   PeekVillageCampfires() const and copied; CClass::Type +0x10 (CThing : CClass); retail CThing bitset +0x20, Components +0x44.
extern "C" void* __cdecl malloc(unsigned int size);       // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                 // 0x00BFEA14
inline void* __cdecl operator new(unsigned int, void* where) throw() { return where; }

class C3DVector {
public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    float GetLengthSquared() const { return Y * Y + X * X + Z * Z; }
};
class CTCBase;
enum ETCInterfaceType { TC_VILLAGE_MEMBER = 0x23 };
class CBaseObject;
class CBaseIntelligentPointer {
public:
    virtual ~CBaseIntelligentPointer();
    CBaseIntelligentPointer();                             // 0x00A01B10
    CBaseObject* GetPItem() const;                         // 0x00A01B50
    void SetPItem(CBaseObject* item);                      // 0x00A01B90
    void* PData;
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    virtual ~CIntelligentPointer();
    CIntelligentPointer(const CIntelligentPointer<T>& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
    T* Get() const { return (T*)GetPItem(); }
    T* operator->() const { return Get(); }
};
namespace std {
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
template <class _Tp> class allocator0 { };
template <class _Tp, class _Alloc = allocator0<_Tp> > class vector {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
};
struct __false_type { __false_type() { } };
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class T> struct _List_node : _List_node_base {
    T _M_data;
};
struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* x) : _M_node(x) { }
};
struct _List_iterator : _List_iterator_base {
    _List_iterator(_List_node_base* x) : _List_iterator_base(x) { }
    _List_iterator(const _List_iterator& x) : _List_iterator_base(x._M_node) { }
};
struct _List_const_iterator : _List_iterator_base {
    _List_const_iterator(_List_node_base* x) : _List_iterator_base(x) { }
    _List_const_iterator(const _List_iterator& x) : _List_iterator_base(x._M_node) { }
    bool operator!=(const _List_const_iterator& y) const { return _M_node != y._M_node; }
    _List_const_iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
};
template <class T> class allocator {
public:
    allocator() { }
    T* allocate(unsigned int n) { return n != 0 ? (T*)malloc(n * sizeof(T)) : 0; }
    void deallocate(T* p, unsigned int) { free(p); }
};
template <class T> struct _STLP_alloc_proxy : allocator<T> {
    T* _M_data;
    _STLP_alloc_proxy() { }
};
template <class T> class _List_base {
public:
    _STLP_alloc_proxy<_List_node<T> > _M_node;
    _List_base()
    {
        _List_node<T>* __tmp = _M_node.allocate(1);
        __tmp->_M_next = __tmp;
        __tmp->_M_prev = __tmp;
        _M_node._M_data = __tmp;
    }
    ~_List_base() { clear(); _M_node.deallocate(_M_node._M_data, 1); }
    void clear();                                          // 0x004B8050
};
template <class T> class list : public _List_base<T> {
public:
    typedef _List_node<T> _Node;
    typedef _List_iterator iterator;
    typedef _List_const_iterator const_iterator;
    list() { }
    list(const list<T>& x) { _M_insert_dispatch(begin(), x.begin(), x.end(), __false_type()); }
    iterator begin() { return _List_iterator(this->_M_node._M_data->_M_next); }
    const_iterator begin() const { return _List_const_iterator(this->_M_node._M_data->_M_next); }
    const_iterator end() const { return _List_const_iterator(this->_M_node._M_data); }
    template <class _InputIter>
    void _M_insert_dispatch(_List_iterator pos, _InputIter first, _InputIter last, const __false_type&);   // 0x00618100
};
}
template <class K, class V> class CKeyPairCompareLess { };
template <class K, class V, class C = CKeyPairCompareLess<K, V> >
class CVectorMap : public std::vector<std::pair<K, V> > {
public:
    typedef std::pair<K, V> value_type;
    typedef const value_type* const_iterator;
    const_iterator LowerBound(const K& key) const;         // 0x004365B0
    const_iterator end() const { return this->_M_finish; }
};

class CClass {
public:
    virtual ~CClass();
    unsigned char _pad04[0x10 - 0x04];
    unsigned char Type;                                    // +0x10
    unsigned long GetType() const { return Type; }
};
class CThing : public CClass {
public:
    unsigned char _pad_0x14[0x20 - 0x14];
    unsigned long UsingComponents[9];                      // +0x20
    CVectorMap<ETCInterfaceType, CTCBase*> Components;     // +0x44
    unsigned char _pad_0x50[0x91 - 0x50];
    bool Dead : 1;                                         // +0x91 bit 0
    bool IsDead() const { return Dead != 0; }
    const C3DVector& GetPos() const;                       // 0x004C73D0
    bool HasTC(ETCInterfaceType type) const { return ((UsingComponents[type >> 5] >> (type & 31)) & 1) != 0; }
    template <class T> __forceinline bool PeekTC(const T** out) const
    {
        bool present = HasTC((ETCInterfaceType)T::InterfaceType);
        if (present) {
            const CVectorMap<ETCInterfaceType, CTCBase*>& components = Components;
            CVectorMap<ETCInterfaceType, CTCBase*>::const_iterator p = components.LowerBound((ETCInterfaceType)T::InterfaceType);
            if (p == components.end() || (ETCInterfaceType)T::InterfaceType < p->first)
                p = components.end();
            *out = (const T*)p->second;
        }
        return present;
    }
};
class CThingObject : public CThing { };
class CThingAICreature : public CThing { };
template <class T> inline bool IS_THING_ALIVE(const T& thing)
{
    if (thing.GetPItem() != 0) {
        if (!thing->IsDead())
            return true;
    }
    return false;
}

class CTCVillage {
public:
    unsigned char _pad00[0x44];
    std::list<CIntelligentPointer<CThing> > VillageCampfires;   // +0x44
    const std::list<CIntelligentPointer<CThing> >& PeekVillageCampfires() const { return VillageCampfires; }
};
class CTCVillageMember {
public:
    enum { InterfaceType = TC_VILLAGE_MEMBER };
    CTCVillage* GetPVillage() const;                       // 0x007EE540
};
class CIsFreeBonfire {
public:
    CIsFreeBonfire() { }
    bool operator()(const CThingObject& thing) const;      // 0x0093B5E0
};
class CIsNearCampfireWithSpace {
public:
    float     DistanceSq;
    C3DVector FromPos;
    CIsNearCampfireWithSpace(float distance_sq, const C3DVector& from) : DistanceSq(distance_sq), FromPos(from) { }
};
class CThingCompare_Nearest {
public:
    C3DVector Pos;
    CThingCompare_Nearest(const C3DVector& pos) : Pos(pos) { }
};
class CThingSearchTools {
public:
    template <class F, class C, class T> bool GetPBestThing(const F& filter, const C& compare, T** best) const;   // 0x0093B640
};
class CAIBrain {
public:
    unsigned char     _pad00[0x18];
    CThingAICreature* Creature;                            // +0x18
    CThingAICreature& GetCreature() const { return *Creature; }
};
class CAIStateGroupBase {
public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                                      // +0x04
protected:
    CThingAICreature& GetCreature() const { return PBrain->GetCreature(); }
    const CThingSearchTools& PeekThingSearchTools() const; // 0x008FD000
};
class CAIStateGroup_BanditSitRoundCampfire : public CAIStateGroupBase {
private:
    CThingObject* GetPNearCampfire() const;
};

CThingObject* CAIStateGroup_BanditSitRoundCampfire::GetPNearCampfire() const
{
    CThingObject* best = 0;
    const CTCVillageMember* member;
    GetCreature().PeekTC(&member);
    CTCVillage* village = member->GetPVillage();
    if (village) {
        std::list<CIntelligentPointer<CThing> > campfires(village->PeekVillageCampfires());
        float best_distance = 0.0f;
        CIsFreeBonfire is_free;
        for (std::list<CIntelligentPointer<CThing> >::const_iterator it = campfires.begin(); it != campfires.end(); ++it) {
            const CIntelligentPointer<CThing>& fire = ((std::_List_node<CIntelligentPointer<CThing> >*)it._M_node)->_M_data;
            if (IS_THING_ALIVE(fire) && fire->GetType() == 5) {
                CThingObject* object = (CThingObject*)fire.Get();
                if (is_free(*object)) {
                    float distance = (object->GetPos() - GetCreature().GetPos()).GetLengthSquared();
                    if (!best || distance < best_distance) {
                        best_distance = distance;
                        best = object;
                    }
                }
            }
        }
        if (best)
            return best;
    }
    PeekThingSearchTools().GetPBestThing(CIsNearCampfireWithSpace(484.0f, GetCreature().GetPos()),
                                         CThingCompare_Nearest(GetCreature().GetPos()), &best);
    return best;
}
