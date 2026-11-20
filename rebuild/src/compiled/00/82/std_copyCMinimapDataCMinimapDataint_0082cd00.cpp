// std::__copy(CMinimapData* first, CMinimapData* last, CMinimapData* result, std::random_access_iterator_tag const&, int*) -- STLport 4.6 _algobase.h: random-access counted forward copy `for (n = last - first; n > 0; --n) *result++ = *first++`, return the advanced result
// Leaf: ??$__copy@PAVCMinimapData@@PAV1@H@std@@YIPAVCMinimapData@@PAV1@00ABUrandom_access_iterator_tag@0@PAH@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x0082CD00, 127 bytes, /Gr `__fastcall` TU (first argument ecx, second edx, the rest on the stack), `ret 0xc`.
// Identity: caller 0082cee0 source (stdvectorCMinimapData_erase_0082cee0.cpp) names CMinimapData; binder ??$__copy@PAVCMinimapData@@PAV1@H@std@@YIPAVCMinimapData@@PAV1@00ABUrandom_access_iterator_tag@0@PAH@Z
// Body: the STLport 4.6 template from the shared model rebuild/include/stlport/fable_containers.h, explicitly instantiated over CMinimapData.
// Naming (NF142, 2026-11-20, q4659): the 8-byte member at +0x24 is CMinimapData::MinimapTexture (Ego_r.pdb CMinimapData: CTexture after the
//   CSurface MinimapSurface), so the SMember08_24 stand-in is replaced by engine/CTexture.h: its operator= 0x009FA1C0 binds Ego_r.pdb 0x00B6A110 +
//   FableWin.pdb 0x030D9F10 ??4CTexture@@QAEABV0@ABV0@@Z (const reference result). CMinimapData itself has no shared header (row-local model kept).
#define FABLE_RETAIL_CTEXTURE
#include "stlport/fable_containers.h"
#include "engine/CTexture.h"
struct SMember0C_8 {                             // 12 bytes, operator= @ 0x0082C980
    long m_words[3];
    SMember0C_8& operator=(const SMember0C_8& other);
};

struct SMember10_14 {                            // 16 bytes, operator= @ 0x009F2E70
    long m_words[4];
    SMember10_14& operator=(const SMember10_14& other);
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
    CTexture      m_texture;                     // +0x24 MinimapTexture
    long          m_value;                       // +0x2C
    SMember0C_30  m_tail;                        // +0x30
};
template CMinimapData* std::__copy<CMinimapData*,CMinimapData*,int >(CMinimapData*, CMinimapData*, CMinimapData*, std::random_access_iterator_tag const&, int*);
