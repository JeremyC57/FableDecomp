// std::__copy(CMinimapData* first, CMinimapData* last, CMinimapData* result, std::random_access_iterator_tag const&, int*) -- STLport 4.6 _algobase.h: random-access counted forward copy `for (n = last - first; n > 0; --n) *result++ = *first++`, return the advanced result
// Leaf: ??$__copy@PAVCMinimapData@@PAV1@H@std@@YIPAVCMinimapData@@PAV1@00ABUrandom_access_iterator_tag@0@PAH@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x0082CD00, 127 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 0xc`.
// Identity: caller 0082cee0 source (stdvectorCMinimapData_erase_0082cee0.cpp) names CMinimapData; binder ??$__copy@PAVCMinimapData@@PAV1@H@std@@YIPAVCMinimapData@@PAV1@00ABUrandom_access_iterator_tag@0@PAH@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CMinimapData.
#include "stlport/fable_containers.h"
struct SMember0C_8 {                             // 12 bytes, operator= @ 0x0082C980
    long m_words[3];
    SMember0C_8& operator=(const SMember0C_8& other);
};

struct SMember10_14 {                            // 16 bytes, operator= @ 0x009F2E70
    long m_words[4];
    SMember10_14& operator=(const SMember10_14& other);
};

struct SMember08_24 {                            // 8 bytes, operator= @ 0x009FA1C0
    long m_words[2];
    SMember08_24& operator=(const SMember08_24& other);
};

struct SMember0C_30 {                            // 12 bytes, operator= @ 0x0045BAAA
    long m_words[3];
    SMember0C_30& operator=(const SMember0C_30& other);
};
class CMinimapData {
public:                       // 60 bytes
    unsigned long m_key0;                        // +0x00
    unsigned long m_key4;                        // +0x04
    SMember0C_8   m_list;                        // +0x08
    SMember10_14  m_surface;                     // +0x14
    SMember08_24  m_texture;                     // +0x24
    long          m_value;                       // +0x2C
    SMember0C_30  m_tail;                        // +0x30
};
template CMinimapData* std::__copy<CMinimapData*,CMinimapData*,int >(CMinimapData*, CMinimapData*, CMinimapData*, std::random_access_iterator_tag const&, int*);
