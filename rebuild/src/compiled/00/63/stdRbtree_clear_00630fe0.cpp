// std::_Rb_tree<unsigned long, pair<const unsigned long, CCharString>, _Select1st, less<unsigned long>, allocator>::clear() -- STLport tree clear of map<unsigned long, CCharString>: when it holds nodes, _M_erase(root) (out of line @ 0x00630D90), re-link the header to itself, zero the count
// Retail: Fable.exe (TLC, VC7.1) @ 0x00630FE0, 51 bytes (`mov esi,ecx ; cmp [esi+4],0 ; je ; mov eax,[esi] ; push [eax+4] ; call _M_erase ; header left/parent/right re-read per store ; count = 0`).
// Was the stand-in "CTCCarryable_OnKill" (__fastcall; the catalog label CTCCarryable::OnKill is propagated: the real OnKill is 0x0047C880).
// NF50 (round 7): identity -- the body is STLport 4.6 _tree.h _Rb_tree::clear() verbatim (left, parent = 0, right, count = 0) and
// its one caller is GFSerialiseMapBinaryIn<CCharString, unsigned long, less<unsigned long> > 0x00632B90 (map::clear inlined there),
// over the tree whose _M_erase is 0x00630D90 (nf44 entry 55; Ego_r.pdb CUIDef::ActionMap = map<unsigned long, CCharString>).
// Explicit instantiation of the shared stlport/fable_map.h clear; the _M_erase call binds the STLport symbol (descriptive-by-library).
// Leaf: ?clear@?$_Rb_tree@KU?$pair@$$CBKVCCharString@@@std@@U?$_Select1st@U?$pair@$$CBKVCCharString@@@std@@@2@U?$less@K@2@V?$allocator@U?$pair@$$CBKVCCharString@@@std@@@2@@std@@QAEXXZ
#include "fable_string.h"
#include "stlport/fable_map.h"
namespace std {
template void _Rb_tree<unsigned long, pair<const unsigned long, CCharString>, _Select1st<pair<const unsigned long, CCharString> >, less<unsigned long>, allocator<pair<const unsigned long, CCharString> > >::clear();
}
