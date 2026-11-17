// std::_Rb_tree_iterator::operator++() -- map<CCharString, CCountedPointer<CConsoleInputBase> > iterator step to the in-order successor (leftmost of the right subtree, else climb past right children; end() stays put) (STLport 4.6, _Nonconst_traits)
// Leaf: ??E?$_Rb_tree_iterator@U?$pair@$$CBVCCharString@@V?$CCountedPointer@VCConsoleInputBase@@@@@std@@U?$_Nonconst_traits@U?$pair@$$CBVCCharString@@V?$CCountedPointer@VCConsoleInputBase@@@@@std@@@2@@std@@QAEAAU01@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x009EE470, 62 bytes, thiscall on the iterator, returns it in eax.
// NF87 (2026-11-17): the catalog stand-in CRetailTreeIterator::Increment was a generated name shared by every byte-identical body of
//   this shape; retail has no identical-code folding, so this address is one instantiation. Its only retail E8 caller (0x009EB15C) is
//   in CConsole::NextCommandGuess 0x009EB0D0, which steps CConsole::GuessPos (Ego_r.pdb: the iterator of RegisteredConsoleInputs,
//   map<CCharString, CCountedPointer<CConsoleInputBase> >); NF87's 0x009EB0D0 row binds exactly this spelling.
// Shape: STLport 4.6 _tree.h `_Self& operator++() { _M_node = _Rb_global_inst::_M_increment(_M_node); return *this; }` with the
//   template static inlined, out of line per instance in retail (stlport/fable_map.h TREE_INC_OUTOFLINE), emitted here by explicit
//   instantiation.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_INC_OUTOFLINE
#include "fable_string.h"
#include "stlport/fable_map.h"
#include "engine/CCountedPointer.h"
class CConsoleInputBase;

typedef std::pair<const CCharString, CCountedPointer<CConsoleInputBase> > _ConsoleInputValue;
template std::_Rb_tree_iterator<_ConsoleInputValue, std::_Nonconst_traits<_ConsoleInputValue> >&
std::_Rb_tree_iterator<_ConsoleInputValue, std::_Nonconst_traits<_ConsoleInputValue> >::operator++();
