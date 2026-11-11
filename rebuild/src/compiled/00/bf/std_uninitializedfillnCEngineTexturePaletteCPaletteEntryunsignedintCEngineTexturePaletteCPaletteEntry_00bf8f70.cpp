// std::__uninitialized_fill_n(CEngineTexturePalette::CPaletteEntry* first, unsigned int n, CEngineTexturePalette::CPaletteEntry const& x, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct n copies of x into raw storage at first, return the end
// Leaf: ??$__uninitialized_fill_n@PAUCPaletteEntry@CEngineTexturePalette@@IU12@@std@@YIPAUCPaletteEntry@CEngineTexturePalette@@PAU12@IABU12@ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BF8F70, 50 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 00bf94d0 std::vector<CEngineTexturePalette::CPaletteEntry>::_M_insert_overflow; binder ??$__uninitialized_fill_n@PAUCPaletteEntry@CEngineTexturePalette@@IU12@@std@@YIPAUCPaletteEntry@CEngineTexturePalette@@PAU12@IABU12@ABU__false_type@0@@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CEngineTexturePalette::CPaletteEntry.
#include "stlport/fable_containers.h"
class CCharString {
public:
    CCharString(const CCharString& other);                       // 0x0099EC30 ??0CCharString@@QAE@ABV0@@Z
    ~CCharString();                                              // 0x0099EAE0 ??1CCharString@@QAE@XZ
    const CCharString& operator=(const CCharString& other);      // 0x0099EFB0 ??4CCharString@@QAEABV0@ABV0@@Z
    class CCharStringData* PStringData;                          // +0x00 (Ego_r.pdb, 4 bytes)
};

class CEngineTexturePalette {
public:
struct CPaletteEntry {
    long first;
    CCharString second;
};
};
template CEngineTexturePalette::CPaletteEntry* std::__uninitialized_fill_n<CEngineTexturePalette::CPaletteEntry*,unsigned int,CEngineTexturePalette::CPaletteEntry >(CEngineTexturePalette::CPaletteEntry*, unsigned int, CEngineTexturePalette::CPaletteEntry const&, std::__false_type const&);
