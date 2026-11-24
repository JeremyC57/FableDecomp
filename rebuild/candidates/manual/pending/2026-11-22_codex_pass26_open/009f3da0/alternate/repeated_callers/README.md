# Caller-dependent surface inlining

FableWin's original `lib_surface.obj` has 42 direct width and height calls across the readback, upload, pixel, clear and save procedures. The selected readback calls both names at `030C9066` and `030C9074`. The `/Od` GetDimensions body is a distinct 254-byte procedure in that object. Its CodeView lines 159, 160 and 162 identify the COM call and saved HRESULT, the Direct3D debug check, and construction of the extent result. The width and height wrappers at `lib_surface.hpp` lines 130 and 131 are from `main_game_component.obj`.

In Ego_r, the same library object expands both readback getters. It still calls the original width and height symbols in other procedures. CanDoProperSignedConversion calls GetHeight at `00B47361`. The four-argument CopyFromMemory calls GetHeight at `00B4791F` and `00B47943` and GetWidth at `00B47938`. Their targets are the 18- and 19-byte wrappers owned by `main_game_component.obj`.

## Same-object diagnostic

`repeat_signed_original_order.cpp` uses the same shared headers and named member calls as the selected row. It adds the actual typed CanDoProperSignedConversion procedure before the readback in their original source order. Compile with VC7.1 RTM 13.10.3077 and `/nologo /c /O2 /Oy /W3` against the clean checkpoint include tree, using the header preparation in the parent README. The recorded verification used tree `17db647dc01f`. The recorded original release flags `/Ox /Ob2 /Oi /Ot /Oy /GT /Gr /GR` reproduced the same outcome.

The object matches 582 masked bytes of the readback and 271 masked bytes of CanDoProperSignedConversion. All 12 direct sibling operands were checked against their native addresses and semantic bindings. The readback expands both named extent getters. The sibling expands the first GetWidth, calls GetHeight twice and calls GetWidth once. Its three real accessor operands are at retail `009F364D`, `009F366D` and `009F3679`. The emitted getter bodies are still 24 bytes each in this library object.

The other probes add the actual GetByteLength body and an experimental reconstruction of the longer four-argument CopyFromMemory. Reordering the surrounding definitions and moving the ordinary GetDimensions body before or after the readback still gives both 24-byte getters. The reconstructed CopyFromMemory differs from retail.

## Link-order diagnostic

Compile `main_game_component_extents.cpp` from the parent alternate directory under the same retail flags. Link it with the object from `repeat_signed_original_order.cpp` using the same offline VC7.1 LINK recipe as the parent README. Owner-first preserves both exact library procedures and routes all three live sibling operands to the exact 18- and 19-byte accessor wrappers. Both wrappers then target the same actual GetDimensions body. Library-first preserves both exact library procedures and routes the three operands to the two 24-byte library copies. `REPEATED_LINK_EVIDENCE.json` records the linked addresses and owner selection from offline inspection of the partial linked image.

`REPEATED_CALLER_EVIDENCE.json` records original donor edges, debug statement evidence, compiler profiles and the verified retail operands. The canonical row has native and portable behavior tests. The strict standalone grade is held because the readback object still contains two different getter copies.
