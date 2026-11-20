// std::_List_base<std::pair<long, EConversationAttitude> >::~_List_base() -- STLport list base destructor: clear the nodes (clear
//   inlined) then free the sentinel node
// Leaf: ??1?$_List_base@U?$pair@JW4EConversationAttitude@@@std@@V?$allocator@U?$pair@JW4EConversationAttitude@@@std@@@2@@std@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00718EC0, 62 bytes: the inlined _List_base::clear loop (`mov esi,[esi] ; push eax ; call free`
//   per node, then the two sentinel self-links), then `mov edi,[edi] ; test edi,edi ; je ; push edi ; call free`.
// Identity (descriptive-by-library: STLport 4.6; both PDBs are Dinkumware builds and carry no list of this pair): the only landed
//   caller, CTCScriptedControl::~CTCScriptedControl 0x00719C70, destroys its AttitudeTags member (Ego_r struct_layouts
//   CTCScriptedControl::AttitudeTags, list<pair<long, EConversationAttitude> >) through it and binds
//   ??1?$_List_base@U?$pair@JW4EConversationAttitude@@@std@@...@std@@QAE@XZ; the ~list thunk 0x007191A0 jumps here. Retail has no
//   other reference to this address (Fable.exe scan). STLport 4.6's ~list is empty and inline; the clear + deallocate body is
//   ~_List_base's (_list.h), as the landed 0x0042AB73 / 0x005792DA instances.
// NF118 (PRIORITY q4018): re-authored on the shared STLport text (explicit destructor instantiation, clear visible). Was the old
//   free-function stand-in SortTreeRecursively_00436900 (`CGuiControlTreePane::SortTreeRecursively`, label StdList_Clear_00718ec0).
#define FABLE_RETAIL_STLP_LIST
#include "stlport/fable_containers.h"
enum EConversationAttitude;                                // MS forward enum (Ego_r: U?$pair@JW4EConversationAttitude@@@std@@; engine/CTCTalk.h)
template std::_List_base<std::pair<long, EConversationAttitude>, std::allocator<std::pair<long, EConversationAttitude> > >::~_List_base();
