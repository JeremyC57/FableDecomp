// CTCInventoryTrade::RefreshEntriesAmountsAndAvailabilitiesPC(NUISystem::CComponent* component, CListMap& lists) -- count a category component's entries for the PC trade screen: for a list component (type 8) with a mapped list, the type-7 entries with nothing left (both counts); otherwise recurse into each type-7 child, adding one plus its counts
// Leaf: RefreshEntriesAmountsAndAvailabilitiesPC
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0059FAFE, length 279 bytes, `ret 0Ch` (hidden pair result). Frame-pointer size TU:
// optimize("y",off) + ("s",on). FableWin.pdb ?RefreshEntriesAmountsAndAvailabilitiesPC@CTCInventoryTrade@@AAE?AU?$pair@KK@std@@
// PAVCComponent@NUISystem@@AAV?$map@...CList...@std@@@Z.
// Callees: NUISystem::CComponent virtual slots 53 (+0xD4, the child vector), 65 (+0x104, the component type) and 63 (+0xFC, a
// pointer to the entry's remaining amount; names descriptive), map<CComponent*, CList*>::find @ 0x005B1A43 (catalog
// "std::set_FindLE" is propagated; STLport node: value +0x14, end() = the header at +0), CTCInventoryTrade::
// RecurseRefreshEntriesAmountsAndAvailabilitiesPC @ 0x0059E6BD (hidden pair result).
// Shape: the counts are one named result pair (its first lives in ebx, its second in the pair's frame home, which is why
// retail keeps a 0x14 frame with an unused word at [ebp-0Ch]) returned by copy through the hidden pointer; the map find is
// defined in this TU with its real STLport body (out of line, auto_inline off) so the caller hands it the component
// argument slot itself instead of homing a key copy there (GOTCHAS 24l lever);
// the mapped-list loop re-fetches the list's children through the virtual for every access, the recursion loop reads the
// component's vector once (begin loaded before end for the size); `this` is parked in [ebp-4] across the virtual calls.
// Layout: STLport vector<CCountedPointer<CComponent> > (8-byte elements, object word first).
#pragma optimize("y", off)
#pragma optimize("s", on)
namespace std {
template <class A, class B> struct pair {
    A first;
    B second;
    pair(const A& a, const B& b) : first(a), second(b) { }
};
}
namespace NUISystem {
struct CComponent;
struct SChildPointer {                          // CCountedPointer<NUISystem::CComponent>
    CComponent* m_pObject;
    void*       m_pCount;
    CComponent* operator->() const { return m_pObject; }
};
struct CChildVector {                           // vector<CCountedPointer<CComponent> > (12 bytes)
    SChildPointer* _M_start;
    SChildPointer* _M_finish;
    SChildPointer* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    const SChildPointer& operator[](unsigned long i) const { return _M_start[i]; }
};
struct CComponent {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52();
    virtual const CChildVector& GetChildren();  // slot 53 (+0xD4)
    virtual void v54(); virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59();
    virtual void v60(); virtual void v61(); virtual void v62();
    virtual const long* GetAmountLeft();        // slot 63 (+0xFC)
    virtual void v64();
    virtual long GetType();                     // slot 65 (+0x104)
};
struct CList : CComponent {
};
}
struct SListMapNode {                           // STLport _Rb_tree_node<pair<CComponent* const, CList*> >
    long          _M_color;
    SListMapNode* _M_parent;
    SListMapNode* _M_left;
    SListMapNode* _M_right;
    NUISystem::CComponent* Key;                 // +0x10
    NUISystem::CList*      List;                // +0x14
};
struct CListMap {                               // std::map<NUISystem::CComponent*, NUISystem::CList*> (12 bytes)
    SListMapNode* _M_header;
    unsigned long _M_node_count;
    long          _M_key_compare;
    SListMapNode* find(NUISystem::CComponent* const& key);   // 0x005B1A43
    SListMapNode* end() const { return _M_header; }
};
#pragma auto_inline(off)
__declspec(noinline) SListMapNode* CListMap::find(NUISystem::CComponent* const& key)
{
    SListMapNode* y = _M_header;
    SListMapNode* x = _M_header->_M_parent;
    while (x != 0) {
        if (!(x->Key < key)) { y = x; x = x->_M_left; }
        else x = x->_M_right;
    }
    if (y == _M_header || key < y->Key) y = _M_header;
    return y;
}
#pragma auto_inline(on)

struct CTCInventoryTrade {
    std::pair<unsigned long, unsigned long> RecurseRefreshEntriesAmountsAndAvailabilitiesPC(NUISystem::CComponent* component, CListMap& lists);   // 0x0059E6BD
    std::pair<unsigned long, unsigned long> RefreshEntriesAmountsAndAvailabilitiesPC(NUISystem::CComponent* component, CListMap& lists);
};

std::pair<unsigned long, unsigned long> CTCInventoryTrade::RefreshEntriesAmountsAndAvailabilitiesPC(NUISystem::CComponent* component, CListMap& lists)
{
    std::pair<unsigned long, unsigned long> result(0, 0);
    const NUISystem::CChildVector& children = component->GetChildren();
    if (component->GetType() == 8) {
        SListMapNode* found = lists.find(component);
        if (found != lists.end()) {
            NUISystem::CList* list = found->List;
            for (unsigned long i = 0; i < list->GetChildren().size(); ++i) {
                if (list->GetChildren()[i]->GetType() == 7 && *list->GetChildren()[i]->GetAmountLeft() == 0) {
                    ++result.first;
                    ++result.second;
                }
            }
        } else {
            for (unsigned long i = 0; i < children.size(); ++i) {
                if (children[i]->GetType() == 7) {
                    std::pair<unsigned long, unsigned long> sub = RecurseRefreshEntriesAmountsAndAvailabilitiesPC(children[i].m_pObject, lists);
                    result.first += sub.first + 1;
                    result.second += sub.second;
                }
            }
        }
    }
    return result;
}