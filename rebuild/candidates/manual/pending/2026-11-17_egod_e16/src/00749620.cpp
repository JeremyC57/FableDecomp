// std::vector<std::pair<CIntelligentPointer<const CThing>,NStealth::CDangerStatus> >::push_back(const value_type& __x) -- append a copy of the danger-thing entry (inline CIntelligentPointer copy, CDangerStatus copy) at _M_finish, or grow through _M_insert_overflow when full
// Leaf: ?push_back@?$vector@U?$pair@V?$CIntelligentPointer@$$CBVCThing@@@@VCDangerStatus@NStealth@@@std@@V?$allocator@U?$pair@V?$CIntelligentPointer@$$CBVCThing@@@@VCDangerStatus@NStealth@@@std@@@2@@std@@QAEXABU?$pair@V?$CIntelligentPointer@$$CBVCThing@@@@VCDangerStatus@NStealth@@@2@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00749620, 105 bytes, __thiscall `ret 4`.
// Identity: Ego_d.pdb tc_stealth.obj lists std::vector<std::pair<CIntelligentPointer<CThing const>,NStealth::CDangerStatus>>::push_back;
//   Ego_r.pdb ?push_back@?$vector@U?$pair@V?$CIntelligentPointer@$$CBVCThing@@@@VCDangerStatus@NStealth@@@std@@... (donor 0x0075B740;
//   both PDBs are Dinkumware builds, so retail's STLport spelling takes `const value_type&`); the caller CTCStealth::SetAsInDanger
//   0x00747B80 pushes onto CTCStealth::DangerThings (shared CTCStealth.h); the inline copy stores the CIntelligentPointer<const CThing>
//   vtable 0x01237F1C (RTTI .?AV?$CIntelligentPointer@$$CBVCThing@@@@).
// Callees: CBaseIntelligentPointer ctor 0x00A01B10 / GetPItem 0x00A01B50 / SetPItem 0x00A01B90 (the inline copy constructor),
//   vector::_M_insert_overflow(pointer, const value_type&, const __false_type&, size_type, bool) 0x00749420.
// Shape: STLport `if (_M_finish != _M_end_of_storage._M_data) { _Construct(_M_finish, __x); ++_M_finish; } else
//   _M_insert_overflow(_M_finish, __x, __false_type(), 1UL, true);` from stlport/fable_containers.h. The element copy is the
//   header's FABLE_RETAIL_CINTELLIGENTPOINTER_COPY_ORIGINAL_CHAIN body (`SetPointer(item.GetPointer())`, the FableWin accessor
//   chain); with the shorter `SetPItem(other.GetPItem())` body VC7.1 swaps this / __x between ebx and edi and bumps _M_finish
//   with `add [edi+4],10h` (DIFFER 98v105, the old draft's residue).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY_ORIGINAL_CHAIN
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/C2DVector.h"
#include "engine/EGameAction.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CTCStealth.h"

template void std::vector<std::pair<CIntelligentPointer<CThing const >,NStealth::CDangerStatus>,std::allocator<std::pair<CIntelligentPointer<CThing const >,NStealth::CDangerStatus> > >::push_back(const std::pair<CIntelligentPointer<CThing const >,NStealth::CDangerStatus>&);
