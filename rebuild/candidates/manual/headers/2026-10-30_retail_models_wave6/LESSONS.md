# Wave 6 lessons (class-model migration, 2026-10-30)

1. **The GRO inlining rule: VC7.1 decides inline-or-call per site from the inlinee's size, so the row must give the
   implicit ScriptedThingBase dtor its real size.**
   - Retail's ~CScriptGameResourceObjectScriptedThingBase (COMDAT 0x007E74D0, twin 0x007E7520) is the implicit dtor:
     the PImp release (`dec [Info]; DeleteFunc(Info->Data)` __fastcall; `operator delete(Info)`; zero Data / Info),
     then GROBase's user-written dtor inline (`mov [esi],0x0126008C; jmp ~CBaseObject 0x0099A430`).
   - VC7.1 RTM 3077 expands that dtor at some sites of a function and calls the COMDAT at others; the same function
     can do both (0x00DF06A0: the early `return` inside the wait loop gets the expansion, the common exit calls
     0x007E74D0). Our compiler reproduces the per-site choices from source alone, with no TU context, once the
     inlinee is the real one.
   - Wave 5's "mixed inlining" reverts came from the rows' pointer models: a two-word `CCountedPointerWords` or a
     dtor-less `CCountedPointer<T>` makes the implicit STB dtor tiny, and then VC7.1 inlines it everywhere. Wave 5's
     own conclusion (lesson 2: leave them local) was right about the header and wrong about the cause.
   - Test (scripts/gro_full.py: the full template replaces the row's pointer model, then mig_gro.fix): 0x00E0CFF0
     DIFFER 320v312 with a dtor-less template -> RELOCATION_MATCH with the full one. Of the 66 wave 5 GRO reverts,
     16 now match (27 DIFFER, 23 compile failures, mostly the MovieBase / template clashes below). Of the 55
     STB-only rows tried (no MovieBase), 43 pass mcheck; 8 DIFFER (the ones inspected differ only by loop layout /
     alignment padding, e.g. 0x00E10BF0 384v384) and 4 fail to compile.
   - **Not explained (residue, logs/mc_gro_a.txt):**
     - MovieBase rows (22 of 23 fail): retail expands the MovieBase dtor but *calls* the CCountedPointer dtor COMDAT
       0x006E7AB0 inside it, while VC7.1 expands both levels. A `~CCountedPointer() { ReleaseData(); }` split
       (Ego_r has ReleaseData) does not change that (0x00DF2D30 stays 385v349).
     - Argument temporaries: 0x00EB6A00 `resize(2, CScriptGameResourceObjectScriptedThingBase())` calls 0x007E72A0 and
       0x007E74D0 in retail; VC7.1 expands both (158v83). The same row calls CScriptThing() 0x006E7B40 out of line.
     - So the 50 rows that call 0x007E72A0 (mostly explicit `Construct()` stand-ins in big Main functions and
       argument temporaries) are still not explained by size alone.
2. **A non-virtual member declaration added to a shared header is a no-op for its includers** (80 random CGSI-header
   includers and all 20 CPlayerManager.h includers mcheck `--all` SAME, apart from `$L` label renumbering). Adding the
   PDB accessors is safe without regrading every includer.
3. **Float setters: give the store target the PDB parameter's type.** With the PDB `float` parameter, a `long m_X`
   target makes VC7.1 convert (DIFFER 176v218); `float m_X` keeps the raw dword copy and the row is SAME (8 rows).
4. **Variable reuse inside the migration scripts.** mig_cgsi_m.py's member-offset loop used `al` for alignment; the
   new alias flag of the same name was overwritten, so the dry run reported every row as "alias". Name flags
   distinctly; read the dry-run notes before applying.
5. **Rewriting a helper's calls needs a real argument splitter.** The first SpawnFunction rewrite split on the first
   comma and produced `CCharString(0, this, &X::Main)` from `new CSpawnedFunc<X>("Main", 0, this, &X::Main)`; 4 rows
   failed to compile until mig_sb.py split at depth-1 commas (quotes, parentheses and template brackets respected).
6. **Leaves that become overrides.** A derived level script that declares `void Main();` non-virtually over a local
   CScriptBase with only a pad now overrides the header's slot 2; the definition compiles to the same bytes and its
   symbol becomes the PDB `UAEXXZ` spelling (65 SB rows). mcheck reports these as `leafsym` renames, not failures.
7. **Template-parameter clashes are the commonest SB residue.** Rows that define `std::vector<T>` / `std::list<T>`
   with one parameter, a non-template CCountedPointer, or a `typedef long EScriptAIPriority` conflict with
   fable_nscript_forward.h (11 of 16 SB failures). cpfix.py / tdfix.py handle two of the three shapes.
