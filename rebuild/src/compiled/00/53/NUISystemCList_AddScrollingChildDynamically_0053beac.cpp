// NUISystem::CList::AddScrollingChildDynamically(const CCountedPointer<CComponent>& child) -- add a child at run time: the base CChangingStateComponent bookkeeping, then record it in the list's child vector (+0x164), mark its slot visible (+0x1c8), register its size key (slot 82) with the scroll layout (+0x17c) and store its row index (last row of +0xb0) in +0xe4
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0053BEAC, length 116 bytes.
// Callees: CChangingStateComponent::AddChildDynamically @ 0x0052CAB0 (direct base call), vector<CComponent*>::push_back
//          @ 0x00486024, the visibility flags' push_back(const bool&) @ 0x0053CB4F, the scroll layout's AddChild
//          @ 0x0053CFC2, vector<long>::push_back @ 0x005178A0, the child's virtual slot 82 (+0x148).
// Shape: the frame-pointer size TU (`optimize("y", off)` + `("s", on)`); every push_back argument is an rvalue, so
// VC copies it into the dead `child` argument slot (`mov [ebp+8],eax ; lea eax,[ebp+8]`), the `true` into that
// slot's high byte (`mov byte ptr [ebp+0Bh],1`); the row count is the signed `(last - first) >> 3` minus one.
#pragma optimize("y", off)
#pragma optimize("s", on)
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
    virtual long GetLayoutKey();                   // slot 82 (+0x148)
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
struct CScrollLayout { void* m_pRows; void AddChild(long key); };   // AddChild @ 0x0053CFC2
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
    CRetailVector<CComponent*>  m_children;        // +0x164
    unsigned char               _pad170[0xc];
    CScrollLayout               m_layout;          // +0x17c
    unsigned char               _pad180[0x48];   // (+0x180)
    CBoolList                   m_visible;         // +0x1c8
    void AddScrollingChildDynamically(const CCountedPointer<CComponent>& child);
};
}

void NUISystem::CList::AddScrollingChildDynamically(const CCountedPointer<CComponent>& child)
{
    CChangingStateComponent::AddChildDynamically(child);
    m_children.push_back(child.GetPtr());
    m_visible.push_back(true);
    m_layout.AddChild(child->GetLayoutKey());
    m_rowIndices.push_back(m_rows.size() - 1);
}