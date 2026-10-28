// NPlayerGui::CDrawMiniMap::IsActive() const -- during a region change the mini map counts as active while its fade is active; otherwise it is active when switched on and not fully transparent
// Leaf: ?IsActive@CDrawMiniMap@NPlayerGui@@QBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00643E09, 37 bytes, thiscall, `ret`, /O1 TU (unaligned starts, `xor eax,eax ; inc eax`).
// Identity (proven): Ego_r.pdb donor order ClearPrimitivesIfNecessary (retail 0x00643DE5, landed) -> IsActive -> SetAsActive
//   (retail 0x00643E2E, landed); ?IsActive@CDrawMiniMap@NPlayerGui@@QBE_NXZ is const in the PDB (reloc_bindings carries a
//   non-const QAE spelling for this address -> Review at landing).
// Layout: the retail offsets of the landed CDrawMiniMap functions (IsFinishedOpening 0x00644179): Active +0xe9,
//   RegionChange +0xeb, Alpha +0xec, FadeInOut (CFadeInFadeOutBase, Active +0x00) +0x100. The engine header is a flat
//   global-namespace struct, so the class is modelled locally with the PDB names.
#pragma optimize("s", on)
class CFadeInFadeOutBase {
public:
    bool          Active;                          // +0x00
    unsigned char _rest[0x1b];
};

namespace NPlayerGui {

class CDrawMiniMap {
public:
    unsigned char      _pad00[0xe8];               // CDrawBase, Bank, Drawing, PrimitiveHandles, MiniMap
    bool               Enabled;                    // +0xe8
    bool               Active;                     // +0xe9
    bool               OldActive;                  // +0xea
    bool               RegionChange;               // +0xeb
    unsigned char      Alpha;                      // +0xec
    unsigned char      _padED[0x100 - 0xed];       // ScreenBox
    CFadeInFadeOutBase FadeInOut;                  // +0x100
    bool IsActive() const;
};

bool CDrawMiniMap::IsActive() const
{
    if (RegionChange)
        return FadeInOut.Active;
    return Active && Alpha > 0;
}

}