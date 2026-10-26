// std::_Construct<std::pair<const CCharString, NBankFileManager::CContainedBank>, std::pair<const CCharString, NBankFileManager::CContainedBank> >(_T1* __p, const _T2& __val) -- STLport 4.6 placement copy-construct of one CRetailBank::ContainedBanks node value
// Leaf: ??$_Construct@U?$pair@$$CBVCCharString@@VCContainedBank@NBankFileManager@@@std@@U12@@std@@YIXPAU?$pair@$$CBVCCharString@@VCContainedBank@NBankFileManager@@@0@ABU10@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009AA520, length 53 bytes, __fastcall (destination in ecx, source in edx), `ret`.
// Identity: STLport 4.6 `_construct.h` `template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val)
//   { new (__p) _T1(__val); }` -- `__fastcall` as every free STLport template in this /Gr build. The value copied is
//   CCharString (copy constructor 0x0099EC30) + a 20-byte block: pair<const CCharString, NBankFileManager::CContainedBank>
//   (Ego_r CContainedBank {DataVersion, EntryCount, InfoOffset, InfoSize, Alignment}; CRetailBank::ContainedBanks is the
//   only Ego_r map with that value type), the same copy as that tree's _M_create_node 0x009AA9E0. Its twin 0x009AA560
//   (same shape, calling the CBankInfo pair copy constructor 0x009A9AD0) is the CBankFileManager::Banks instantiation.
//   No code references it (an emitted, unreferenced COMDAT). Was landed under the descriptive name
//   _global::Construct_PairCharStringContainedBank_009aa520 (the manifest label `_Cons_val<allocator<...>>` is Dinkumware).
// Shape: `new (p) T(v)` null-tests the placement result, then the implicit pair copy constructor: the CCharString copy
//   constructor, then the 20-byte CContainedBank copied word by word through both pointers re-based to +4
//   (`add edi,4 ; add esi,4`).
inline void* operator new(unsigned int, void* where) { return where; }

class CCharString {
public:
    void* PStringData;                                      // +0x00 (CCharStringData*)
    CCharString(const CCharString& other);                  // 0x0099EC30
};

namespace NBankFileManager {
class CContainedBank {                                      // 0x14 bytes (Ego_r.pdb NBankFileManager::CContainedBank)
public:
    unsigned long DataVersion;                              // +0x00
    unsigned long EntryCount;                               // +0x04
    unsigned long InfoOffset;                               // +0x08
    unsigned long InfoSize;                                 // +0x0C
    unsigned long Alignment;                                // +0x10
};
}

namespace std {
template <class _T1, class _T2> struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;                                              // +0x00
    _T2 second;                                             // +0x04
};

template <class _T1, class _T2>
inline void __fastcall _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
}

typedef std::pair<const CCharString, NBankFileManager::CContainedBank> _TreeValue;
template void __fastcall std::_Construct<_TreeValue, _TreeValue>(_TreeValue*, const _TreeValue&);
