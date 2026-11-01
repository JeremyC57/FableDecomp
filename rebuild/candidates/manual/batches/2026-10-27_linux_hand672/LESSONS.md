# Chunk 672 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- 2026-11-01 (chunk 672) Entity-script implicit dtors are named the same way as quest scripts, one level deeper: the entity
  vtable's ctor -> factory (`__fastcall` Alloc with two stack args, `new` = Ego_r sizeof) -> the parent quest's Main stores the
  factory beside `push "<EntityName>"` (CEntityScriptBinding record, vtable 0x012D2704 / 0x012D6D04) -> Main is slot 2 of the quest
  vtable -> quest ctor -> quest Alloc -> RegisterScripts `push "Q_<Name>"`. The factory's `new` size picks between same-named nests
  (CBriarRose 0x50 = CQ_MinionCampScript's; the other two nests are 0x20) (0x00D9FED0, 0x00D728F0).
- 2026-11-01 (chunk 672) The RegisterScripts blocks near 0x00CD7xxx-0x00CD9xxx push only the short section name ("S_VBCP",
  "S_VCK") next to the Alloc store; refs/script_recovery/conversion_queue.tsv maps section -> script name -> Alloc address, a
  second source for the quest class (0x00E477F0, 0x00E62780, 0x00EB6AB0, 0x00E1DA40, 0x00E68EB0).
- 2026-11-01 (chunk 672) Seven quest/entity implicit dtors matched on the first probe from the chunk 664/665 recipe (PDB ctor
  defined in the TU, local CLevelScriptBase<T> without dtor declaration, inline `~CScriptThing() {}` / `~CTimer()` after the
  headers, real STLport _Vector_base/__destroy_aux and _tree.h text); a `CTimer X[4]` member array becomes retail's inline
  countdown loop with no EH iterator (0x00E62780).
- 2026-11-01 (chunk 672) A class whose second base is CMovableResource (vptr only) shows the base's own vtable store with no call
  before the tail jump: model `virtual ~CMovableResource() { }` inline (only FableWin has ??1CMovableResource); the same store
  follows `Uninitialise()` in the inline ~CIndexBuffer (0x00A4EEC0, 0x00BABE60).
- 2026-11-01 (chunk 672) Destructor labels in this band: 3 of 10 right (2 of those missing the nesting class). The ??_G slot-0
  lookup settled the engine rows; the RTTI-less script rows needed the ctor/Alloc/registration chain every time.
- 2026-11-01 (chunk 672) vland's SHARED_HEADER_AVAILABLE check only matches `class X {` with no base list and is silenced by the
  text `engine/X.h` anywhere in the source: write the `// Layout:` line naming each flattened header you restate.

Review at landing:
- NAME_FIXES.tsv (26 rows): 10 own rows (8 wrong or unnested catalog labels), 11 landed slot-0 ??_G stand-ins that call them
  (seven CLandscapeBackgroundPatch `vector deleting destructor` free functions, three `Dtor()` / `_dtor` stand-ins, plus
  0x00A555A0 for C3DAnimationStats), and callees 0x00A4EB50 (~C3DAnimationStats), 0x00A53300 (_List_base<CIntelligentPointer<
  C3DAnimation2> >::clear), 0x00AC1EE0 (CIndexBuffer::Uninitialise), 0x00F37030 (FreeMemoryPage spelling), 0x00F35B40.
- vland WARN RELOC_NAME on 0x00BABE60: 0x00AC1EE0 catalog 'CVolume::ReleaseVolume' and 0x0099A300 'CBaseClass::~CBaseClass'
  (??1CBaseClassNonCopyable@@MAE@XZ, raised by earlier chunks) -- catalog labels, not source problems.
- 0x00A53300 is called from two unreferenced out-of-line ~list bodies (0x00A54130 / 0x00A544C0, landed as `list_T` stand-ins);
  they are probably ~_List_base<CIntelligentPointer<C3DAnimation2> > COMDATs of the same TU -- not proven, not in NAME_FIXES.
- Each script row's TU defines the class ctor with an empty/init-list body (not retail's body: only the graded dtor is retail).
- Global 0x01436E98 spelled `g_pEngine_01436e98` (CEngineGlobal_01436e98, majority landed spelling); still no established name.
- Every row matched within 2 probes; no row needed more than 4 grades; nothing parked, nothing int3-after-noreturn.
