# Chunk 696 (NA039) -- 10 of 10 byte-exact; nine of ten catalog labels were wrong; ~CScriptThing 0x004AA840 lands (2026-11-01)

All ten rows landed (`verify.log`: WINS 10, 1 MATCH + 9 RELOCATION_MATCH, PASS; vland's jump-table check clean on the switch row; bindings
multi-check 0): ~CScriptThing 0x004AA840 (66, 4,187 call sites; was "~C3DClothPrimitive", copied from its byte twin 0x00AC3B80, which stores the
C3DClothPrimitive vtable and is a different symbol), NUISystem::CClickable::Update 0x0055ACB0 (vtable 0x0124BD2C slot 1, RTTI from Fable.exe),
CTCInventoryAbilities::GetCategory 0x005EDE37 (the one right label; size-optimised TU, `#pragma optimize("s", on)` as 6,918 landed rows), ~CTCAnimationComplex
0x0070B5B0 (was "~CAInputProcess"), the implicit NCreatureMode::CModeJackOfBladesScream::~ 0x00740540 / CModeBriarRoseTeleportInvulnerable::~
0x008C36D0 / ~CSpawnedFuncBase 0x00CDD4C0 / NScript::CV_TalentlessBardScript::CTB_Bard::~ 0x00ED7DD0 (all four labelled "_Dest_val<CMacro>"; the
one-line ungraded DestroyOne helper of chunks 656/657 makes VC emit the out-of-line `??1`), CMemoryDataOutputStream::PeekData 0x00995E70 (MATCH;
`size() > 0`, not `empty()`) and ~CManagedTexture 0x00A69E20 (was "CEnginePrimitiveHandle::Clear"). New binding 0x00CDD4F0 =
??1?$CCountedPointer@$$CBVCPredicateBase@NScript@@@@QAE@XZ (vland RELOC_NAME warning until the label applied here).

14 labels applied (`ghidra_out/labels_chunk696_2026-10-27.tsv`); NAME_FIXES.tsv 14 rows + 2 coordinator rows queued c696 for naming wave 28: the ten
spellings bound to 0x004AA840 (one non-virtual QAE, nine invented classes), twelve invented ~CTCBase 0x00686830 bindings, the landed 0x00ED7DB0
(a CLandscapeBackgroundPatch vector deleting dtor stand-in; it is ??_GCTB_Bard@CV_TalentlessBardScript@NScript@@UAEPAXI@Z), ~CEntityScriptBase
0x00F35B40's wrong spellings, PeekData / CClickable::Update / ~CSpawnedFuncBase binders. Decisions (coordinator): the landed caller respells go to
naming wave 28 rather than this landing (no new name collides with a landed spelling, so nothing forces them here); the 14 sources declaring an
invented virtual ~CSpawnedFuncBaseRuntime move to CSpawnedFuncBase; engine/CSpawnedFuncBase.h gets the implicit dtor and real member types (all
includers rechecked). Header debt (NOTES.md): CMemoryDataOutputStream.h Data as opaque bytes; CDataOutputStream.h / CTCBase.h / CModeStandBase.h
/ CHoverable.h flatten the vptr (the last two also `struct` and CHoverable outside NUISystem); CCountedPointer.h's inline dtor expands where
retail calls 0x00CDD4F0.
