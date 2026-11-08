// CTCInventoryMap::FindTeleports() -- rebuilds the list of regions that hold a teleport: for every region (1..count), every map linked to the region is searched for teleporter things (TC 0xDD) and the region index is recorded once when an enabled teleporter is found
// Leaf: ?FindTeleports@CTCInventoryMap@@AAEXXZ
// Naming wave 27 (family acc, DECISIONS_W26 D5): FindTeleports is private in both PDBs (AAE). Was public (QAE).
// Naming wave 26 (family vlong): the vector is STLport 4.6 std::vector<long, allocator<long> >; its grow-insert 0x00440990 is the
//   protected ?_M_insert_overflow@?$vector@JV?$allocator@J@std@@@std@@IAEXPAJABJABU__true_type@2@I_N@Z (size_type unsigned int; the
//   own row's spelling, c689 queue). Was CLongVector::_M_insert_overflow(..., const __false_type&, unsigned long, bool).
// Retail: Fable.exe (TLC, VC7.1) @ 0x005FC620, 528 bytes, plain `ret`. Speed TU (no pragmas; RTM `and esp,-8` frame).
// Callees: memmove 0x00BFEAE6 (inline STLport clear), CTCBase::PeekWorld() const 0x00686A80, CWorld slot 13 (+0x34)
//   PeekWorldMap, CWorldMap::GetRegion 0x004FC180, malloc 0x00BFEA0E / free 0x00BFEA14 (STLport list),
//   CThingSearchTools::GetAllThings<CThing, list, CFilter_Bind2<CThingFilter_HasTC, CThingFilter_InMap>> 0x00617780
//   (same-TU stand-in), vector<long>::_M_insert_overflow 0x00440990.
// Shape: the thing search goes through an inline CWorld::GetAllThings(filter, list&) wrapper (filters stored before
//   GetWorld, as UpdateRegionsCorrespondances 0x005FC0C0); the teleporter lookup is an inline getter with an uninitialised
//   result and a fully inlined const lower_bound; the push_back of the loop index passes the index's frame slot and the
//   thing walk breaks after the first enabled teleporter.
// Layout: local view -- CTCInventoryMap teleport region list vector<long> +0x208, region count +0x23C; CWorld +0x20
//   thing search tools (engine/CWorld.h); CRegion's linked-map vector<long> +0x00; teleporter (0xDD) enabled flag +0x10.
#include "fable_compiler.h"
#include <new>
extern "C" void* __cdecl memmove(void* dst, const void* src, unsigned int n);   // 0x00BFEAE6
namespace std {
template <class _Tp> class allocator { public: allocator() { } };
struct __true_type { __true_type() { } };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector { // std::vector<long> (12 bytes)
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    static _Tp* copy_trivial(_Tp* first, _Tp* last, _Tp* result)
    {
        if (first == last)
            return result;
        unsigned int n = (char*)last - (char*)first;
        return (_Tp*)((char*)memmove(result, first, n) + n);
    }
    void erase(_Tp* first, _Tp* last) { _M_finish = copy_trivial(last, _M_finish, first); }
    void clear() { erase(_M_start, _M_finish); }
protected:
    void _M_insert_overflow(_Tp* position, const _Tp& x, const __true_type& tag, unsigned int n, bool atEnd);   // 0x00440990
public:
    void push_back(const _Tp& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) _Tp(x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, x, __true_type(), 1, true);
    }
};
}

extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E
extern "C" void __cdecl free(void* block);              // 0x00BFEA14
template <class T>
struct _List_node {
    _List_node* _M_next;
    _List_node* _M_prev;
    T           _M_data;
};
template <class T>
struct _STLP_alloc_proxy {
    _List_node<T>* _M_data;
    _STLP_alloc_proxy(_List_node<T>* p) : _M_data(p) {}
};
template <class T>
struct list {
    typedef _List_node<T> _Node;
    _STLP_alloc_proxy<T> _M_node;
    list() : _M_node(0)
    {
        _Node* node = (_Node*)malloc(sizeof(_Node));
        node->_M_next = node;
        node->_M_prev = node;
        _M_node._M_data = node;
    }
    ~list()
    {
        clear();
        free(_M_node._M_data);
    }
    void clear()
    {
        _Node* cur = _M_node._M_data->_M_next;
        while (cur != _M_node._M_data) {
            _Node* tmp = cur;
            cur = cur->_M_next;
            free(tmp);
        }
        _M_node._M_data->_M_next = _M_node._M_data;
        _M_node._M_data->_M_prev = _M_node._M_data;
    }
    void push_back(const T& x)
    {
        _Node* tmp = (_Node*)malloc(sizeof(_Node));
        tmp->_M_data = x;
        _Node* n = _M_node._M_data;
        _Node* p = n->_M_prev;
        tmp->_M_next = n;
        tmp->_M_prev = p;
        p->_M_next = tmp;
        n->_M_prev = tmp;
    }
};

