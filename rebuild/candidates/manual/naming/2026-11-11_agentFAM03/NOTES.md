# agentFAM03: family `<Class>::vfunc_N` (1,432 landed rows, unnamed virtual slots)

Result: **742 rows renamed** (`rows/<addr>.cpp`), final batch `work/final.out`: 742/742 MATCH with the `// Leaf:` symbol present in
the object (`objdump -t`). Behaviour tests unchanged (every family test is a self-contained `printf("PASS_<addr>")` / model that
links nothing from the row): 742/742 PASS under VC7.1 /Od and 742/742 under g++ -std=c++11 with the catalog PassPattern (`work/tests.tsv`). Nothing landed, no git,
nothing written under the repo, BUSY.txt / agentH40j / agentQ3 / agentQ4 untouched (no BUSY address is in this family).
**690 rows left** (LEFT.tsv, one reason per row): 445 have their name PROVEN (symbol in LEFT.tsv `proven_symbol`) but no respelled
source this pass; 8 withheld as weak alignments; 237 unproven.

## Method (settled once, scripted: `work/*.py`)
No repo file holds the PDB virtual ORDER: struct_layouts_egor.tsv / fable_types.h are data members only, and the Ego_r / FableWin
name sets are keyed by donor address (no vtables). So:
1. **TLC vtable census** (`census.py`): every RTTI vtable of retail Fable.exe with its slot addresses and RTTI base chain
   (stops at the first entry that is neither a catalogued function start nor preceded by int3/nop/ret padding).
2. **Order source** (`dump360.py`): the Xbox 360 Test PDB type records (`$S/anniv_dbg/dl/WellingtonGame-Xbox360-Test.pdb`, private
   lookup, FABLE_ANNIVERSARY.md usage rule) give each class's full vtable: intro-virtual vfptr offsets (LF_ONEMETHOD / LF_METHODLIST),
   overrides placed on the base slot. Used only as an ORDER, never as a spelling or a layout.
3. **Alignment to TLC** (`align2.py`): 360 entries are kept only if the name + parameter list is a virtual of the class hierarchy in the
   TLC name sets (drops Anniversary additions such as CTCBase::BeginSerialising and the CInterpolationInfo overload of
   DrawPreparePrimitivesForRendering: TLC CTCBase has 28 slots, the 360 one 30). Anchors = TLC slots whose catalogued name is a TLC
   virtual of the hierarchy; between two anchors a slot is named only when the gap's TLC-known 360 entries are exactly as many as the TLC
   slots (gap kind GS = strict signature match, GSC = name + arity). Renamed rows use GS/GSC gaps only.
4. **Owner** (`fam.py`): the most-base class whose vtable holds the body at that slot; bodies at several slots or in unrelated
   vtables (identical-code folding) are not named. 40 renamed rows change class: the catalog had the inheriting class (e.g.
   0x00686880 `CTCInGameMenu::vfunc_7` = `CTCBase::RenderUpdate`).
5. **Spelling**: the owner's virtual of that name and parameter list in the Ego_r / FableWin name sets (U/M/E access, const-ness,
   return type exactly as mangled). 1,194 of 1,195 proposed symbols bind no other address (`collide.py`); the one clash is below.
6. **Independent check**: `ret N` of every body against the signature (`retchk.py`): 1,121 agree, 1 disagrees (excluded), the rest
   jump/no-ret. Bodies that contradict the signature (a float load in a void ProcessEvent) were traced to weak gaps and withheld (8).
Two TLC sources per row: the TLC vtable slot pinned by the TLC-named anchor slots around it (the 360 order is the bridge, recorded as
"Anniv360 (third source)"), and the owner class's exact virtual in the Ego_r / FableWin name sets; `ret` as a third check.

## Renamed (RENAMES.tsv section 1; evidence column per row)
- Constant bodies (`mov al,1` / `xor al,al` / `mov eax,imm` / `fldz` ...), 551: HasFrameUpdate 151, GetType 133 (creature modes,
  entity events, messages), IsComponentSerialisable 71, GetModeType, GetComponentType / GetComponentSubType (PSC), GetInterfaceType,
  Is* camera-mode flags, ...; empty bodies 112 (OnKill, OnDie, OnChangeCategory, FrameUpdate, RenderUpdate ...).
- GetAnimFilter 73 (headerless NCreatureMode::CMode*): `lea eax,[ecx+N]` returns the Ego_r `CAnimFilterOfGroup Filter` member at the
  retail offset (engine/CAnimFilterOfGroup.h).
