# NF275 header overlay (q12659, row 0x00418DCA CMainGameComponent::CMainGameComponent)

Base copies: `orig/engine/` (copied from the repo working tree at the start of the entry; re-compared equal before and after the
equiv_check). Every change is add-only under a NEW macro (each name had 0 hits in rebuild/ before), except one existing `#if` line in
engine/CMainGameComponent.h that gains `|| defined(FABLE_RETAIL_CMAINGAMECOMPONENT_CTOR)` and four existing member lines that now sit in the
`#else` arm of a new `#ifdef`. With none of the new macros defined, every header's token stream is the HEAD one apart from pinned assert
spellings (whole-tree equiv_check, NOTES.md).

| Header | Macro | What |
|---|---|---|
| engine/CMainGameComponent.h | `FABLE_RETAIL_CMAINGAMECOMPONENT_CTOR` | includes CCountedPointer.h, boost/scoped_ptr.h, CDiskFileWin32.h, CFrameRateSmoother.h; types PPlayerManager (`boost::scoped_ptr<CPlayerManager>`), PLUA (`CCountedPointer<CLUA>`), PDebugFont (added to the existing STEALTH_DRAW `#if`), EventPackageFile (`CDiskFileWin32`), FrameRateSmoother (`CFrameRateSmoother`). Same offsets / size (the class assert is unchanged). Not combinable with the PLAYERMANAGER accessors (they read PPlayerManager as an opaque word). |
| engine/CMainGameComponent.h | `FABLE_RETAIL_CMAINGAMECOMPONENT_INIT_SETQUIT` | `virtual void Init();` (slot 1, 0x004184BD, Ego_r + FableWin ?Init@CMainGameComponent@@UAEXXZ) and `virtual void SetQuit();` (slot 4, 0x004197A0, FableWin ?SetQuit@CMainGameComponent@@UAEXXZ). Declarations only. |
| engine/CNetworkClient.h | `FABLE_RETAIL_CNETWORKCLIENT_CTOR` (inside the CNETWORKCLIENT_UPDATE class) | `CNetworkClient();` 0x00419323 and `virtual ~CNetworkClient();` 0x00419352, declarations only. |
| engine/CGameEventPackageSet.h | `FABLE_RETAIL_CGAMEEVENTPACKAGESET_CTOR` | `CGameEventPackageSet();` 0x009F1A80, declaration only. 3 asserts pinned (31..33). |
| engine/CRoughFrameCounter.h | `FABLE_RETAIL_CROUGHFRAMECOUNTER_CTOR` | `CRoughFrameCounter();` 0x009F8B10, declaration only. 1 assert pinned (12). |
| engine/CDiskFileWin32.h | `FABLE_RETAIL_CDISKFILEWIN32_DEFAULTCTOR_OUTOFLINE` (ignored with `_DEFAULTCTOR`) | `CDiskFileWin32();` 0x00415C30, declaration only. |
| engine/CDiskFileWin32.h | `FABLE_RETAIL_CDISKFILEWIN32_DTOR_DECL_ONLY` | the existing inline `virtual ~CDiskFileWin32() { Close(); }` becomes a declaration (0x00401630) for that TU: no dtor / vtable / ~CAFile copies. 9 asserts pinned (43, 72..79). |
| engine/CFrameRateSmoother.h | NEW FILE | `CFrameRateSmoother : CInitBaseClass`, size 0x30 (Ego_r member order and names at retail sizes; ctor 0x00419747 and virtual dtor declared only). Included only under `FABLE_RETAIL_CMAINGAMECOMPONENT_CTOR`, so no landed row includes it. |

Pins: `pin_asserts.py rebuild/include hdr <5 headers>` -> 13 pinned (CGameEventPackageSet.h 3, CRoughFrameCounter.h 1,
CDiskFileWin32.h 9; CMainGameComponent.h and CNetworkClient.h already spell every assert `_AT`). No assert was added or removed in an
existing header.

Header users: `grep -rl` of each header name in rebuild/src/compiled finds the landed includers (CMainGameComponent.h 491 rows directly);
none of them defines any of the new macros, and the whole-tree equiv_check (every landed source and test, old tree v new tree) is the
proof that the opt-ins change none of them (NOTES.md, evidence/equiv_e1.log).

NF274's overlay (`$S/w22/agentNF274/hdr/`) was empty when these were made; none of the six headers is in it.


## Entry 2 (nf273 ruling 2b, row 0x00A96DA0 ~CVertexGroupBlock)
| Header | Macro | What |
|---|---|---|
| engine/CVertexGroupBlock.h | `FABLE_RETAIL_CVERTEXGROUPBLOCK_DTOR_INLINE` (effective only with `FABLE_RETAIL_CVERTEXGROUPBLOCK_MEMBERS`) | the existing `~CVertexGroupBlock();` declaration line moves into the `#else` arm; the opt-in arm is the inline empty body `~CVertexGroupBlock() { }` (members typed, so their destructors run). Only 0x00A96DA0 defines it. The class assert was already `_AT(44)`: pin_asserts pinned 0. |

Base: orig/engine/CVertexGroupBlock.h (repo copy at the start of entry 2; re-compared equal after its equiv_check). Its own whole-tree
equiv_check (repo tree v the repo tree with only this file replaced) is NOTES entry 2. Landed users: 14 direct includers; none defines the macro.
