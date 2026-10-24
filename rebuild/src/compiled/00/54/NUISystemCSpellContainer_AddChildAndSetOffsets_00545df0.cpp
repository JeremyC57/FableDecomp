// NUISystem::CSpellContainer::AddChildAndSetOffsets(CCountedPointer<CComponent>& child) -- place the new child one offset step per existing child along X (in its state 0), then add it
// Leaf: AddChildAndSetOffsets
// Retail: Fable.exe (TLC, VC7.1) @ 0x00545DF0, 106 bytes (`ret 4`).
// Callees: the child's virtual slot 82 (+0x148, GetStates(), the state map by reference), map<unsigned long, CUIState>::
// operator[](const unsigned long&) @ 0x0042D5B1 (manifest "CPersistTraits::Transfer" is propagated: lower_bound, insert a
// default CUIState when absent, return the value), own virtual slot 59 (+0xec, AddChild(CCountedPointer<CComponent>&)).
// Label: CSpellContainer vtable 0x0124786C slot 147 (rtti-vtables.tsv); Ego_r.pdb names
// ?AddChildAndSetOffsets@CSpellContainer@NUISystem@@UAEXAAV?$CCountedPointer@VCComponent@NUISystem@@@@@Z.
// Layout: Children (12-byte vector of 8-byte counted pointers) at retail +0xb0; Ego_r.pdb CSpellContainer PositionOffset
// (C2DVector) +0x1a8 = retail +0x174 (BaseSpells 12 bytes at +0x15c, NewSpell +0x168, EmptySlot +0x170).
// CUIState::Position.X sits at +0x08 (landed CUIState 0x0052E5D0).
// Shape: one statement: the right-hand side `(float)Children.size()` (fild + the 2^32 fix-up) times PositionOffset.X is
// evaluated first and spilled into the dead argument slot across the calls (the child pointer is read before the slot is
// reused; a named float local gets its own frame word instead); the map key 0 is a frame temporary; GetStates() is called before the operator[] whose
// key argument was pushed first; the float is stored as a raw dword.
struct C2DVector {
    float X;
    float Y;
    C2DVector() { }
};

namespace NUISystem {
class CComponent;
}
struct CCPPointerInfo;
template <class T> class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    T* operator->() const { return Data; }
};
template <class T> class CCountedVector {         // STLport vector<CCountedPointer<T>> (12 bytes)
public:
    CCountedPointer<T>* _M_start;
    CCountedPointer<T>* _M_finish;
    CCountedPointer<T>* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
};

namespace NUISystem {
class CUIState {
public:
    unsigned long StateChangeFlag;                 // +0x00
    long          StateChangeType;                 // +0x04
    C2DVector     Position;                        // +0x08
};
class CStateMap {                                  // map<unsigned long, CUIState>
public:
    unsigned char _rep[0xc];
    CUIState& operator[](const unsigned long& key);   // 0x0042D5B1
};
class CComponent {
public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055();
    virtual void v056(); virtual void v057(); virtual void v058();
    virtual void AddChild(CCountedPointer<CComponent>& child);   // slot 59 (+0xec)
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067();
    virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075();
    virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079(); virtual void v080(); virtual void v081();
    virtual CStateMap& GetStates();                // slot 82 (+0x148)
    unsigned char _pad04[0xb0 - 4];
    CCountedVector<CComponent> Children;           // +0xb0
    unsigned char _padbc[0x15c - 0xbc];
};
class CSpellContainer : public CComponent {
public:
    CCountedVector<CComponent> BaseSpells;         // +0x15c
    CComponent*   NewSpellComponent;               // +0x168
    unsigned long NewSpellIndex;                   // +0x16c
    long          EmptySlot;                       // +0x170
    C2DVector     PositionOffset;                  // +0x174
    void AddChildAndSetOffsets(CCountedPointer<CComponent>& child);
};

void CSpellContainer::AddChildAndSetOffsets(CCountedPointer<CComponent>& child)
{
    child->GetStates()[0].Position.X = PositionOffset.X * (float)Children.size();
    AddChild(child);
}
}