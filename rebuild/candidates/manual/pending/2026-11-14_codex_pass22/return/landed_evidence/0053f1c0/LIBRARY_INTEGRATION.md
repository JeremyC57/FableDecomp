# Required normal library selection

Compile the target with VC7.1 RTM /O2 /Oy /W3 and its shared-header opt-ins. Compile the two explicit-instantiation sources using the same headers plus original STLport4.6.2:

1. library/StateMapHelpers.cpp: /O1 /Oy /W3.
2. library/StateMapSize.cpp: /O1 /Oy- /W3.
3. The target object follows those two objects in ordinary link order.

All library sources are real original template bodies. No symbol is suppressed, renamed, substituted or patched. The original allocator compatibility option routes to the native malloc/free path. The target TU carries ordinary unselected COMDAT copies; their bytes must not be confused with the selected functions. SOURCE_MANIFEST and the overlay record preserve vendor provenance and notices.

LIBRARY_SELECTION.json proves actual linker selection of all15 native bodies and preserves two rejected counterexamples: target-first selects only12 correct bodies; size-first only2. The diagnostic no-entry DLLs permit unresolved game boundaries solely to inspect selection; they are not executed. Separately, the fully linked behavior executable actually executes the14 correctly selected library bodies, verified in its PE and link map. No runtime byte payload is used.

Target1601 + selected helpers1248 =2849 verified bytes. Only1601 count toward assigned-row progress. MAIN_AUDIT.json and the exact42-edge inventory identify every helper address/leaf/size. Return both wrapper sources, modified header overlay, original vendor source/manifest/notices and this integration recipe. Exclude all objects, executables, DLLs, raw retail bytes and private donor files from the handoff.
