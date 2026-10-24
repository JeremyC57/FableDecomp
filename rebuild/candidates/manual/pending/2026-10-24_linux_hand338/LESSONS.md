# Chunk 338 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- A directly recursive inline function (STLport `_M_erase` as an inline member) is expanded exactly once by VC7.1 and then called out of line. That reproduces a retail one-level unrolled tree erase (0051ac70 -> 0x4A9CB0).
- A /GS function whose error arm builds a std::string from a literal matched only with `__forceinline` string ctor/dtor; an out-of-line ctor call otherwise remains (008f8870, 008f8ea0).
- `return x != -1 ? true : false;` drops the extra `xor/setne/mov` that a plain `return x != -1;` keeps after a store of x (008f8910).
- An ebx-held bool constant reused as several call arguments and stores comes from a named `bool result` local, assigned once before the uses (0090b080).
- A chained `a = c = b = -1;` emits the stores rightmost first (b, c, a); write the chain in the retail store order reversed (00c38cd0).
- A compiler-generated dtor that stores no vtable (??1 with member dtors only) comes from an implicit dtor; pull it into the TU with a `new` factory over an inline ctor (00bd9250).
- `boost::checked_delete<T>` is `__fastcall` (YI) in FableWin.pdb; declare the template `__fastcall` and explicitly instantiate it (004ae810).
- STLport `vector::erase(first, last)` in retail LSocket passes the same iterator twice (`erase(begin()+n, begin()+n)`, one register pushed twice), a no-op erase in the original source; reproduce it as written (008f8440).
- Mass math: C2DVector with a user copy ctor plus member `+ - *(float)` reproduces GetVel (008eea30). The same model misses on the dot-product schedule in ApplyImpulse and ForceToVel: retail multiplies the X term from memory after an fxch.

Review at landing:
- 008eea80 ApplyImpulse took 5 grades (one over the budget of 4) before parking.
- 0x009AD2E0 (the callee of 009ad3b0) is bound to conflicting names across landed rows: FindFactionBySymbolName, GetClassInfo and FindClassInfo. 0x0042B467 likewise carries several names.
- The 0051ac70 TU also emits an out-of-line `Erase` COMDAT (0x4A9CB0's body); the lander must take only the DestroyRange symbol.
- 009a0190 / 009a0430 are byte twins; which address is CBasicString<char>::~CBasicString and which is ~CCharStringData is unproven (both names are plausible from the xrefs).
- Engine headers not used: rebuild/include/engine/LHHttp2.h, CDefinitionManager.h and CEngineInternalPrimitiveAnimatedMeshFromFile.h carry the Ego_r.pdb (Dinkumware) layout, are data-only, or are opaque, and their offsets disagree with retail's STLport layout. These rows use local retail-offset models.
- The comment in the already-landed LHHttp2::ResetDocumentRecv says the DocumentParts vector is at +0x880; retail has it at +0x884 (ProxyPort is at +0x880).
- 00c38cd0 carries a descriptive placeholder class name (CSlotRequest_00c38cd0) until the owner is identified.
