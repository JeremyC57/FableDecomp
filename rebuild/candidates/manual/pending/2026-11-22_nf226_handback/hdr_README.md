# NF226 hdr/ (base copies in ../orig/, taken from the repo working tree 2026-11-22, nf225 applied)
All three overlays are add-only: every added line sits under `#ifdef` / `#elif defined(...)` of a new member-specific macro that no file
in rebuild/, tools/ or docs/ defines (grep), so the default path and every existing opt-in path are token-identical. The only `<` line in
`diff orig hdr` is NUISystem_CObservable.h's static assert, respelled by pin_asserts.py (below). No other live agent overlay holds these
headers (AUTH12 has none).

## engine/NUISystem_CManager.h (q12566 / q12567)
- FABLE_RETAIL_CMANAGER_SETGRAPHICSBANK: `public: void SetGraphicsBank(CCountedPointer<CGraphicDataBank> graphics_bank);` -- Ego_r.pdb /
  FableWin.pdb ?SetGraphicsBank@CManager@NUISystem@@QAEXV?$CCountedPointer@VCGraphicDataBank@@@@@Z, out of line at 0x0042A9B7 (landed;
  its own row and reloc_bindings spell the same). Declaration only, after the WORLDINIT block. Users: rows 0x0041E641, 0x0041E3F6.
- No existing member changed: the header already had `protected: CManager(); ~CManager();` (IAE) and the retail layout (probe: sizeof 0xD0,
  PGraphicsBank +0x10 ... Keys +0x24, ThingActionParams +0x5C, UIActionParams +0x60, Layers +0x70, BastardChild +0x80, BastardChildren +0x88,
  m_bHasErrorMessage +0x9C, MouseMovement +0xB0, as retail; work/t/probe_layout.cpp). Its 45-slot vtable has retail 0x01230134's slot count and every slot's target address (work/vt_0041e641.txt); 12 of the target rows
  emit another spelling than the slot (NOTES "Vtable").
- pin_asserts.py: pinned 0. Includers: 274 landed rows + 1 header (engine/CEnginePrimitive2DSprite.h); none defines the new macro.

## engine/NUISystem_CObservable.h (q12567)
- FABLE_RETAIL_COBSERVABLE_CTOR_DECL_ONLY: `public: CObservable();` -- Ego_r.pdb 0x0043A1C1 / FableWin.pdb 0x01AE4FA4
  ??0CObservable@NUISystem@@QAE@XZ, out of line in retail at 0x0042BE7B (the CManager constructor calls it). Declaration only, first in the
  class (the class had no declared constructor: the default path keeps the implicit one). User: row 0x0041E3F6 (includes this header itself,
  first, before engine/NUISystem_CManager.h, so the overlay copy is the one used).
- pin_asserts.py: pinned 1 (`FABLE_STATIC_ASSERT(sizeof(NUISystem::CObservable)==16)` -> `FABLE_STATIC_ASSERT_AT(23, ...)`, the six added
  lines moved it). Includers: 3 landed rows + engine/NUISystem_CManager.h; none defines the new macro.

## engine/CFrontendGameComponent.h (q12568)
- FABLE_RETAIL_CFRONTENDGAMECOMPONENT_INITGRAPHICS: a third layout branch (`#elif`, so not with _CLEARSTRING): `private: bool
  InitGraphics();` (Ego_r.pdb 0x0049D3B0 / FableWin.pdb 0x01A41C50 ?InitGraphics@CFrontendGameComponent@@AAE_NXZ) and
  `CCountedPointer<const class CFontBank> MainFont;` at +0x1E04 (Ego_r.pdb struct layout: MainFont 7684 = 0x1E04,
  CCountedPointer<CFontBank const>); the rest opaque, same size 0x1E60 as the default path. The includer includes engine/CCountedPointer.h
  first. User: row 0x00494840 and its test.
- pin_asserts.py: pinned 0. Includers: 5 landed rows; none defines the new macro.

assert_names_check.py rebuild/include hdr: mismatches 0.
