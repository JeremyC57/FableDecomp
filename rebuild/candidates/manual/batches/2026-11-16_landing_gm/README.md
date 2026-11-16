# 2026-11-16 landing gm -- naming-blocked rows, part 2 (eight rows + one respelling)

Owner: "do those 13 blocked on naming now." The last eight of the thirteen (part 1: landing gl). All RELOCATION_MATCH (0x00ADD5C0
MATCH) with their behaviour tests passing, `LANDVERIFY_STRICT_HEADERS=1`, repo headers (`logs/vland_land.log`).

| row | function | name evidence |
|---|---|---|
| 0x00702E00 | CThingObject::GetAlpha | PROVEN: Ego_r.pdb ?GetAlpha@CThingObject@@IBEEXZ 0x005522B0 between DrawGetEditDrawInfo and GetProperties, as retail sits between the landed 0x00702D10 / 0x00702FF0; FableWin.pdb the same order with GFGetUnsignedAngleDifference right after it (the helper the body expands); `E` return + const fit. Ported from local stand-ins (CHeadingSource / CThingHeadingGauge) to CThingObject.h + CTCPhysicsBase.h (GetFacingVector) + CEnvironment.h (PeekSunVector) + C3DVector.h (GetAngleXY) + GFAngles.h |
| 0x00C11700 | CSoundEventManager::GetFaderOutput | DESCRIPTIVE: `add ecx,7D8h ; jmp CFader::GetOutput`; owner from its only caller 0x00C271E0 (CPhysicalPlayer::UpdateManagerTime(CSoundEventManager*), as the landed Play 0x00C271F0 binds it) and the manager ctor / dtor (CFader at +0x7D8). The callee is a pure getter, so a getter name (acctC's "GetTime() (double)" proposal does not fit the float output) |
| 0x00C14700 | CVtable_012b401c::~CVtable_012b401c | implicit dtor of the manager's +0x7B8 worker part: ctor 0x00C26A30 (bound by 0x00C11D00, stored at +0x7B8), the manager dtor 0x00C13470 inlines this dtor for +0x7B8 (`lea ecx,[edi+0Ch] ; call 0x00C14630`). Landed class spelling kept |
| 0x00C14710 | CVtable_012b4030::~CVtable_012b4030 | as 0x00C14700 for +0x7B4 (ctor 0x00C26E90, queue 0x00C14560) |
| 0x00C2D9B0 | SSegment::~SSegment | DESCRIPTIVE, the landed CSegmentFile::OpenSegment model's entry struct; CSegmentFile::~CSegmentFile 0x00C2DBE0 inlines this dtor (the only `lea ecx,[reg+8] ; call 0x00C0BA40` in the image, 0x00C2DC2C) |
| 0x00C47EA0 | SOpenArgs::~SOpenArgs | DESCRIPTIVE: its one reference is unwind funclet 0x01215920 (state 2 of FuncInfo 0x0136FC80), whose handler is pushed by the landed CAsyncDecoder_00c48640::Run 0x00C47F10; state 2 there is the open-request copy Run models as SOpenArgs |
| 0x00C48740 | SDecoderMessage::~SDecoderMessage | the spelling the landed PKCStaticQueue<SDecoderMessage> ctor 0x00C48F80 already binds; the +0x14 member is Run's request name pointer |
| 0x00ADD5C0 | CCountedPointer<CThreadedFile>::IsNull | INFERRED: the CLHFile TU's only pointer-first object is the CLHFile's CCountedPointer<CThreadedFile> (a CLHFile's own first word is its vptr); IsNull is the engine CCountedPointer member (FableWin.pdb, 343 T's), `return Data == 0`. No PDB covers LHA, so T is not PDB-confirmed |

Respelled: the landed 0x00C271E0 (was `_global::Cons_val(Alloc*, Pair*, Pair*)` calling `cons_ctor(Pair*)`, a propagated
_Cons_val label) is CPhysicalPlayer::UpdateManagerTime(CSoundEventManager*) -- the spelling the landed CPhysicalPlayer::Play 0x00C271F0
declares and binds (descriptive; the body only reads the manager's fader output and drops it). Files renamed to
`CPhysicalPlayer_UpdateManagerTime_00c271e0.*`, on libpk/CSoundEventManager.h; recheck RELOCATION_MATCH + PASS
(`logs/vland_recheck_00c271e0.log`); its reloc_bindings.tsv line updated to the new callee spelling.

Shared callee 0x00C167C0 (an owner-pointer destructor `if (p) { delete p; p = 0; }` with nine bound spellings and a landed stand-in row
`CActiveFile_OnReadFinished`): 0x00C47EA0 / 0x00C48740 bind `??1CIntelligentPointer_00c16900@@QAE@XZ`, the spelling Run binds along with
that class's copy ctor 0x00C16900 and Get 0x00C168A0; the other spellings are queued (`NAME_FIXES.tsv`). vland WARN RELOC_NAME on it is
the catalog label of 0x00C167C0.

Implicit destructors: 0x00C14700 / 0x00C14710 / 0x00C2D9B0 / 0x00C47EA0 / 0x00C48740 are compiler-generated (a user-written `{}` on the
polymorphic CVtable_* classes stores the vptr first; retail does not). Each row carries an `Emit...Destructor` helper (`delete p;`) that
only makes the TU emit the implicit ??1, marked "not a retail function", as the landed precedents 0x00695F40 / 0x006F48A0 / 0x00AF6570.

Headers (`hdr_notes/README.md`): new libpk/CFader.h; opt-ins in libpk/CSoundEventManager.h, engine/CThingObject.h, engine/GFAngles.h.
pincheck 0 mismatches; whole-tree equivalence (exact): only 0x00702E00 / 0x00C11700 / 0x00C271E0 (their new opt-ins) and the
`__FILE__`-path-only 0x00CBFB7D / 0x0047042A differ (`logs/equivalence_changed_input_rows.tsv`).

With landing gl this closes all thirteen naming-blocked rows from the w24 triage.
