// NUISystem::CDeletion::CDeletion(const CDeletion& other) -- copy constructor of the UI component deletion record: Method copied inline, AssociatedParents through the list copy constructor
// Leaf: ??0CDeletion@NUISystem@@QAE@ABV01@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0042CD84, 29 bytes, `ret 4`:
//   mov eax,[esp+4] ; push esi ; mov esi,ecx ; mov ecx,[eax] ; add eax,4 ; mov [esi],ecx ; push eax ; lea ecx,[esi+4] ;
//   call list<CComponent*>::list(const list&) 0x0042BF06 ; mov eax,esi ; pop esi ; ret 4
// Identity (sweep2, FINDINGS chunk 474 "0x42CD84 = CDeletion copy ctor"): the name sets hold ??0CDeletion@NUISystem@@QAE@ABV01@@Z;
//   Ego_r.pdb NUISystem::CDeletion is {EDeletionMethod Method +0; list<NUISystem::CComponent*> AssociatedParents +4} (8 bytes in
//   retail, the STLport list is one node pointer). Its 88 callers (NUISystem::CManager 0x0041E6D3, ...) copy a local built by
//   CDeletion::CDeletion() 0x0042BED4 into the by-value argument of CComponent::SetDeletion(CDeletion) (vtable slot 62,
//   `call [edi+0xf8]`). Was the generated stand-in SPairLongCSecond_0042bf06 (catalog "pair<unsigned long,CAnimationEntry>").
namespace std {
template <class _Tp> class allocator {
public:
    allocator() { }
};
template <class _Tp, class _Alloc = allocator<_Tp> > class list {
public:
    void* _M_node;                                          // the STLport sentinel node pointer
    list(const list<_Tp, _Alloc>& __x);                     // 0x0042BF06
};
}

namespace NUISystem {
class CComponent;
enum EDeletionMethod { DELETION_METHOD_DEFAULT };

class CDeletion {
public:
    EDeletionMethod        Method;                          // +0x00
    std::list<CComponent*> AssociatedParents;               // +0x04
    CDeletion(const CDeletion& other);
};

CDeletion::CDeletion(const CDeletion& other)
    : Method(other.Method)
    , AssociatedParents(other.AssociatedParents)
{
}
}
