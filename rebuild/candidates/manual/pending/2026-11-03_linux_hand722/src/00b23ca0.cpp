// CEngine::ProcessPrimitiveStats(const CPrimitiveStatMap& stats, EEnginePrimitiveStatIndex statIndex, unsigned long& total, CArray<CCharString>& report) const -- turn collected primitive statistics into a total and report lines (a CIEngine interface forwarder)
// Leaf: ?ProcessPrimitiveStats@CEngine@@UBEXABV?$map@KKU?$less@K@std@@V?$allocator@U?$pair@$$CBKK@std@@@2@@std@@W4EEnginePrimitiveStatIndex@@AAKAAV?$CArray@VCCharString@@@@@Z
// Flags: none
// Grade: RELOCATION_MATCH (q.py, cl 13.10.3077, no flags)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B23CA0, length 11 bytes: `mov ecx,[0x01436E80] ; jmp 0x00B33390`.
// Identity: slot 34 of the CEngine vtable 0x012A0F3C (RTTI CEngine : CIEngine) points here; Ego_r.pdb and FableWin.pdb both carry
//   ?ProcessPrimitiveStats@CEngine@@UBEXABV?$map@KKU?$less@K@std@@V?$allocator@U?$pair@$$CBKK@std@@@2@@std@@W4EEnginePrimitiveStatIndex@@AAKAAV?$CArray@VCCharString@@@@@Z (the virtual CIEngine method, `this` never read).
// Callee: CEnginePrimitiveRenderer::ProcessPrimitiveStats @ 0x00B33390, Ego_r + FableWin ?ProcessPrimitiveStats@CEnginePrimitiveRenderer@@QBEXABV?$map@KKU?$less@K@std@@V?$allocator@U?$pair@$$CBKK@std@@@2@@std@@W4EEnginePrimitiveStatIndex@@AAKAAV?$CArray@VCCharString@@@@@Z;
//   its `ret 10h` pops the same 4 stack arguments, so VC7.1 passes them through untouched and tail-jumps.
// Global: the engine component pointer 0x01436E80 (CEnginePrimitiveRenderer*) under its landed spelling g_pPrimitiveRenderer_01436e80; it has no PDB data symbol
//   (CEngine::AddEngineComponent<T>(T*, T*&, unsigned char) publishes each component into its own global).
// Shape: a virtual member of a local CEngine model: the shared engine/CEngine.h carries the vptr as a data member, so it cannot
//   declare the CIEngine virtual (same model as the landed siblings 0x00B239C0 / 0x00B23CB0 / 0x00B25D20).
class CCharString;
template <class T> class CArray;
enum EEnginePrimitiveStatIndex { };
namespace std {                                   // retail STLport 4.6 lives in std (EXTRA_RULES 2026-10-25)
template <class _Tp> struct less;
template <class _Tp> class allocator;
template <class _T1, class _T2> struct pair;
template <class _Key, class _Tp, class _Compare, class _Alloc> class map;
}
typedef std::map<unsigned long, unsigned long, std::less<unsigned long>, std::allocator<std::pair<const unsigned long, unsigned long> > > CPrimitiveStatMap;

class CEnginePrimitiveRenderer { public:
    void ProcessPrimitiveStats(const CPrimitiveStatMap& stats, EEnginePrimitiveStatIndex statIndex, unsigned long& total, CArray<CCharString>& report) const;   // 0x00B33390
};
extern CEnginePrimitiveRenderer* g_pPrimitiveRenderer_01436e80;   // 0x01436E80

class CEngine { public:                      // vtable 0x012A0F3C, slot 34
    virtual void ProcessPrimitiveStats(const CPrimitiveStatMap& stats, EEnginePrimitiveStatIndex statIndex, unsigned long& total, CArray<CCharString>& report) const;
};

void CEngine::ProcessPrimitiveStats(const CPrimitiveStatMap& stats, EEnginePrimitiveStatIndex statIndex, unsigned long& total, CArray<CCharString>& report) const
{
    g_pPrimitiveRenderer_01436e80->ProcessPrimitiveStats(stats, statIndex, total, report);
}
