// NPlayerGui::CDrawCombatMultiplier::CDrawCombatMultiplier(CPlayerGui& gui, std::vector<NPlayerGui::CDrawBase*>& drawList) -- CDrawBase(gui, drawList), own vtable, then 13 primitive handles reserved
// Leaf: ??0CDrawCombatMultiplier@NPlayerGui@@QAE@AAVCPlayerGui@@AAV?$vector@PAVCDrawBase@NPlayerGui@@V?$allocator@PAVCDrawBase@NPlayerGui@@@std@@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0064E2A6, 38 bytes (unaligned: a size-optimised TU), thiscall `ret 8`.
// Identity (proven): NPlayerGui::CDrawCombatMultiplier::CDrawCombatMultiplier; evidence: after the NPlayerGui::CDrawBase constructor 0x0064CC65 it stores vptr 0x0125A624, RTTI .?AVCDrawCombatMultiplier@NPlayerGui@@ (so it is that class's constructor); the PDB name sets hold ??0CDrawCombatMultiplier@NPlayerGui@@QAE@AAVCPlayerGui@@AAV?$vector@PAVCDrawBase@NPlayerGui@@V?$allocator@PAVCDrawBase@NPlayerGui@@@std@@@std@@@Z; its only caller is the CPlayerGui constructor 0x0043B570, which builds every draw element of the HUD list; it resizes the PrimitiveHandles vector (+0x1C) to 13.
// Label: the manifest's "CDrawBettingHUD" is propagated (family H090: six NPlayerGui draw-element constructors that only resize the
//   handle vector, each its own function).
// Callees: NPlayerGui::CDrawBase::CDrawBase(CPlayerGui&, vector<CDrawBase*>&) 0x0064CC65 (the pass-through argument pushes of a size
//   TU), std::vector<CEnginePrimitiveHandle>::resize(size_type) 0x00443F90 (STLport `resize(n, CEnginePrimitiveHandle())` out of line).
// Layout: Ego_r.pdb NPlayerGui::CDrawBase: PlayerGui +4, PlayerManager +8, World +0xC, Component +0x10, Bank +0x14, Drawing +0x18,
//   PrimitiveHandles +0x1C (12 bytes in retail); CDrawCombatMultiplier adds no member the constructor touches. Local classes -- Review at
//   landing (the landed CDrawHealth 0x0064E1E4 models these with Dinkumware-style local vectors and pointer parameters).
#pragma optimize("s", on)
class CPlayerGui;
class CEnginePrimitiveHandle {                                // Ego_r.pdb: vptr + handle
public:
    virtual void _v0();
    long Handle;
};
namespace std {
template <class _Tp> class allocator { };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    _Tp* _M_start;                                            // +0x00
    _Tp* _M_finish;                                           // +0x04
    _Tp* _M_end_of_storage;                                   // +0x08
    void resize(unsigned int __new_size);                     // out of line
};
}
namespace NPlayerGui {
class CDrawBase {
public:
    CDrawBase(CPlayerGui& gui, std::vector<CDrawBase*>& drawList);   // 0x0064CC65
    virtual void Draw();
    CPlayerGui*   PlayerGui;                                  // +0x04
    void*         PlayerManager;                              // +0x08
    void*         World;                                      // +0x0C
    void*         Component;                                  // +0x10
    void*         Bank;                                       // +0x14
    bool          Drawing;                                    // +0x18
    std::vector<CEnginePrimitiveHandle> PrimitiveHandles;     // +0x1C
};
class CDrawCombatMultiplier : public CDrawBase {                            // vtable 0x0125A624
public:
    CDrawCombatMultiplier(CPlayerGui& gui, std::vector<CDrawBase*>& drawList);
    virtual void Draw();
};
CDrawCombatMultiplier::CDrawCombatMultiplier(CPlayerGui& gui, std::vector<CDrawBase*>& drawList)
    : CDrawBase(gui, drawList)
{
    PrimitiveHandles.resize(13);
}
}