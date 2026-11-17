#pragma optimize("s",on)
// std::_Rb_tree<CWideString, pair<const CWideString, bool>, _Select1st, less<CWideString>, allocator>::~_Rb_tree() -- STLport tree destructor of map<CWideString, bool>: clear() (out of line @ 0x00485AEF), then ~_Rb_tree_base frees the header node
// Retail: Fable.exe (TLC, VC7.1) @ 0x00485E45, 23 bytes (size TU: `mov esi,ecx ; call clear ; mov eax,[esi] ; test ; push ; call free ; pop ecx`).
// Was the stand-in global "CActiveFile_OnReadFinished" (catalog label CFileInstaller::CActiveFile::OnReadFinished, propagated).
// NF50 (round 7): identity -- called only from the destructors of NUISystem::CFrontEndManager 0x005954AD (Ego_r.pdb Signatures =
// map<CWideString, bool>) and CTCInGameMenu 0x0047E07F; STLport 4.6 map declares no destructor, so its implicit one is expanded in
// the owner and the COMDAT it calls is the tree's (the landed FABLE_RETAIL_STLP_TREE_CTOR_DTOR_OUTOFLINE precedent, set<EWeaponClass>
// ~_Rb_tree 0x00451B07; this TU's own compile of the map destructor under optimize("s") emits the same out-of-line ~_Rb_tree call).
// Body: ~_Rb_tree() { clear(); } with ~_Rb_tree_base inlined. The clear call binds the STLport
// ?clear@?$_Rb_tree@VCWideString@@U?$pair@$$CBVCWideString@@_N@std@@U?$_Select1st@U?$pair@$$CBVCWideString@@_N@std@@@3@U?$less@VCWideString@@@3@V?$allocator@U?$pair@$$CBVCWideString@@_N@std@@@3@@std@@QAEXXZ
// (nf41 e117/e217; FABLE_RETAIL_STLP_TREE_CLEAR_OUTOFLINE, declaration only, as the landed 0x004804DE / 0x004830A1).
// Leaf: ??1?$_Rb_tree@VCWideString@@U?$pair@$$CBVCWideString@@_N@std@@U?$_Select1st@U?$pair@$$CBVCWideString@@_N@std@@@3@U?$less@VCWideString@@@3@V?$allocator@U?$pair@$$CBVCWideString@@_N@std@@@3@@std@@QAE@XZ
#define FABLE_RETAIL_STLP_TREE_CLEAR_OUTOFLINE
#include "fable_string.h"
#include "stlport/fable_map.h"

namespace std {
template _Rb_tree<CWideString, pair<const CWideString, bool>, _Select1st<pair<const CWideString, bool> >, less<CWideString>, allocator<pair<const CWideString, bool> > >::~_Rb_tree();
}