class CTCBase;
struct CInterfaceMap {                                // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44 (lower_bound inlined)
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                   // +0x20 bitset of interface types
    CInterfaceMap m_interfaces;                       // +0x44
    FABLE_FORCEINLINE unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    FABLE_FORCEINLINE CTCBase* GetTC(long kind) const
    {
        const CInterfaceMap::SEntry* it = m_interfaces.first;
        long count = m_interfaces.last - m_interfaces.first;
        while (count > 0) {
            long half = count / 2;
            if (it[half].type < kind) {
                it += half + 1;
                count -= half + 1;
            } else
                count = half;
        }
        const CInterfaceMap::SEntry* end = m_interfaces.last;
        return (it == end || kind < it->type ? end : it)->component;
    }
    template<class T> FABLE_FORCEINLINE T* PeekTC(long kind) const
    {
        T* out;
        if (HasTC(kind))
            out = (T*)GetTC(kind);
        return out;
    }
};
class CTCTeleporter { public:
    unsigned char _pad00[0x10];
    bool Enabled;                                     // +0x10
};
class CRegion { public:
    long* LinkedMapsStart;                            // +0x00 vector<long>
    long* LinkedMapsFinish;
    long* LinkedMapsEnd;
    unsigned long size() const { return LinkedMapsFinish - LinkedMapsStart; }
};
class CWorldMap { public:
public:
    const CRegion& GetRegion(long) const;   // 0x004FC180
};
class CThingFilter_HasTC { public:
    long m_type;
    CThingFilter_HasTC(long type) : m_type(type) { }
};
class CThingFilter_InMap { public:
    const CWorldMap* m_worldMap;
    long m_map;
    CThingFilter_InMap(const CWorldMap& worldMap, long map) : m_worldMap(&worldMap), m_map(map) { }
};
struct CFilter_Bind2 {                                // CFilter_Bind2<CThingFilter_HasTC, CThingFilter_InMap>
    const CThingFilter_HasTC* m_first;
    const CThingFilter_InMap* m_second;
    CFilter_Bind2(const CThingFilter_HasTC& first, const CThingFilter_InMap& second) : m_first(&first), m_second(&second) { }
};
inline CFilter_Bind2 operator&&(const CThingFilter_HasTC& a, const CThingFilter_InMap& b) { return CFilter_Bind2(a, b); }
CThing* PeekSearchCandidate();
class CThingSearchTools { public:
    void GetAllThings(const CFilter_Bind2& filter, list<CThing*>& out);   // 0x00617780
};
#pragma auto_inline(off)
// Same-TU stand-in for GetAllThings<CThing, list, CFilter_Bind2<HasTC, InMap>> 0x00617780: it appends through the out
// list without writing or keeping its node word (retail keeps the list's node in esi and reuses the list's slot).
void CThingSearchTools::GetAllThings(const CFilter_Bind2& filter, list<CThing*>& out)
{
    CThing* thing = PeekSearchCandidate();
    if (thing && filter.m_first->m_type)
        out.push_back(thing);
}
#pragma auto_inline(on)
class CWorld { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual CWorldMap* GetWorldMap();                 // slot 12
    virtual const CWorldMap* PeekWorldMap() const;    // slot 13 (+0x34) (FableWin.pdb: const)
    unsigned char _pad04[0x20 - 0x04];
    CThingSearchTools* PThingSearchTools;             // +0x20
    void GetAllThings(const CFilter_Bind2& filter, list<CThing*>& out) const { PThingSearchTools->GetAllThings(filter, out); }
};
class CTCBase { protected:
    const CWorld& PeekWorld() const;   // 0x00686A80 (Ego_r.pdb CTCBase, protected)
};
class CTCInventoryMap : public CTCBase { public:
    unsigned char _pad00[0x208];
    std::vector<long>   TeleportRegions;                    // +0x208
    unsigned char _pad214[0x23c - 0x214];
    long          NumberOfRegions;                    // +0x23c
private:
    void FindTeleports();                             // private (Ego_r.pdb / FableWin.pdb AAE)
};
void CTCInventoryMap::FindTeleports()
{
    TeleportRegions.clear();
    const CWorldMap* worldMap = PeekWorld().PeekWorldMap();
    for (long r = 1; r <= NumberOfRegions; ++r) {
        const CRegion& region = worldMap->GetRegion(r);
        for (unsigned long i = 0; i < region.size(); ++i) {
            list<CThing*> things;
            PeekWorld().GetAllThings(CThingFilter_HasTC(0xdd) && CThingFilter_InMap(*worldMap, region.LinkedMapsStart[i]), things);
            for (_List_node<CThing*>* node = things._M_node._M_data->_M_next; node != things._M_node._M_data; node = node->_M_next) {
                CTCTeleporter* teleporter = node->_M_data->PeekTC<CTCTeleporter>(0xdd);
                if (teleporter->Enabled) {
                    TeleportRegions.push_back(r);
                    break;
                }
            }
        }
    }
}