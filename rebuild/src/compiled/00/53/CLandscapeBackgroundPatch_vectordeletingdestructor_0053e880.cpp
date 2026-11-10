// __fastcall this=ecx, flags=stack. base /O2 /Oy (no pragma). family len30 x22 (template 00412b00)
// Leaf: ?VecDel@T@@QAEPAXI@Z
// Name fix (agentNF 2026-11-10): the destroyed object is an STLport list<long> base; its out-of-line destructor 0x0042AB73 is
//   ??1?$_List_base@JV?$allocator@J@std@@@std@@QAE@XZ (was bound as ?Dtor_0042ab73@T@@QAEXXZ). The deleting function itself
//   is unreferenced in retail (no call, jump or pointer to 0x0053E880); its own name stays descriptive.
namespace std {
template <class _Tp> class allocator;
template <class _Tp, class _Alloc> class _List_base { public:
    void* _M_node;
    ~_List_base();                                      // 0x0042AB73
};
}
struct T : std::_List_base<long, std::allocator<long> > { void* VecDel(unsigned flags); };
void __cdecl operator delete(void* p);
void* T::VecDel(unsigned flags) {
    this->std::_List_base<long, std::allocator<long> >::~_List_base();
    if (flags & 1) ::operator delete(this);
    return this;
}
