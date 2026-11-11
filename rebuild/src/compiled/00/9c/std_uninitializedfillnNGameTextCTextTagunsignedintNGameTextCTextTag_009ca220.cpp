// std::uninitialized_fill_n(NGameText::CTextTag* first, unsigned int n, NGameText::CTextTag const& x) -- STLport 4.6 _uninitialized.h: copy-construct n copies of x into raw storage at first (the __false_type helper inlined), return the end
// Leaf: ??$uninitialized_fill_n@PAVCTextTag@NGameText@@IV12@@std@@YIPAVCTextTag@NGameText@@PAV12@IABV12@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x009CA220, 50 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 4`.
// Identity: caller 009caf60 std::vector<NGameText::CTextTag, std::allocator<NGameText::CTextTag>>::_M_fill_insert; binder ??$uninitialized_fill_n@PAVCTextTag@NGameText@@IV12@@std@@YIPAVCTextTag@NGameText@@PAV12@IABV12@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over NGameText::CTextTag.
#include "stlport/fable_containers.h"
class CCharString {
public:
    CCharString(const CCharString& other);                       // 0x0099EC30 ??0CCharString@@QAE@ABV0@@Z
    ~CCharString();                                              // 0x0099EAE0 ??1CCharString@@QAE@XZ
    const CCharString& operator=(const CCharString& other);      // 0x0099EFB0 ??4CCharString@@QAEABV0@ABV0@@Z
    class CCharStringData* PStringData;                          // +0x00 (Ego_r.pdb, 4 bytes)
};

namespace NGameText {
class CTextTag {
public:
    long first;
    CCharString second;
};
}
template NGameText::CTextTag* std::uninitialized_fill_n<NGameText::CTextTag*,unsigned int,NGameText::CTextTag >(NGameText::CTextTag*, unsigned int, NGameText::CTextTag const&);
