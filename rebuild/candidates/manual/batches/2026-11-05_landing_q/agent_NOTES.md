# chunk 790 notes

Four rows RELOCATION_MATCH (src/ + test/) and two parked (differ/ + .test.cpp). All six names are PDB-proven: each
symbol is in both the Ego_r and FableWin name sets, the vtable slot or spawn site is checked against retail, and objdump -t
confirms the emitted symbol. No descriptive names.

## 0x00D26190 NScript::CQ_DragonBossFightScript::DoMission -- RELOCATION_MATCH (4 attempts)
- Shape: generated level-script method that spawns RunEnemySpawning and JackTaunts as CSpawnedFunc threads and waits on
  DragonState (+0x48) == 4. Cutscene flags go through cs_flags["PutMaskOn"] (+0x84). It writes MasterScriptData
  BodyGuardsInLimbo / HeroWoreMask and has one shared exit.
- Key spellings:
  - The ~vector<CScriptThing> is a declared-only `template <> std::vector<CScriptThing, ...>::~vector();`, so it is called at
    0x008AC970 rather than expanded at the shared exit.
  - The morality test constant is 0.5f (0x0122F59C). q.py masks it; fltchk found it.
- New header: hdr/engine/CQ_DragonBossFightScript.h.
- Callee 0x00D26BA0 had no label. It is RunEnemySpawning (see LABELS.tsv).

## 0x00E93120 NScript::CV_HiddenBootyScript::CM_HiddenBooty::Main -- RELOCATION_MATCH (2)
- FABLE_RETAIL_CDEFSTRING_OUTOFLINE_CONVERSION makes CDefString::operator CCharString() a call to 0x00415D70, as in retail.
- The inline CIsThingAlive ctor calls the CScriptThing copy 0x004ABE90. That copy is not defined inline in this TU.

## 0x00EE95C0 NScript::CExpression_FishScript::Main -- RELOCATION_MATCH (16)
- __forceinline CScriptThing::GetPos.
- The squared distance is spelled `X*X + Y*Y + Z*Z`. Five other operand orders and the right-nested forms give 6-10 difflines.
- The radius test is `10.0f * 10.0f`, which folds to 100.0f.
- New header: hdr/engine/CExpression_FishScript.h. This is a global expression script: its base is CScriptBase and it has its
  own Game pointer.

## 0x00D78670 NScript::CQ_HeroSoulsScript::Main -- RELOCATION_MATCH (first grade)
- New header: hdr/engine/CQ_HeroSoulsScript.h.
- The nested CNotForODG / CEndScythe / CEndBriarRose are defined locally, each with only its static FASTCALL Alloc, for the three
  CEntityScriptBinding<CQ_HeroSoulsScript> bindings.
- The generated level-script spelling (an IsActiveThreadTerminating guard at the top of every if-body, NewScriptFrame wait loops)
  matched on the first grade.
- strchk 82/82. No float constant is loaded from memory.

## 0x00E64210 NScript::CV_ChickenKickingScript::CKickedChicken::Main -- DIFFER(2803v2803) 8md, parked (52 attempts)
- Shape learned (each item moved the diff from 579 to 8 difflines):
  - The sound CCharString needs its own statement.
  - The foul / hero copies use the implicit operator=. The copy ctor lets VC7.1 drop the unused Z store.
  - The landing wait is a bool-flag loop. while(true)+break rotates it.
  - The angle dot is a running sum.
  - acos * INV_TWO_PI and * 0.5f are separate statements, with a static const float for INV_TWO_PI. The literal product folds to
    0.0796, which fltchk catches.
  - ++sector is its own statement, not part of the for-increment.
- Residue: operand order in the final `direction.Dot(a)`. VC7.1 ignores the source operand order. In the retail term order
  Y,Z,X it loads direction first for Y and X; retail loads a first in all three products.
- Callee names come from reloc_bindings.tsv's correct spelling, not the first (junk) spelling there:
  - 0x0099A2E0: first spelling ?OnDie@CInventoryItemBase. Here it is the empty base-class dtor reached from ~CScriptThing (bound under several spellings, including ??1CBase@NParticleEngine@@QAE@XZ).
  - 0x004AA840: first spelling ~ScriptMethods. It is ~CScriptThing.
  - 0x008AC970: first spelling CVector_008ac970. It is ~vector<CScriptThing>.
  - 0x00A13C60: first spelling CQuakeVector::Rotate. It is C3DVector::Rotate.
  - 0x008ADF90: first spelling ScriptVector::PushBack. It is vector<CScriptThing>::push_back.
  - The coordinator may want to retire those first spellings: ann.py and other first-match tools show them.
- Label: 0x00CBE551 is GetDistanceBetweenThingAndPosition (LABELS.tsv).
- Name fix: CChickenSign::Main's +0x5c member is PrizesWon, not FinalSector (NAME_FIXES.tsv).

## 0x00DC5F80 NScript::CQ_OpeningGraveyardSecretPassageScript::CSecretPassageWoman::Main -- DIFFER(4259v4245) 36md, parked (10)
- Key spellings:
  - The declared-only `template <> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();` makes the dtor a call
    to 0x006E7AB0.
  - The second loop is `if (T) return; while (true) { ...; NSF; T }`, so the second movie ctor is the Construct call 0x006E7B60.
  - The movie and pause sit in an inner block that closes before MoveToPosition.
- Residue (GOTCHAS 1913/1933, open): inside the expanded CIsThingConscious predicate ctor/dtor, retail calls the CScriptThing
  copy 0x004ABE90 and dtor 0x004AA840. Our build expands them, because the same TU must expand the copy at the four by-value
  Set*Wander* pushes. Knock-on: retail keeps ebp == 0 for the whole function.
- The oracle row spells the class `CSecretPass`, a truncated name. The PDB and labels_anniversary give CSecretPassageWoman.

## Header debt
None. Three new opt-in headers; no existing header changed (hdr/README.md).
