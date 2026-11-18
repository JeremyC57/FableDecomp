// CConsole::RegisteredConsoleInputs insert_unique -- std::map<CCharString, CCountedPointer<CConsoleInputBase> >::insert_unique (STLport _Rb_tree), the console's registered-input map
// Flags: /O2 /Oy /W3
// Leaf: ?insert_unique@?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@V?$CCountedPointer@VCConsoleInputBase@@@@@std@@U?$_Select1st@U?$pair@$$CBVCCharString@@V?$CCountedPointer@VCConsoleInputBase@@@@@std@@@3@U?$less@VCCharString@@@3@V?$allocator@U?$pair@$$CBVCCharString@@V?$CCountedPointer@VCConsoleInputBase@@@@@std@@@3@@std@@QAE?AU?$pair@U?$_Rb_tree_iterator@U?$pair@$$CBVCCharString@@V?$CCountedPointer@VCConsoleInputBase@@@@@std@@U?$_Nonconst_traits@U?$pair@$$CBVCCharString@@V?$CCountedPointer@VCConsoleInputBase@@@@@std@@@2@@std@@_N@2@ABU?$pair@$$CBVCCharString@@V?$CCountedPointer@VCConsoleInputBase@@@@@2@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009EFB80. Ego_r.pdb CConsole::RegisteredConsoleInputs (map<CCharString, CCountedPointer<CConsoleInputBase> >).
// NF65 (round 7): the explicit instantiation on stlport/fable_map.h (NF58's form) plus the real CBasicString<char>::operator< 0x00429950
//   body defined in this TU under auto_inline(off) (fable_string.h declares it out of line), so VC7.1 sees it does not clobber the key.
// NF80 (q10763): that body now comes from fable_string.h (FABLE_RETAIL_CBASICSTRING_LESS_BODY: an inline, never-inlined COMDAT of the
//   one definition the self row 0x00429950 defines); this TU no longer carries its own copy.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_LESS_INLINE
#define FABLE_RETAIL_CBASICSTRING_LESS_BODY
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_DEC_OUTOFLINE
#include "stlport/fable_map.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
class CConsoleInputBase;
namespace std {
template pair<_Rb_tree<CCharString, pair<CCharString const, CCountedPointer<CConsoleInputBase> >, _Select1st<pair<CCharString const, CCountedPointer<CConsoleInputBase> > >, less<CCharString >, allocator<pair<CCharString const, CCountedPointer<CConsoleInputBase> > > >::iterator, bool> _Rb_tree<CCharString, pair<CCharString const, CCountedPointer<CConsoleInputBase> >, _Select1st<pair<CCharString const, CCountedPointer<CConsoleInputBase> > >, less<CCharString >, allocator<pair<CCharString const, CCountedPointer<CConsoleInputBase> > > >::insert_unique(pair<CCharString const, CCountedPointer<CConsoleInputBase> > const&);
}
