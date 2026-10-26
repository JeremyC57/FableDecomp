// CEngineSurfaceCompositionManager::GetGraphicInfo(unsigned long) -- resolve a graphic handle to its frame: handles with the top bit set index the manager's own composed-graphic table, the others the engine's global graphic bank; out-of-range handles read through a null record
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
class CGraphicInfoFrame;

struct SComposedGraphic {
    unsigned char      _pad[0x28];
    CGraphicInfoFrame* m_pFrame;                   // +0x28
};
struct SBankGraphic {
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

struct CGraphicBank {
    unsigned char                 _pad[0x1e0];
    CRetailVector<SBankGraphic>   m_records;       // +0x1e0
    SBankGraphic* Get(unsigned long index) { return index < m_records.size() ? &m_records[index] : 0; }
};

struct CEngineGlobal_01436e98 {
    unsigned char _pad[0x954];
    CGraphicBank* m_pGraphicBank;                  // +0x954
};
extern CEngineGlobal_01436e98* g_pEngine_01436e98;   // 0x01436E98

class CEngineSurfaceCompositionManager { public:
    unsigned char                   _pad[0x884];
    CRetailVector<SComposedGraphic> m_composedGraphics;   // +0x884
    SComposedGraphic* GetComposed(unsigned long index) { return index < m_composedGraphics.size() ? &m_composedGraphics[index] : 0; }
    CGraphicInfoFrame* GetGraphicInfo(unsigned long handle);
};

CGraphicInfoFrame* CEngineSurfaceCompositionManager::GetGraphicInfo(unsigned long handle)
{
    if (handle & 0x80000000)
        return GetComposed(handle & 0x7fffffff)->m_pFrame;
    return g_pEngine_01436e98->m_pGraphicBank->Get(handle)->m_pFrame;
}