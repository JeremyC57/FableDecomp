# h16_rberase.py: naming wave 16, family stl -- 0x009AA780 _M_erase: the landed STLport 4.6 _Rb_tree text kept verbatim, its
# per-instantiation placeholders (CUnprovenKey/Value/KeyOfValue_009aa780) replaced by the proven instantiation
# map<CCharString, NBankFileManager::CContainedBank> (~CRetailBank 0x009AC7C0 on ContainedBanks +0x18; Ego_r CRetailBank member type).
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from he import edit
A = '009aa780'
edit(A, 'src', '''// std::_Rb_tree<CUnprovenKey_009aa780, CUnprovenValue_009aa780, CUnprovenKeyOfValue_009aa780, std::less<CUnprovenKey_009aa780>, std::allocator<CUnprovenValue_009aa780>>::_M_erase(std::_Rb_tree_node<CUnprovenValue_009aa780> *) -- STLport 4.6 _M_erase(_Link_type): erase without rebalancing -- recurse right, destroy the value, free the node, continue left
// Retail: Fable.exe (TLC, VC7.1) @ 0x009AA780, 59 bytes.
// Types: unproven -- no retail evidence names this instantiation's key/value, so they are the per-instantiation
//   placeholders CUnprovenKey/Value/KeyOfValue_009aa780; only the value members the erase bodies destroy are proven
//   (lea ecx,[node+0x10+off]; call ~T): CCharString @+0x0, value 0x18 bytes (node 0x28).
''', '''// std::_Rb_tree<CCharString, std::pair<const CCharString, NBankFileManager::CContainedBank>, std::_Select1st<...>, std::less<CCharString>,
//   std::allocator<...> >::_M_erase(_Rb_tree_node<...>*) -- STLport 4.6 _M_erase(_Link_type): erase without rebalancing -- recurse right,
//   destroy the value, free the node, continue left
// Retail: Fable.exe (TLC, VC7.1) @ 0x009AA780, 59 bytes.
// Identity (proven, chunk c667): ~CRetailBank 0x009AC7C0 calls it on ContainedBanks +0x18, Ego_r.pdb map<CCharString,
//   NBankFileManager::CContainedBank> (the class's only tree); the 0x28-byte node = 0x10 header + pair<const CCharString,
//   CContainedBank> (4 + 0x14), the value's only destructor is the key's ~CCharString (lea ecx,[node+0x10]).
// Naming wave 16: the per-instantiation placeholders CUnprovenKey/Value/KeyOfValue_009aa780 replaced by the proven types
//   (name-only: the bytes are unchanged); CContainedBank as the landed ~CRetailBank 0x009AC7C0 row models it.
''')
edit(A, 'src', '''struct CUnprovenKey_009aa780 { CCharString _M_unproven; };
struct CUnprovenValue_009aa780 { CUnprovenKey_009aa780 _M_key; unsigned char _M_unproven4[20]; };
struct CUnprovenKeyOfValue_009aa780 { const CUnprovenKey_009aa780& operator()(const CUnprovenValue_009aa780& __v) const { return __v._M_key; } };
''', '''namespace NBankFileManager {
class CContainedBank {
public:
    unsigned long DataVersion;                          // +0x00
    unsigned long EntryCount;                           // +0x04
    unsigned long InfoOffset;                           // +0x08
    unsigned long InfoSize;                             // +0x0C
    unsigned long Alignment;                            // +0x10
};
} // namespace NBankFileManager
''')
edit(A, 'src', '// Leaf: ?_M_erase@?$_Rb_tree@UCUnprovenKey_009aa780@@UCUnprovenValue_009aa780@@UCUnprovenKeyOfValue_009aa780@@U?$less@UCUnprovenKey_009aa780@@@std@@V?$allocator@UCUnprovenValue_009aa780@@@5@@std@@AAEXPAU?$_Rb_tree_node@UCUnprovenValue_009aa780@@@2@@Z',
     '// Leaf: ?_M_erase@?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@VCContainedBank@NBankFileManager@@@std@@U?$_Select1st@U?$pair@$$CBVCCharString@@VCContainedBank@NBankFileManager@@@std@@@3@U?$less@VCCharString@@@3@V?$allocator@U?$pair@$$CBVCCharString@@VCContainedBank@NBankFileManager@@@std@@@3@@std@@AAEXPAU?$_Rb_tree_node@U?$pair@$$CBVCCharString@@VCContainedBank@NBankFileManager@@@std@@@2@@Z')
edit(A, 'src', 'template void _Rb_tree<CUnprovenKey_009aa780, CUnprovenValue_009aa780, CUnprovenKeyOfValue_009aa780, less<CUnprovenKey_009aa780 >, allocator<CUnprovenValue_009aa780 > >::_M_erase(_Rb_tree_node<CUnprovenValue_009aa780 >*);',
     '''typedef pair<const CCharString, NBankFileManager::CContainedBank> _ContainedBankPair;
template void _Rb_tree<CCharString, _ContainedBankPair, _Select1st<_ContainedBankPair>, less<CCharString>, allocator<_ContainedBankPair> >::_M_erase(_Rb_tree_node<_ContainedBankPair>*);''')
