# NF325 lessons (2026-11-24)
- Read a "held" reason against the header before acting on it: NF322 held 0x00A85FB0 / 0x00A86030 on a `_PASS24` switch, but
  CIPhysicsFacesSegmentBase's model is unconditional (only one static and three includes sit under `_PASS24`); and NF321 said
  CInputProcessManager declares AddInputProcess public when the header already has it protected.
- A probe compares full mangled names, so access counts: the `_PASS27` CCombatSequenceBase model declared its protected slots 16-18
  public, which every derived table would have shown as DIFF. Fix access add-only under the new macro, never by editing the old lines.
- A model with reference members and a vector of a forward-declared type cannot be `new`ed; a scratch-only `class CCombatAction {};`
  before the include lets VC7.1 build the implicit copy constructor (`new X(x)`) and emit the table. Abstract bases (MazeBase slot 12
  `_purecall`) are checked through their derived tables instead.
- genmodel.py writes the opt-in above the flat model, which moves the flat model's asserts: run pin_asserts.py on EVERY generated
  header before equiv_check, or every includer differs (my first run flagged all of them).
- genmodel leaves the destructor implicit in base mode; a protected retail destructor (`??_E...@@MAE`) needs a hand-added
  `protected: virtual ~X();` or slot 0 differs in access.
- One missing derived declaration hides several: ParticleGroup's model lacked nine overrides as well as the slot-28 virtual NF322
  named. Generate the whole RTTI slot list for the base, not only the slot the derived probe pointed at.
- mvrow.py's head search must ignore comments (fixed: comments are masked before the search); always open the draft before locking.
- gen_class_headers.py writes into rebuild/include by design: copy it and redirect OUT before running it for a Q2 base header.
- A background busy loop (`while kill -0 ...; do :; done &`) is still a wait; check background jobs with `kill -0` in a later call only.
