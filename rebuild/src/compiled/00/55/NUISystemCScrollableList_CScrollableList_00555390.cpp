// Grade: RELOCATION_MATCH (Fable landing 2026-10-19 after the Codex review hold: wrong callee: ::operator new where retail calls the malloc import 0xBFEA0E)
// NUISystem::CScrollableList::~CScrollableList() -- mark both scroll arrows for deletion using method 2 and an empty associated-parent list
// Leaf: ??1CScrollableList
// Retail00555390,174 bytes through ret0055543d; two following int3 bytes excluded.
// RTTI COL0133AC88/CHD0133AC78 confirms CUIInterface/CObserver/CActionParam bases.
// Secondary slot0 adjustors005562D0/E0 target deleting destructor005562F0.
// PDB CDeletion: Method+0,AssociatedParents+4; method2 value retained without inferred enum name.
// No shared CScrollableList/CChangingStateComponent header exists. Retail MI bases are +0,+4,+18;
// PDB arrow pointers +1a0/+1a4 shift to +170/+174 in shipped layout.
// Deletion value copy0042CD84, list destructor0042ABCA, changing-state destructor0052CCF0.
// The STLport list header node comes from the CRT malloc import thunk 0x00BFEA0E (not ::operator new).
// Local CObserver: engine/CObserver.h is the Ego_r donor (16-byte set, 0x18 bytes); the retail STLport set is 12 bytes,
// so the native CObserver is 0x14 bytes and the header does not fit.
// NF156 (2026-11-20, q4776): the local CDeletionParents / CDeletion / CUIInterface / CObserver / CActionParam / CComponent /
//   CChangingStateComponent / CScrollableList models replaced by the shared headers: engine/NUISystem_CScrollableList.h (PUpArrow
//   +0x170 / PDownArrow +0x174) and engine/NUISystem_CDeletion.h with its in-class constructors (FABLE_RETAIL_CDELETION_INLINE: retail
//   expands the default constructor here -- the head node malloc'd in place -- and calls the copy constructor 0x0042CD84) over the
//   STLport list<CComponent*> (~list 0x0042ABCA out of line with its body visible, FABLE_RETAIL_STLP_LIST_DTOR_OUTOFLINE). The deletion
//   method 2 is CDeletion::UI_DELETE_ALL_INSTANCES. Was ??1CDeletionParents@NUISystem@@QAE@XZ.
#define FABLE_RETAIL_CDELETION_INLINE
#define FABLE_RETAIL_STLP_LIST_DTOR_OUTOFLINE
#include "engine/NUISystem_CScrollableList.h"
#include "engine/NUISystem_CDeletion.h"
NUISystem::CScrollableList::~CScrollableList() {
    CDeletion deletion;
    deletion.Method = CDeletion::UI_DELETE_ALL_INSTANCES;
    if (PUpArrow) { PUpArrow->SetDeletion(deletion); PUpArrow=0; }
    if (PDownArrow) { PDownArrow->SetDeletion(deletion); PDownArrow=0; }
}
