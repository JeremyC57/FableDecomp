# Chunk 845 -- lessons from the authoring agent (fold into GOTCHAS at landing)
- AddSpawnedFunction's region name built AFTER the new-expression (and destroyed first) is a default argument: the opt-in
  FABLE_RETAIL_CSCRIPTBASE_ADDSPAWNEDFUNCTION_DEFAULT_REGION (`const CCharString& = CCharString("")`) gives retail's order with the
  shared headers, no wrapper or pragma (0x00ED8680, md 324 -> 196).
- `CCharString("ParentClass.") + CCharString(name)`: a string-literal left operand picks a different operator+ (0x00ED8680).
- A row-local `inline CScriptThing::~CScriptThing() { }` can fix both the per-site temporary dtors and the frame slot order of a
  long Main (Book 0x00E54E90, md 104 -> 35).
- A row-local `__forceinline CScriptThing::operator=` expands a second if-chain assignment that the implicit operator= leaves
  called (HeroDolls 0x00E97070).
- An entry `mov [esp+N],0` (or `,ebx`) with no visible reader is a conditional-temporary flag home; VC7.1 keeps the store even when
  every test of the flag is threaded away, and the slot is shared with later temporaries. Two rows (0x00E54E90, 0x00E97070) carry
  one whose owning expression was not found: a final-loop `&&` put it at the right slot but grew the frame by 4.
- Inline-frequency split of STLport resize(n): the same `v.resize(n)` expands in a mini TU and after one preceding wait loop but
  is called after two (0x00D67890 bisect); it is not the S15/S20 budget.
- The two-idle-loop shape of CAssassin1::Main matches under the diagnostic S15 meter only: a caller-size park (0x00D04AE0).
- Single-site do-while(0) round a whole TalkedTo body or an if-chain stops the movie-dtor pads merging (Book, md 35 -> 918).
- FixMovieSequenceCamera(true/false) round the RunCutsceneMacro chain is written explicitly (0x00E57E60, 0x00D67890).
