// CCountedPointer<NPlayerGui::CDrawMeleeActionButtonEffects>::DeleteData(void*) -- the per-type deleter a counted pointer stores in its count block:
// destroy the object (non-virtual destructor) and free it, null-guarded (`delete (T*)data`).
// Leaf: ?DeleteData@?$CCountedPointer@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00443aa0, 23 bytes, static __fastcall (data in ecx), plain `ret`.
// Renamed (rename1, 2026-10-24) from _global::DeleteObject_NPlayerGui__CDrawMeleeActionButtonEffect_00443aa0 (symbol OnDie) @ 00/44/CTCMapwho_OnDie_00443aa0.cpp:
//   the PDB (Ego_r) name set has `?DeleteData@?$CCountedPointer@VCDrawMeleeActionButtonEffects@NPlayerGui@@@@SIXPAX@Z` (public static void __fastcall
//   DeleteData(void*)); the body calls the landed NPlayerGui::CDrawMeleeActionButtonEffects destructor @ 0x004420D0 then operator delete;
//   its only reference is the absolute store `mov [eax+4], 0x00443aa0` in the count-block setup of
//   CCountedPointer<NPlayerGui::CDrawMeleeActionButtonEffects>::Reset @ 0x00444B40 (PDB name set: ?Reset@?$CCountedPointer@VCDrawMeleeActionButtonEffects@NPlayerGui@@@@QAEXPAV...@Z).
//   The manifest's CTCMapwho::OnDie is a propagated label.
// Callees: ~CDrawMeleeActionButtonEffects @ 0x004420D0 (thiscall), operator delete @ 0x00BFE9BC.
namespace NPlayerGui {
class CDrawMeleeActionButtonEffects {
public:
    ~CDrawMeleeActionButtonEffects();   // 0x004420D0
};
}

template <class T> class CCountedPointer {
public:
    static void __fastcall DeleteData(void* data)
    {
        delete (T*)data;
    }
};

template class CCountedPointer<NPlayerGui::CDrawMeleeActionButtonEffects>;
