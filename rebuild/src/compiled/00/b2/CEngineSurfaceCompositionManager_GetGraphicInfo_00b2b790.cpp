// Leaf: GetGraphicInfo
// CEngineSurfaceCompositionManager::GetGraphicInfo(unsigned long) (private, const CGraphicInfoFrame* result) -- resolve a graphic handle to its frame: handles with the top bit set index the manager's own composed-graphic table, the others the engine's global graphic bank; out-of-range handles read through a null record
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B2B790, length 137 bytes (+ int3 padding). No callees.
// Globals: the engine @ 0x01436E98 (graphic bank pointer at +0x954, its 0x2c-byte record vector at +0x1e0).
// Layout: the manager's own 0x2c-byte record vector sits at +0x884 in retail (engine/CEngineSurfaceCompositionManager.h
// has no such member at that offset: the PDB containers are wider), so `this` is modelled locally; each record holds
// its frame pointer at +0x28.
// Shape: `handle & 0x80000000` gives `test ; jns`; each table is read through its OWN inline getter
// (`index < size() ? &v[index] : 0`, STLport `size()` = `end() - begin()` by the 0x2E8BA2E9 reciprocal, `operator[]` =
// `*(begin() + n)`) over a vector of a DIFFERENT record type, so VC7.1 keeps two arms (one shared getter merges them into
// one block); the +0x28 read is duplicated into each found arm and the shared null arm (`xor esi,esi`); the reciprocal
// is hoisted above the branch; the bank arm re-reads begin() after the division.
// NF46 (round 7): the 0x01436E98 stand-in CEngineGlobal_01436e98 / g_pEngine_01436e98 is NEngineInternal::GPResourceManager
//   (?GPResourceManager@NEngineInternal@@3PAVCEngineResourceManager@@A) on engine/CEngineResourceManager.h; +0x954 is GetGraphicBank (_GETGRAPHICBANK,
//   as same-TU 0x00B2D9F0), and the bank's local CGraphicBank / SBankGraphic record model is the header CGraphicDataBank / CGraphicInfo
//   (GraphicList +0x1E0, Frames +0x28): FableWin.pdb inline PeekGraphic (as landed 0x00BB0970) and PeekInfoFrame(0). The manager's own table stays local.
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#define FABLE_RETAIL_CENGINERESOURCEMANAGER_GETGRAPHICBANK
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RESET_ADOPT
#define FABLE_RETAIL_CMOVABLERESOURCE
#define FABLE_RETAIL_CRESOURCE_TOUCHRESOURCE
#define FABLE_RETAIL_CGRAPHICFRAME
#define FABLE_RETAIL_CGRAPHICFRAME_CTOR
#define FABLE_RETAIL_CGRAPHICFRAME_GETTEXTURE
#define FABLE_RETAIL_CGRAPHICFRAME_DTOR_FORCEINLINE
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CEngineResourceManager.h"
#include "engine/CGraphicDataBank.h"
#include "engine/CGraphicInfo.h"

struct SComposedGraphic {
    unsigned char      _pad[0x28];
    CGraphicInfoFrame* m_pFrame;                   // +0x28
};

template <class T> struct CRetailVector {
    T* m_pFirst;
    T* m_pLast;
    T* m_pEnd;
    T* begin() { return m_pFirst; }
    T* end() { return m_pLast; }
    unsigned long size() { return end() - begin(); }
    T& operator[](unsigned long n) { return *(begin() + n); }
};

namespace NEngineInternal { extern CEngineResourceManager* GPResourceManager; }   // 0x01436E98
inline const CGraphicInfo* CGraphicDataBank::PeekGraphic(unsigned long index) const
{
    return index < GraphicList.size() ? &GraphicList[index] : 0;
}

class CEngineSurfaceCompositionManager { public:
    unsigned char                   _pad[0x884];
    CRetailVector<SComposedGraphic> m_composedGraphics;   // +0x884
    SComposedGraphic* GetComposed(unsigned long index) { return index < m_composedGraphics.size() ? &m_composedGraphics[index] : 0; }
private:
    const CGraphicInfoFrame* GetGraphicInfo(unsigned long handle);
};

const CGraphicInfoFrame* CEngineSurfaceCompositionManager::GetGraphicInfo(unsigned long handle)
{
    if (handle & 0x80000000)
        return GetComposed(handle & 0x7fffffff)->m_pFrame;
    return NEngineInternal::GPResourceManager->GetGraphicBank((EGraphicBankNo)0)->PeekGraphic(handle)->PeekInfoFrame(0);
}