- Member getters / setter 6 (headerless camera modes and CTCCameraPointDefinitionBase), Ego_r member at the retail offset.
- Spelling forms: classes with no shared header (443 rows, incl. the 73 GetAnimFilter and 6 getter rows) get a minimal local declaration of the real class (namespace, class key from
  RTTI, the one virtual, retail access); classes with a header include it with the opt-ins of a landed row of that class (124 rows),
  or with the overlay opt-in below (175 rows). Enum returns use the engine enum header's enumerator where one exists, else a cast to the
  enum (namespaced enums have no TLC enumerator list).

## Header overlay (hdr/, README.md lists the 91 headers and their rows)
`FABLE_RETAIL_<HEADER>_VSLOTS`: the retail virtuals the header's declaration lacks, declared inside the class (declaration only); in a
flat generated header the `void* __vftable` member is swapped for them under the same opt-in (same layout, vptr at +0). Default path
of every overlay header is byte-identical to the repo copy (checked by stripping the blocks: `work/hdrreadme.py`).

## Left (690), LEFT.tsv
- **237 unproven**: 172 NOALIGN (slot in an anchor gap whose 360 count differs from TLC, or the class is absent from the 360 PDB:
  NUISystem lists / menus were rewritten for Anniversary, CTCD* debug components), 34 SECONDARY (multiple-inheritance secondary
  vtables, not aligned), 30 NOVT (address in no RTTI vtable the census reads: non-RTTI vtables or a cut slot list), 1 NOPDBSYM.
- **445 name PROVEN, row not respelled** (symbol given in LEFT.tsv): bodies that need real callees or members (OTHER 246, JMP 50
  tail calls to helpers such as `FinalCleanup` whose own names are unproven propagated labels, FWDV 9 slot forwards, getters whose
  retail offset is not the Ego_r offset, `const T&` getters that return a global's address), and header classes whose header
  would not take the declaration (hand headers needing other opt-ins; `#error` include chains that hang cl).
  Next: the same generator with body text carried over from the landed source onto the header class (most OTHER rows are already
  byte-exact stand-ins); the proven names can also go to the catalog now.
- **8 WEAK_ALIGNMENT** (withheld, symbol listed): gaps of kind GSCNX/GTRAIL or a gap holding a TLC-named slot the 360 order does not
  place (0x0042D5AA 0x0084FD10 0x0053FC00 0x0052F150 0x0052F160 0x0084FBF0 0x007B5010 0x005419F0).

## NAME_FIXES
- Per renamed address, every landed binder with another spelling (reloc_bindings.tsv), for the coordinator respell.
- 0x00644627 is bound as `?ClearPrimitivesIfNecessary@CDrawBase@NPlayerGui@@UAEXXZ`, but it is slot 0 (deleting destructor) of 23
  NPlayerGui::CDraw* vtables; ClearPrimitivesIfNecessary is slot 1 = 0x00442760 (renamed this pass). Correct dtor spelling not settled.

## Rule notes
- Grades per row: STATUS.tsv notes. Rows of header classes went through several prologue tries (most SRC_FAIL compile failures
  while searching for the right opt-ins); **rule slip: 130 renamed rows exceed the 10-grade budget this way** (10..18 attempts,
  nearly all SRC_FAIL compiles of wrong prologue variants; count in STATUS notes, `work/gradelog_all.tsv`).
  No row needed more than one grade once its spelling form was settled.
- Local pads: headerless classes declare `unsigned char _base_0x04[N]` before the one member a getter reads (no shared header exists
  for those classes); member names and types are Ego_r's.
- No descriptive names introduced: every symbol is in the Ego_r / FableWin name sets.

## Header debt
- NCreatureMode::CMode*, NCameraModes::C* (except CBase / CStringAndCage*), NParticleEngine::CPSC*, NEntityEvents::CEvent*,
  NHitLocations::CPhysicalPrimitive*: no shared headers (the rows declare the class locally).
- Flat generated headers (CTCPhysicsRigidBody and others) model the vptr as data; the VSLOTS opt-in is the first polymorphic view.

## Scripts (work/)
dump360.py (360 vtables), pdbvirt.py (name-set index), census.py, align2.py, fam.py, retchk.py, shape.py, collide.py, badgap.py,
gen1.py (headerless), gen2.py (header prologues; grader with timeout), gen3.py / ovl.py and gen6.py / ovl2.py (VSLOTS overlays),
gen4.py (GetAnimFilter), gen5.py (member getters), grade.py, testrun.py, gcount.py, hdrreadme.py, finalize.py.
