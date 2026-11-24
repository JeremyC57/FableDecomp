# Surface extent owner controls

The selected row uses the original named GetWidth and GetHeight calls. FableWin's `lib_surface.cpp` line 666 calls GetWidth at `030C9066` and line 667 calls GetHeight at `030C9074`. The optimized Ego_r leaf at `00B47AA0` expands both into Direct3D GetDesc sites. Both donor PDBs record the separate out-of-line owners in `main_game_component.obj` at `lib_surface.hpp` lines 130 and 131. Those owners call `GetDimensions` in both original executables.

## Direct-expansion diagnostic

`expanded_extent_control.cpp` spells these two reads as `GetDimensions().X` and `GetDimensions().Y`. This is a diagnostic for nested inlining. The selected source preserves the original named calls. The diagnostic retains the original width and height definitions under inline depth zero, matches all 582 masked retail leaf bytes and all seven mapped bodies, and resolves all 12 relocation operands.

Compile with VC7.1 RTM 13.10.3077 using `/nologo /c /O2 /Oy /W3`. Use a clean copy of the packet's original `include/` and apply `hdr/PATCHES.json` from the checkpoint in listed order. The grade records header tree `6a20af7d676e`. The diagnostic enables the existing `FABLE_RETAIL_CSURFACE_SIZE_OUTOFLINE` option. For the source-synchronized `expanded_extent_control_test.cpp`, use `/Od /W3 /GR-` or GCC `-std=c++03 -pedantic-errors`.

## Original owner context

`main_game_component_extents.cpp` materializes the genuine original inline GetWidth and GetHeight from the shared header with GetDimensions declared. Two verification functions take member addresses to keep both bodies in the object. VC7.1 builds using `/nologo /c /O2 /Oy /W3` and the donor's recorded optimization profile `/nologo /c /Ox /Ob2 /Oi /Ot /Oy /GT /Gr /GR` both produce the exact 18-byte width and 19-byte height wrappers. Each wrapper's sole relocation resolves to `CSurface::GetDimensions` at retail `009F2F90`.

For both profiles, compile `main_game_component_extents_test.cpp` with `/Od /W3 /GR-` and link it with the corresponding owner object. The test checks both coordinates and successive GetDimensions calls. The owner and test sources also build together with GCC `-std=c++03 -pedantic-errors -DFABLE_RETAIL_PASS23_HOST_TEST_ABI` against the same include directory.

For the offline copy-selection diagnostic, compile the selected `009f3da0.cpp` and the separate owner fixture using `/nologo /c /O2 /Oy /W3`. Link both objects with VC7.1 `link.exe /dll /noentry /nodefaultlib /force:unresolved /opt:noref /opt:noicf /incremental:no /base:0x400000 /map`. Supplying the owner object before the leaf object retains both exact wrappers and the exact copy leaf. Reversing the order retains both 24-byte leaf-object copies. This diagnostic inspects the linked image without executing it. The complete row behavior and both named getters are exercised by the separate tests.

`COMPILAND_EVIDENCE.json` records both donor identities, the standalone and separated-owner grades, and both linked outcomes. `RELOCATION_AUDIT.json` records the exact operand targets. `TEST_RESULTS.json` records the behavior tests. The selected source remains held by the strict standalone object check for two unreferenced copies.

`repeated_callers/README.md` documents an additional donor-backed diagnostic. It reconstructs the exact 271-byte sibling CanDoProperSignedConversion, including its three surviving width and height calls, in the same object as the exact 582-byte readback. The two input orders show which accessor bodies receive the sibling's live calls.
