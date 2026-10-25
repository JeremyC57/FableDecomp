// NUISystem::CDeletion::CDeletion() -- default-constructs a UI component deletion record: Method left unset, empty AssociatedParents list
// Leaf: ??0CDeletion@NUISystem@@QAE@XZ
// Retail: Fable.exe @ 0x0042BED4, 22 bytes, returns this: constructs the list at +4 through list<CComponent*>::list(const allocator&)
//   0x0042ABAF with a default allocator temporary (`lea eax,[esp+7]`). Catalog label CBankFile::GetBankHandle is wrong.
// Identity (sweep2): was landed as NUISystem::CAction::CAction, but (a) the object is 8 bytes and goes BY VALUE to
//   CComponent::SetDeletion(CDeletion) (NUISystem::CManager 0x0041E922: ctor into [ebp-0x248], `mov [ebp-0x248],1`, copy via
//   CDeletion's copy ctor 0x0042CD84, `call [edi+0xf8]` = vtable slot 62 SetDeletion), and its partner dtor 0x0042BEEA
//   destroys the +4 list with ~list<NUISystem::CComponent*> 0x0042ABCA; (b) Ego_r.pdb CDeletion is {EDeletionMethod Method;
//   list<CComponent*> AssociatedParents} (one list), while CAction holds two lists and a bool (16 bytes in retail).
//   Name set: ??0CDeletion@NUISystem@@QAE@XZ.
namespace std {
template <class _Tp> class allocator {
public:
    allocator() { }
};
template <class _Tp, class _Alloc = allocator<_Tp> > class list {
public:
    typedef _Alloc allocator_type;
    explicit list(const allocator_type& __a = allocator_type());   // 0x0042ABAF (out of line)
    void* _M_node;
};
}
namespace NUISystem {
class CComponent;
enum EDeletionMethod { DELETION_METHOD_DEFAULT };
class CDeletion {
public:
    EDeletionMethod        Method;                  // +0x00 (left unset)
    std::list<CComponent*> AssociatedParents;       // +0x04
    CDeletion();
};
CDeletion::CDeletion()
{
}
}
