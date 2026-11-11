// std::__uninitialized_copy(CEngineTexturePalette::CPaletteEntry* first, CEngineTexturePalette::CPaletteEntry* last, CEngineTexturePalette::CPaletteEntry* result, std::__false_type const&) -- STLport 4.6 _uninitialized.h: copy-construct [first, last) into raw storage at result (placement new per element), return the end
// Leaf: ??$__uninitialized_copy@PAUCPaletteEntry@CEngineTexturePalette@@PAU12@@std@@YIPAUCPaletteEntry@CEngineTexturePalette@@PAU12@00ABU__false_type@0@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BF8F30, 54 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 8`.
// Identity: caller 00bf94d0 std::vector<CEngineTexturePalette::CPaletteEntry>::_M_insert_overflow; caller 00bf94d0 std::vector<CEngineTexturePalette::CPaletteEntry>::_M_insert_overflow; caller 00bf95d0 std::vector<CEngineTexturePalette::CPaletteEntry, std::allocator<CEngineTexturePalette::CPaletteEntry>>::_M_fill_insert; binder ??$__uninitialized_copy@PAUCPaletteEntry@CEngineTexturePalette@@PAU12@@std@@YIPAUCPaletteEntry@CEngineTexturePalette@@PAU12@00ABU__false_type@0@@Z
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
template CEngineTexturePalette::CPaletteEntry* std::__uninitialized_copy<CEngineTexturePalette::CPaletteEntry*,CEngineTexturePalette::CPaletteEntry* >(CEngineTexturePalette::CPaletteEntry*, CEngineTexturePalette::CPaletteEntry*, CEngineTexturePalette::CPaletteEntry*, std::__false_type const&);
