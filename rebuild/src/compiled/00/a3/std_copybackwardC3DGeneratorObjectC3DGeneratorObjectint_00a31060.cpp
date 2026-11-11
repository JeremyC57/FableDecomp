// std::__copy_backward(C3DGeneratorObject* first, C3DGeneratorObject* last, C3DGeneratorObject* result, std::random_access_iterator_tag const&, int*) -- STLport 4.6 _algobase.h: random-access counted backward copy `for (n = last - first; n > 0; --n) *--result = *--last`, return the moved-back result
// Leaf: ??$__copy_backward@PAVC3DGeneratorObject@@PAV1@H@std@@YIPAVC3DGeneratorObject@@PAV1@00ABUrandom_access_iterator_tag@0@PAH@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A31060, 103 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 0xc`.
// Identity: caller 00a33f80 std::vector<C3DGeneratorObject, std::allocator<C3DGeneratorObject>>::_M_fill_insert; binder ??$__copy_backward@PAVC3DGeneratorObject@@PAV1@H@std@@YIPAVC3DGeneratorObject@@PAV1@00ABUrandom_access_iterator_tag@0@PAH@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over C3DGeneratorObject.
#define FABLE_RETAIL_STLP_RANDOM_ACCESS_COPY_BACKWARD
#include "stlport/fable_containers.h"
#include "engine/CMatrix3x4.h"
class CCharString {
public:
    CCharString(const CCharString& other);                       // 0x0099EC30 ??0CCharString@@QAE@ABV0@@Z
    ~CCharString();                                              // 0x0099EAE0 ??1CCharString@@QAE@XZ
    const CCharString& operator=(const CCharString& other);      // 0x0099EFB0 ??4CCharString@@QAEABV0@ABV0@@Z
    class CCharStringData* PStringData;                          // +0x00 (Ego_r.pdb, 4 bytes)
};

class C3DDummyObject { public:                      // Ego_r.pdb, 0x34 bytes
    CMatrix3x4 Transform;                    // +0x00
    long       BoneIndex;                    // +0x30
};
class C3DGeneratorObject {
public:                   // 0x40 bytes
    C3DDummyObject Object;                   // +0x00
    CCharString    Name;                     // +0x34
    long           Index;                    // +0x38
    bool           Flag;                     // +0x3C
};
template C3DGeneratorObject* std::__copy_backward<C3DGeneratorObject*,C3DGeneratorObject*,int >(C3DGeneratorObject*, C3DGeneratorObject*, C3DGeneratorObject*, std::random_access_iterator_tag const&, int*);
