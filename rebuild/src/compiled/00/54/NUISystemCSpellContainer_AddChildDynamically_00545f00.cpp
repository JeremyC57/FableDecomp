// NUISystem::CSpellContainer::AddChildDynamically(CCountedPointer<CComponent>& child, unsigned long index) -- give the new child the per-state position and colour of the child currently in slot index, then, when index is a valid slot, insert the child there, parent it to the container, initialise it and re-apply the state of the component now in that slot (the new child itself)
// Leaf: ?AddChildDynamically@CSpellContainer@NUISystem@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00545F00, 332 bytes (+ int3 padding), `ret 8`. Slot 142 of the CSpellContainer
//   vtable 0x0124786C (absolute ref 0x01247AA4); Ego_r.pdb
//   ?AddChildDynamically@CSpellContainer@NUISystem@@UAEXAAV?$CCountedPointer@VCComponent@NUISystem@@@@K@Z.
// Callees: the components' virtual slots 82 (+0x148, GetStates: the state map by reference), 51 (+0xcc, SetParent), 43
//   (+0xac, Initialise), 86 (+0x158, GetState) and 48 (+0xc0, ChangeState) as named by the landed CSpellContainer /
//   CComponentContainer rows; map<unsigned long, CUIState>::operator[](const unsigned long&) @ 0x0042D5B1 (manifest
//   "CPersistTraits::Transfer" is propagated); STLport vector<CCountedPointer<CComponent>>::insert(iterator, const
//   value_type&) @ 0x00535B10 (its iterator result unused).
// Layout: as the landed CSpellContainer::AddChildAndSetOffsets 0x00545DF0 / CComponentContainer::Initialise 0x005461D0
//   (local model, CComponent is not in the PDB dump): Children (8-byte counted pointers) +0xb0; CUIState (Ego_r.pdb)
//   Position +0x08, Zoom +0x10, Colour +0x18; STLport tree node {colour, parent, left, right, key +0x10, value +0x14}.
// Shape: the walk keeps the child's state map in a local (header re-read for each end() test); each copy evaluates the
//   slot child's state (right-hand side) before the new child's operator[]; the STLport increment is inline; the
//   insert position is begin() + index.
// NF44 (round 6): the local C2DVector / CUIState / state-map stand-ins replaced by the shared engine/C2DVector.h,
// engine/NUISystem_CUIState.h and stlport/fable_map.h (FABLE_RETAIL_STLP_MAP + _INDEX_DECL_ONLY: operator[] out of line), so the
// subscript binds STLport map<unsigned long, NUISystem::CUIState>::operator[] 0x0042D5B1 (the landed row spelling).
#include "engine/C2DVector.h"
#include "engine/NUISystem_CUIState.h"
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_MAP_INDEX_DECL_ONLY
#include "stlport/fable_map.h"

namespace NUISystem {

typedef std::map<unsigned long, CUIState> CStateMap;          // STLport, 12 bytes
typedef CStateMap::iterator CStateMapIterator;
class CComponent;
struct CCountedComponentPointer {           // CCountedPointer<NUISystem::CComponent>
    CComponent* Object;
    void*       Info;
    CComponent* operator->() const { return Object; }
};
struct CComponentPointerVector {            // STLport vector<CCountedPointer<CComponent>>
    CCountedComponentPointer* _M_start;
    CCountedComponentPointer* _M_finish;
    CCountedComponentPointer* _M_end_of_storage;
    unsigned int size() const { return _M_finish - _M_start; }
    CCountedComponentPointer* begin() { return _M_start; }
    CCountedComponentPointer& operator[](unsigned int i) { return _M_start[i]; }
    CCountedComponentPointer* insert(CCountedComponentPointer* position, const CCountedComponentPointer& x);   // 0x00535B10
};
class CComponent {
public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042();
    virtual void Initialise();                                      // slot 43 (+0xac)
    virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void ChangeState(unsigned long state);                  // slot 48 (+0xc0)
    virtual void v049(); virtual void v050();
    virtual void SetParent(CComponent* parent);                     // slot 51 (+0xcc)
    virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067();
    virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075();
    virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079(); virtual void v080(); virtual void v081();
    virtual CStateMap& GetStates();                                 // slot 82 (+0x148), returns &StateMap (+0x24)
    virtual void v083(); virtual void v084(); virtual void v085();
    virtual long GetState();                                        // slot 86 (+0x158)
    unsigned char _pad04[0xb0 - 4];
    CComponentPointerVector Children;       // +0xb0
    unsigned char _padbc[0x15c - 0xbc];
};
class CSpellContainer : public CComponent {
public:
    virtual void AddChildDynamically(CCountedComponentPointer& child, unsigned long index);
};

void CSpellContainer::AddChildDynamically(CCountedComponentPointer& child, unsigned long index)
{
    CStateMap& states = child->GetStates();
    for (CStateMapIterator it = states.begin(); it != states.end(); ++it) {
        states[it->first].Position = Children[index]->GetStates()[it->first].Position;
        states[it->first].Colour = Children[index]->GetStates()[it->first].Colour;
    }
    if (index < Children.size()) {
        Children.insert(Children.begin() + index, child);
        child->SetParent(this);
        child->Initialise();
        child->ChangeState(Children[index]->GetState());
    }
}

}