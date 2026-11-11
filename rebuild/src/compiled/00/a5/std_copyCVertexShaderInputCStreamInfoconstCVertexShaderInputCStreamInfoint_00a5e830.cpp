// std::__copy(CVertexShaderInput::CStreamInfo const* first, CVertexShaderInput::CStreamInfo const* last, CVertexShaderInput::CStreamInfo* result, std::random_access_iterator_tag const&, int*) -- STLport 4.6 _algobase.h: random-access counted forward copy `for (n = last - first; n > 0; --n) *result++ = *first++`, return the advanced result
// Leaf: ??$__copy@PBVCStreamInfo@CVertexShaderInput@@PAV12@H@std@@YIPAVCStreamInfo@CVertexShaderInput@@PBV12@0PAV12@ABUrandom_access_iterator_tag@0@PAH@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A5E830, 75 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 0xc`.
// Identity: caller 00a5e9c0 std::vector<CVertexShaderInput::CStreamInfo, std::allocator<CVertexShaderInput::CStreamInfo>>::operator=; caller 00a5e9c0 std::vector<CVertexShaderInput::CStreamInfo, std::allocator<CVertexShaderInput::CStreamInfo>>::operator=; binder ??$__copy@PBVCStreamInfo@CVertexShaderInput@@PAV12@H@std@@YIPAVCStreamInfo@CVertexShaderInput@@PBV12@0PAV12@ABUrandom_access_iterator_tag@0@PAH@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CVertexShaderInput::CStreamInfo.
#include "stlport/fable_containers.h"
struct SVector {                             // 12-byte STL vector (first, last, end of storage)
    void* m_pFirst;                          // +0x00
    void* m_pLast;                           // +0x04
    void* m_pEnd;                            // +0x08
    SVector& operator=(const SVector& other);              // 0x00A5E4D0
};

class CVectorHolder {                        // 0x10 bytes
public:
    virtual void v00();                      // +0x00 vtable
    SVector Items;                           // +0x04
};
class CVertexShaderInput {
public:
class CStreamInfo {
public:               // 0x14 bytes
    long          first;                     // +0x00
    CVectorHolder second;                    // +0x04
};
};
template CVertexShaderInput::CStreamInfo* std::__copy<CVertexShaderInput::CStreamInfo const*,CVertexShaderInput::CStreamInfo*,int >(CVertexShaderInput::CStreamInfo const*, CVertexShaderInput::CStreamInfo const*, CVertexShaderInput::CStreamInfo*, std::random_access_iterator_tag const&, int*);
