# hdr/ overlay -- chunk 880 (0x0058AF10 CTCInventory::OpenPCOptionsMenu)

Every change is additive behind a new `FABLE_RETAIL_*` opt-in; the default path of each repo header is unchanged
(`diff rebuild/include/<f> hdr/<f>`).

| File | Opt-in | Change |
|---|---|---|
| stlport/fable_containers.h | FABLE_RETAIL_STLP_MAKE_PAIR_ARRAYS | STLport 4.6 `_pair.h` string-literal `make_pair` overloads (array arguments decay to `const T*`), so `make_pair("A", "B")` is a `pair<const char*, const char*>` converted by the member-template ctor (0x00592495). Without them VC7.1 rejects `make_pair` of two literals (C2536). |
| engine/CTCInventory.h | FABLE_RETAIL_CTCINVENTORY_OPTIONSMENU | `void OpenPCOptionsMenu();`, the PDB nested enum `CURRENT_OPEN_MENU` (type only) and `CurrentOpenMenu` typed as it (+0x21C, same 4 bytes, static assert under the opt-in). |
| engine/CTCInGameMenu.h | FABLE_RETAIL_CTCINGAMEMENU_OPENPC | `void OpenInGameMenuPC(NUISystem::CComponent*, NUISystem::CComponent*); // 0x004804DE` + forward declaration. |
| engine/NUISystem_CRedefinerList.h | FABLE_RETAIL_CREDEFINERLIST_SETACCEPTBUTTON | `void SetAcceptButton(CComponent*); // 0x00556570`. |
| engine/CUserProfileManager.h | FABLE_RETAIL_CUSERPROFILEMANAGER_DISPLAYMODES | `const std::vector<CDisplayModeDesc>& GetDisplayModes() const; // 0x00406880` (+ includes). |
| engine/CDisplayManager.h | FABLE_RETAIL_CDISPLAYMANAGER_ANTIALIASING | `void EnumerateAntiAliasingModes(CArray<ESurfaceMultisampleType>&); // 0x009C05C0`. |
| engine/CDisplayModeDesc.h | (new file) | Ego_r.pdb CDisplayModeDesc: C2DExtentsI Dimensions, long BitsPerPixel, long RefreshRate (16 bytes). |
| engine/NUISystem_CActionParamEBOOL.h | FABLE_RETAIL_CACTIONPARAMEBOOL (new file, `#error` without it) | CActionParamEBOOL : CActionParam, inline `CActionParamEBOOL(bool)` (type 8, `bool Value` +8), out-of-line virtual dtor; sizeof 0xC. Twin of NUISystem_CActionParamULONG.h. |

Include-order note: `fable_string.h` and other top-level repo headers include `stlport/fable_containers.h` by a quoted path, which
MSVC resolves next to the includer first (the repo copy). The row therefore includes `stlport/fable_containers.h` first so the
overlay copy wins; once the change is merged into rebuild/include this is a no-op.
