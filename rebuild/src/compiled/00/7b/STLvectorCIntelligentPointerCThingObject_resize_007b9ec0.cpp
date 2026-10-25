// std::vector<CIntelligentPointer<CThingObject> >::resize(unsigned int new_size) -- STLport one-argument resize of a tavern table's tankard/food pointer vector: resize(n, CIntelligentPointer<CThingObject>()) with the empty pointer destroyed on both exits
// Leaf: ?resize@?$vector@V?$CIntelligentPointer@VCThingObject@@@@@std@@QAEXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007B9EC0, 116 bytes (+ int3 padding), `__fastcall` (this -> esi), `ret 4`.
// Identity: Ego_r.pdb `?resize@?$vector@V?$CIntelligentPointer@VCThingObject@@@@...@@QAEXI@Z` (CTCTavernTable::Tankards
//   and ::Food); both callers (0x007B91F1, 0x007B91FA) are CTCTavernTable::OnInitialActivate 0x007B90F0. The temporary's
//   vtable 0x0125A858 is CIntelligentPointer<CThingObject>. The catalog's
//   "vector<CIntelligentPointer<NParticleEngine::CParticleEmitter>>::resize" is propagated (same 8-byte shape).
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer() 0x00A01B10, vector::erase(first, last) 0x007B9870 (catalog
//   "StdVector_Clear"), vector::_M_fill_insert(pos, n, x) 0x007B9B40, ~CBaseIntelligentPointer 0x00A01C10 (virtual,
//   called directly on the complete temporary through the derived class's implicit destructor).
// Shape: STLport 4.6 `resize(n) { resize(n, _Tp()); }` over the inline `resize(n, x) { if (n < size()) erase(begin() + n,
//   end()); else insert(end(), n - size(), x); }`: the default element in the 8-byte frame (base constructor, then the derived vtable store scheduled after the compare), its address pushed
//   to the fill-insert, and its destructor on both exits.
class CBaseObject;

class CBaseIntelligentPointer {                         // 8 bytes: vptr, item
public:
    CBaseObject* PItem;                                 // +0x04
    CBaseIntelligentPointer();                          // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
};

template <class T>
class CIntelligentPointer : public CBaseIntelligentPointer {   // vtable 0x0125A858 for CThingObject
public:
    CIntelligentPointer() : CBaseIntelligentPointer() { }
    virtual void v01();                                 // gives the derived class its own vtable
};

class CThingObject;

namespace std {
template <class _Tp>
class vector {
public:
    _Tp* _M_start;                                      // +0x00
    _Tp* _M_finish;                                     // +0x04
    _Tp* _M_end_of_storage;                             // +0x08
    _Tp* begin() { return _M_start; }
    _Tp* end() { return _M_finish; }
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    _Tp* erase(_Tp* __first, _Tp* __last);              // 0x007B9870
    void _M_fill_insert(_Tp* __pos, unsigned int __n, const _Tp& __x);   // 0x007B9B40
    void insert(_Tp* __pos, unsigned int __n, const _Tp& __x) { _M_fill_insert(__pos, __n, __x); }
    void resize(unsigned int __new_size, const _Tp& __x)
    {
        if (__new_size < size())
            erase(begin() + __new_size, end());
        else
            insert(end(), __new_size - size(), __x);
    }
    void resize(unsigned int __new_size) { resize(__new_size, _Tp()); }
};

template void vector<CIntelligentPointer<CThingObject> >::resize(unsigned int);
}