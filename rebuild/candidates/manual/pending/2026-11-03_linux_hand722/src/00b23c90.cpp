// CEngine::CollectPrimitiveStatsForArea(CPrimitiveStatMap& stats, const C2DBoxF& area, EEnginePrimitiveStatIndex statIndex, unsigned long flags) const -- collect primitive statistics for an area of the world (a CIEngine interface forwarder)
// Leaf: ?CollectPrimitiveStatsForArea@CEngine@@UBEXAAV?$map@KKU?$less@K@std@@V?$allocator@U?$pair@$$CBKK@std@@@2@@std@@ABVC2DBoxF@@W4EEnginePrimitiveStatIndex@@K@Z
// Flags: none
// Grade: RELOCATION_MATCH (q.py, cl 13.10.3077, no flags)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B23C90, length 11 bytes: `mov ecx,[0x01436E80] ; jmp 0x00B321B0`.
// Identity: slot 33 of the CEngine vtable 0x012A0F3C (RTTI CEngine : CIEngine) points here; Ego_r.pdb and FableWin.pdb both carry
//   ?CollectPrimitiveStatsForArea@CEngine@@UBEXAAV?$map@KKU?$less@K@std@@V?$allocator@U?$pair@$$CBKK@std@@@2@@std@@ABVC2DBoxF@@W4EEnginePrimitiveStatIndex@@K@Z (the virtual CIEngine method, `this` never read).
// Callee: CEnginePrimitiveRenderer::CollectPrimitiveStatsForArea @ 0x00B321B0, Ego_r + FableWin ?CollectPrimitiveStatsForArea@CEnginePrimitiveRenderer@@QBEXAAV?$map@KKU?$less@K@std@@V?$allocator@U?$pair@$$CBKK@std@@@2@@std@@ABVC2DBoxF@@W4EEnginePrimitiveStatIndex@@K@Z;
//   its `ret 10h` pops the same 4 stack arguments, so VC7.1 passes them through untouched and tail-jumps.
// Global: the engine component pointer 0x01436E80 (CEnginePrimitiveRenderer*) under its landed spelling g_pPrimitiveRenderer_01436e80; it has no PDB data symbol
//   (CEngine::AddEngineComponent<T>(T*, T*&, unsigned char) publishes each component into its own global).
// Shape: a virtual member of a local CEngine model: the shared engine/CEngine.h carries the vptr as a data member, so it cannot
//   declare the CIEngine virtual (same model as the landed siblings 0x00B239C0 / 0x00B23CB0 / 0x00B25D20).
class C2DBoxF;
enum EEnginePrimitiveStatIndex { };
namespace std {                                   // retail STLport 4.6 lives in std (EXTRA_RULES 2026-10-25)
template <class _Tp> struct less;
template <class _Tp> class allocator;
template <class _T1, class _T2> struct pair;
template <class _Key, class _Tp, class _Compare, class _Alloc> class map;
}
typedef std::map<unsigned long, unsigned long, std::less<unsigned long>, std::allocator<std::pair<const unsigned long, unsigned long> > > CPrimitiveStatMap;

class CEnginePrimitiveRenderer { public:
    void CollectPrimitiveStatsForArea(CPrimitiveStatMap& stats, const C2DBoxF& area, EEnginePrimitiveStatIndex statIndex, unsigned long flags) const;   // 0x00B321B0
};
extern CEnginePrimitiveRenderer* g_pPrimitiveRenderer_01436e80;   // 0x01436E80

class CEngine { public:                      // vtable 0x012A0F3C, slot 33
    virtual void CollectPrimitiveStatsForArea(CPrimitiveStatMap& stats, const C2DBoxF& area, EEnginePrimitiveStatIndex statIndex, unsigned long flags) const;
};

void CEngine::CollectPrimitiveStatsForArea(CPrimitiveStatMap& stats, const C2DBoxF& area, EEnginePrimitiveStatIndex statIndex, unsigned long flags) const
{
    g_pPrimitiveRenderer_01436e80->CollectPrimitiveStatsForArea(stats, area, statIndex, flags);
}
