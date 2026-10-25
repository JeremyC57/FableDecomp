// NUISystem::CList::AddScrollingChildDynamically(const CCountedPointer<CComponent>& child) -- add a child at run time: the base CChangingStateComponent bookkeeping, then record it in ScrollingChildren (+0x164), mark it visible in ScrollingChildrenVisibility (+0x1c8), copy its state map (GetStates(), slot 82) into ScrollingChildrenStates (+0x17c) and store its row index (last row of +0xb0) in +0xe4
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0053BEAC, length 116 bytes.
// Callees: CChangingStateComponent::AddChildDynamically @ 0x0052CAB0 (direct base call), vector<CComponent*>::push_back
//          @ 0x00486024, the visibility flags' push_back(const bool&) @ 0x0053CB4F, vector<map<unsigned long, CUIState> >
//          ::push_back @ 0x0053CFC2 (landed; declared with that row's _STL::vector<CUIStateMap> spelling so the symbols
//          agree), vector<long>::push_back @ 0x005178A0, the child's virtual slot 82 (+0x148) GetStates(), whose returned
//          reference is pushed straight on (Ego_r.pdb ?GetStates@CComponent@NUISystem@@UAEAAV?$map@KVCUIState@...).
// Layout: Ego_r.pdb CList ScrollingChildren +0x194, ScrollingChildrenStates +0x1b4, ScrollingChildrenVisibility +0x204;
//          retail is 0x30 below at ScrollingChildren and a further 4 below per STLport 12-byte vector (+0x164 / +0x17c /
//          +0x1c8). The earlier model called +0x17c a CScrollLayout with AddChild(long) (FINDINGS chunk 376).
// Shape: the frame-pointer size TU (`optimize("y", off)` + `("s", on)`); every push_back argument is an rvalue, so
// VC copies it into the dead `child` argument slot (`mov [ebp+8],eax ; lea eax,[ebp+8]`), the `true` into that
// slot's high byte (`mov byte ptr [ebp+0Bh],1`); the row count is the signed `(last - first) >> 3` minus one.
#pragma optimize("y", off)
#pragma optimize("s", on)
namespace _STL {
class CUIStateMap;                                 // map<unsigned long, NUISystem::CUIState> (12 bytes)
template <class T> class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    void push_back(const T& x);                    // 0x0053CFC2 for CUIStateMap
};
}
namespace NUISystem {
struct CComponent {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055();
    virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063();
    virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071();
    virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079();
    virtual void v080(); virtual void v081();
    virtual _STL::CUIStateMap& GetStates();        // slot 82 (+0x148)
};
}
template <class T> class CCountedPointer {
public:
    T*    m_pObject;
    T*    GetPtr() const { return m_pObject; }
    T*    operator->() const { return m_pObject; }
};
template <class T> struct CRetailVector {
    T* m_first;
    T* m_last;
    T* m_end;
    long size() const { return m_last - m_first; }
    void push_back(const T& value);
};
struct CBoolList { void push_back(const bool& value); };   // 0x0053CB4F
struct SRow { long a; long b; };
struct CChangingStateComponent : NUISystem::CComponent {
    void AddChildDynamically(const CCountedPointer<NUISystem::CComponent>& child);   // 0x0052CAB0
};
namespace NUISystem {
struct CList : CChangingStateComponent {
    unsigned char               _pad04[0xac];
    CRetailVector<SRow>         m_rows;            // +0xb0
    unsigned char               _padbc[0x28];
    CRetailVector<long>         m_rowIndices;      // +0xe4
    unsigned char               _padf0[0x74];
    CRetailVector<CComponent*>  ScrollingChildren;             // +0x164
    unsigned char               _pad170[0xc];                  // NonScrollingChildren
    _STL::vector<_STL::CUIStateMap> ScrollingChildrenStates;   // +0x17c
    unsigned char               _pad188[0x40];
    CBoolList                   ScrollingChildrenVisibility;   // +0x1c8
    void AddScrollingChildDynamically(const CCountedPointer<CComponent>& child);
};
}

void NUISystem::CList::AddScrollingChildDynamically(const CCountedPointer<CComponent>& child)
{
    CChangingStateComponent::AddChildDynamically(child);
    ScrollingChildren.push_back(child.GetPtr());
    ScrollingChildrenVisibility.push_back(true);
    ScrollingChildrenStates.push_back(child->GetStates());
    m_rowIndices.push_back(m_rows.size() - 1);
}