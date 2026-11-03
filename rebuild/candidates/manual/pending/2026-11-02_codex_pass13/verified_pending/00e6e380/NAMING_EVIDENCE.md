# 00e6e380 final handoff

Final status: **VERIFIED_MATCH**. Selected grade: 5. 

ROW.json and root STATUS.tsv are authoritative. Earlier status claims below are historical; they may have been superseded.

## rows/00e6e380/NAMING_EVIDENCE.md

# 00e6e380 naming and validation

`NScript::CV_DemonDoorsScript::Main`, exact `?Main@CV_DemonDoorsScript@NScript@@UAEXXZ`, is present in both original PDB name sets. Retail script vtable 012e8b10 slot 2 points at this complete assigned extent. The neighbouring ctor and registered script factory independently establish the class identity.

Original Ego_r TPI 0x10567 establishes the base, class key, members and nested classes. The new shared header retains every data member with original access, including protected reference ParentClass in each nested entity. The existing shared CLevelScriptBase and CEntityScriptBinding implementations produce the retail layout and compiler shape without code-generation devices. Existing header defaults are unchanged.

14 registration names and factories were verified individually against PDB names and original string/address pairs. All 113 relocated operands and both binding-vtable entries were audited. The post-registration virtual call uses Game slot64 PostAddScriptedEntities. The 16 behavioral scenarios exercise success, each individual allocation failure, and all allocations failing; the pasted production body checks names, factories, parent pointers, flags, string lifetimes and callback order. Both C++03 compilers pass. Tests use instrumented dependencies; the production compile separately proves layout and exact code.
