// CAttackHistoryCombo::`scalar deleting destructor'(unsigned int flags) -- destroy the record (free its vector block) and free it when bit 0 is set
// Leaf: ??_GCAttackHistoryCombo@@QAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E1CFC, 36 bytes, thiscall, `ret 4` (size-optimised module).
// Identity (proven): its callers are the landed std::__destroy_aux<CAttackHistoryCombo*> 0x004E41C0 (16-byte stride, flags 0) and 0x004E3686; Ego_r.pdb CAttackHistoryCombo {vector<pair<EAttackHistoryType, EDamageAttribute> > AttackCombo, float Multiplier}.
// Callees: free 0x00BFEA14 (the vector's block), operator delete 0x00BFE9BC.
// Shape (family M100, representative 0x004E1CFC): the compiler-generated non-virtual `??_G` -- the implicit destructor inline
//   (STLport ~_Vector_base: `if (_M_start) deallocate`), then the flag test; the delete-expression in DeleteOne forces it into
//   the object (not part of the retail body).
#pragma optimize("s", on)
extern "C" void __cdecl free(void* block);                 // 0x00BFEA14
void __cdecl operator delete(void* p);                      // 0x00BFE9BC
namespace std {
template <class _Tp> class allocator {
public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { free(__p); }
};
template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc { public: _Tp _M_data; };
template <class _Tp, class _Alloc> class _Vector_base {
public:
    ~_Vector_base() { if (_M_start != 0) _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    ~vector() { }
};
}
enum EAttackHistoryType { ATTACK_HISTORY_TYPE_NONE = 0 };
enum EDamageAttribute { DAMAGE_ATTRIBUTE_NONE = 0 };
namespace std { template <class _T1, class _T2> struct pair { _T1 first; _T2 second; }; }
class CAttackHistoryCombo {                               // Ego_r.pdb, 0x10 bytes in retail
public:
    std::vector<std::pair<EAttackHistoryType, EDamageAttribute> > AttackCombo;   // +0x0
    float Multiplier;                                     // +0xc
};

void DeleteOne(CAttackHistoryCombo* object)   // forces the scalar deleting destructor into the object; not part of the retail body
{
    delete object;
}