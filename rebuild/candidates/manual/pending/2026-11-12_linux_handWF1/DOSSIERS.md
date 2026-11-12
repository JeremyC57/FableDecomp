## 0x00A114B0

# DOSSIER 0x00A114B0 CPhysicsMeshObject::SetPositionMatrix (338 B)

## 1. Retail shape (xdis.py 00a114b0 0x162)
- __thiscall (this=ecx, never moved), `ret 0Ch` = 3 stack args (position*, axes*, float scale). No frame pointer (no push ebp), no SEH, no callee-saved pushes. `sub esp,30h` = 48-byte frame = FOUR C3DVector locals:
  [esp+0..8] position copy (X never copied: read straight from memory; Y,Z copied via edx/eax to +4/+8),
  [esp+0xC..0x14] right (only Z spilled, to +0x14), [esp+0x18..0x20] forward copy (Z at +0x20), [esp+0x24..0x2C] up copy (Z at +0x2C).
- Order: (a) load Up.X, Up.Y, Fwd.X, Fwd.Y on x87, Up.Z / Fwd.Z through edx to frame; (b) cross product read from axes MEMORY (not from copies): rX=Fwd.Y*Up.Z-Fwd.Z*Up.Y, rY=Fwd.Z*Up.X-Fwd.X*Up.Z, rZ=Fwd.X*Up.Y-Up.X*Fwd.Y (= Forward ^ Up; note rZ operand spelled `fld Fwd.X fmul Up.Y ; fld Up.X fmul Fwd.Y` -- header operator^ is X*o.Y-Y*o.X, i.e. Fwd.Y*Up.X, compiler may commute);
  (c) identity, zeros stored in REVERSE order E43,E42,E41,E32,E31,E23,E21,E13,E12 (xor eax) then ones E33,E22,E11 (eax=3F800000h) => chained assignment `E12=E13=E21=E23=E31=E32=E41=E42=E43=0.0f; E11=E22=E33=1.0f;`;
  (d) translation: `fld pos.X ; fadd E41 ; fstp E41`, then pos.Y/pos.Z from the frame copy `fld [esp+4] ; fadd E42` etc (copy-then-add, X+E41 operand order);
  (e) E33 = Up.Z (int mov from frame) BEFORE the x87 row stores; then E11..E13 = -right, E21..E23 = -forward (fchs), E31,E32 = Up.X,Up.Y;
  (f) nine `fld scale ; fmul Eij ; fstp Eij` in E11..E33 row-major order (scale reloaded each time from [esp+3Ch], i.e. NOT folded into row stores);
  (g) Scale(+0x38)=scale via eax (int copy, loaded at 0x00A1159F mid-scale-sequence), OO_Scale(+0x3C)= 1.0f(0x0122DED8)/scale.
- No calls, no size-optimised idioms. Plain /O2 style; no pragma evidence.

## 2. Identity (name proven)
- Mangled: `?SetPositionMatrix@CPhysicsMeshObject@@QAEXABVC3DVector@@ABVCRightHandedSet@@M@Z`
  => `void CPhysicsMeshObject::SetPositionMatrix(const C3DVector& position, const CRightHandedSet& axes, float scale)` (non-const, non-virtual, public).
- Sources: (1) Ego_r.pdb egor_pdb_names.tsv:33619 (Ego_r 0x00B7EF10); (2) FableWin.pdb _fablewin_api.tsv:289909 (0x031D1440, beside ?TransformPlane@CMatrix3x4 0x031D2030 which = retail 0x00A112F0 just before this row, labels_sweep3_2026-10-26.tsv:66); (3) layout: stores to +8..+0x34 / +0x38 / +0x3C match CPhysicsMeshObject (PositionMatrix +8, Scale +0x38, OO_Scale +0x3C) and NOT C3DMeshObject (PositionMatrix +0x10, Scale +0x74); (4) sole caller 0x00A12D13 is in landed ctor rebuild/src/compiled/00/a1/CPhysicsMeshObject_CPhysicsMeshObject_00a12cc0.cpp, which declares this exact signature. Manifest label is CORRECT.
- Parameter names: ctor caller uses `orientation`; old draft `axes`. PDB has no param names in TSVs.
- TWIN: C3DMeshObject::SetPositionMatrix retail 0x00A357B0 (318 B) is the SAME code (PositionMatrix at +0x10, no Scale/OO_Scale stores). Parked: rebuild/candidates/manual/pending/2026-10-31_parked_retry2_sec5/differ/00a357b0.cpp (307v318). Its lesson: a flat model folds `*= scale` into row stores; a loop `float* e=&m.E11; for(k<9) e[k]*=scale;` gives retail's reload sequence. Residue there = cross operand order + translate copy scheduling. Same source pattern almost certainly (shared inline CMatrix3x4/CRightHandedSet helpers).

## 3. Callees
- None (no call/jmp). Data: 1.0f @0x0122DED8. Inline helpers likely used (FableWin.pdb has inline copies, no Ego_r/retail out-of-line): ?SetIdentity@CMatrix3x4@@QAEXXZ (FableWin 0x017F0E5E), ?SetTranslate@CMatrix3x4@@QAEXABVC3DVector@@@Z (0x017F4031), ?PostTranslate@CMatrix3x4@@QAEXABVC3DVector@@@Z, ?PreScale@CMatrix3x4@@QAEXM@Z, ?Set@CMatrix3x4@@QAEXABVC3DVector@@000@Z, ?GetRight@CRightHandedSet@@QBE?AVC3DVector@@XZ (0x0181243C), ?GetUp@CRightHandedSet (0x017A3C0D), ??GC3DVector@@QBE?BV0@XZ (unary minus, 0x01797688), ??TC3DVector (cross ^).
- Old draft spelling: free `CrossProduct()` -- not a PDB name; real is C3DVector::operator^ / CRightHandedSet::GetRight (Forward ^ Up).

## 4. Members / headers
- this: rebuild/include/engine/CPhysicsMeshObject.h (CBaseClass base, size 0x40). Mesh +4 (CIVCountedPointer<const CPhysicsMesh>, opt-in FABLE_RETAIL_CPHYSICSMESHOBJECT), PositionMatrix +8 CMatrix3x4 (opt-in FABLE_RETAIL_CPHYSICSMESHOBJECT_MATRIX, else unsigned char[48]), Scale +0x38 float, OO_Scale +0x3C float (struct_layouts_egor.tsv). PositionMatrix/Scale/OO_Scale are PRIVATE and the header does NOT declare SetPositionMatrix -> a new opt-in (e.g. FABLE_RETAIL_CPHYSICSMESHOBJECT_SETPOSITIONMATRIX declaring the member) is needed.
- CMatrix3x4: rebuild/include/engine/CMatrix3x4.h -- flat E11..E43 (or FABLE_RETAIL_CMATRIX3X4_DATA_UNION union with Data[4][3], the PDB form). FABLE_RETAIL_CMATRIX3X4 offers __forceinline Set(4 vectors), Set(12 floats), PreScale(float) (row-major E11..E33 = retail scale order), PostScale, PostTranslate(const C3DVector&) (`E41+=v.X` -> `fld E4k ; fadd v`: OPPOSITE of retail here), SetInverse. FABLE_RETAIL_CMATRIX3X4_SETTRANSLATEONLY gives SetTranslateOnly. NO SetIdentity / SetTranslate in the header (would need a new opt-in; chained form is proven in landed 0x00AF0380 and 0x00BEACA0 local models).
- CRightHandedSet: rebuild/include/engine/CRightHandedSet.h, Up +0, Forward +0xC (protected). FABLE_RETAIL_CRIGHTHANDEDSET gives GetUp(), GetForward(), GetRight() = `Forward ^ Up` (or _RIGHT_COMPONENTS explicit form). 
- C3DVector: rebuild/include/engine/C3DVector.h; FABLE_RETAIL_C3DVECTOR gives operator-() (unary); operator^ variants FABLE_RETAIL_C3DVECTOR_CROSS_XY_GROUP / _CROSS_X_GROUP.
- Old draft models LOCALLY (all have shared headers): C3DVector, CRightHandedSet, CMatrix3x4, CPhysicsMeshObject (also local CPhysicsMesh fwd).

## 5. Visibility
- No out-of-line callees. All helpers inline (no retail COMDAT): FableWin-only copies of SetIdentity/SetTranslate/PreScale/GetRight. Header policy (owner 2026-11-01): Lionhead math helpers with no out-of-line copy -> FABLE_FORCEINLINE.
- Same TU neighbours (retail 0x00A11xxx-0x00A12Dxx, CPhysicsMeshObject.cpp): TransformPlane 0x00A112F0 (unlanded, CMatrix3x4 member emitted here), GetWorldPosition 0x00A11430, IsRigidBodyBoundingPointsValid 0x00A11610, GetHelperPointPositionStatic 0x00A11760, GetBoundingBoxCentre/X/Y/ZEdge 0x00A11AE0..0x00A11C90, dtor 0x00A127D0, ctor 0x00A12CC0.

## 6. Sibling landed rows (rebuild/src/compiled/00/a1/)
- CPhysicsMeshObject_GetBoundingBoxXEdge_00a11c20.cpp: `// Flags: none`; defines FABLE_RETAIL_DEF_CLASS_BASE, _C3DVECTOR, _C3DBOUNDINGBOX, _CMATRIX3X4_TRANSFORMVECTOR, _CIVCOUNTEDPOINTER, _CPHYSICSMESHOBJECT, _CPHYSICSMESHOBJECT_MATRIX, _CPHYSICSMESHOBJECT_EDGES; includes engine/C3DBoundingBox.h, engine/CPhysicsMeshObject.h.
- CPhysicsMeshObject_GetBoundingBoxCentre_00a11ae0.cpp: Flags none; same set + FABLE_RETAIL_CMATRIX3X4 + _CPHYSICSMESHOBJECT_CENTRE.
- CPhysicsMeshObject_CPhysicsMeshObject_00a12cc0.cpp (caller): local CPhysicsMeshObject model (float PositionMatrix[12]) -- no pragmas.
- CPhysicsMeshObject_IsRigidBodyBoundingPointsValid_00a11610.cpp, _GetHelperPointPositionStatic_00a11760.cpp: engine/C3DVector.h only.
- No pragmas in any sibling.

## 7. Old draft model errors (vs disassembly)
- Identity written row-by-row (E11=1,E12=0,...) -- retail's reverse zero order is the CHAINED SetIdentity; translation zeroes are part of it.
- Frame: retail has 4 C3DVector locals (pos copy, right, forward, up); draft's `C3DVector p = position` matches the pos copy; right is a named local (Z spilled), consistent with `C3DVector right = axes.GetRight()` / `Forward ^ Up` then negated at store via unary minus (or -right temporaries). Retail computes the cross from axes memory, but up/forward copies are taken first (before cross): declaration order up, forward, right, OR a helper taking them by value.
- Translation operand order `fld pos.X ; fadd E41` = pos.X + E41 (draft `E41 += p.X`; the twin's 10-31 note lists this as residue). Try `E41 = p.X + E41` or a SetTranslate helper (FableWin ?SetTranslate@CMatrix3x4@@QAEXABVC3DVector@@@Z: likely SetIdentity + add/assign).
- Scale: draft's nine `*= scale` statements risk folding (twin lesson); retail = row-major PreScale-shape reload; header PreScale(float) is that order; else twin's float* loop.
- E33=Up.Z stored by integer move before the x87 row stores: consequence of up being a copied local with Z in frame.
- Local struct models must be replaced by the shared headers (+ new opt-ins for SetIdentity/SetTranslate and the member declaration).

## 0x007FF190

# DOSSIER 0x007FF190 CTCNamedUserActionPoints::GetFreeNamedActionPoint (343 bytes)

## 1. Retail shape (xdis 007ff190 0x167)
- Extent 0x007FF190..0x007FF2E6 = 343 bytes (0x157); int3 padding then 0x007FF2F0 (vtable slot 4 of CTCNamedUserActionPoints). Old draft header said 341 -- wrong.
- __thiscall, `ret 1Ch` (7 dword args). No frame pointer (`sub esp,8; push ebx/ebp/esi/edi`), no SEH, plain /O2 (aligned loop head: `jmp 7ff1f0` + `lea ebx,[ebx]` pad). No pragmas needed.
- Stack args (esp after prologue): +1c thing, +20 position, +24 orientation, +28 name (CCharString by value, PStringData), +2c index&, +30 start, +34 checkBlocked.
- Locals 8 bytes at esp+10 = the CIntelligentPointer<CThing> temporary (vptr 0x01238C6C).
- Prologue: temp ctor 0xA01B10, vptr store, temp.SetPItem(&thing) 0xA01B90, temp.GetPItem 0xA01B50, `lea ebx,[esi+2Ch]` (ebx = &PThing, kept live all function), PThing.SetPItem, temp dtor 0xA01C10.
- i = start: kept in EBP with a frame copy in the dead `thing` slot [esp+1c] (written at init and at every ++i); the copy's address is find's value arg and push_back's arg. checkBlocked is re-read from its slot [esp+34] each time (`mov al,[esp+34h]`). Find's empty tag temporary is homed at [esp+34] (the checkBlocked slot) -- same trick as the landed 0x007FEC30 / 0x007FEFE0.
- Loop: `if (start >= NumberOfActionPoints) goto end;` then rotated do-while (for loop), end test re-reads [esi+24].
- Body order: name text (edx; null -> 0x0129AAF4 NullChar; else [PStringData]) interleaved with element addr `Names._M_start + i*4`, then element text; `strstr(element, name)`; je continue.
  `edi = Free._M_finish; ecx = Free._M_start; edx = edi; push &tag; push &i_copy; call 0x50E540`; `cmp eax,edi; je continue`.
  Inline erase: `next=pos+1; if (edi != next) memmove(pos,next,edi-next)` (uses the edi still-cached finish, NOT a reload), then reload [esi+10], add -4, store.
  `call 0x7FEC30(i)` (non-virtual base IsActionPointIndexBlocked(long)); `if (al && checkBlocked) continue;`
  `owner = PThing.GetPItem()` (mov ecx,ebx); virtual `[edx+70h]`(owner, position, orientation, i); if false -> `*index = i; return true`; if true and !checkBlocked -> same return true; else `Blocked.push_back(i_copy)` out of line 0x00485FF3 (ecx = esi+18).
- Two epilogues each with ~CCharString(name) 0x99EAE0 (callee destroys the by-value string): false path (`xor al,al`) and true path (`mov [index],ebp; mov al,1`).

## 2. Identity (manifest label CORRECT)
- Ego_r.pdb (ghidra_out/egor_pdb_names.tsv 007f2a00): `?GetFreeNamedActionPoint@CTCNamedUserActionPoints@@QAE_NAAVCThing@@AAVC3DVector@@AAVCRightHandedSet@@VCCharString@@AAJJ_N@Z`
- FableWin.pdb (ghidra_out/_fablewin_api.tsv 017dd0d4 / 02589e10; naming_stragglers/FableWin.exe.signatures.tsv): same mangled name.
- Order cross-check: Ego_r IsActionPointIndexBlocked(virt) 7f2930 -> GetFreeNamedActionPoint 7f2a00 (0x160 long) -> OnCreate 7f2b60; retail virt IsActionPointIndexBlocked 0x7FF0C0 (vtable slot 28) -> 0x7FF190 (343) -> 0x7FF2F0 (slot 4). Landed 0x007FEC30 row also names 0x7FF263 as the caller site in GetFreeNamedActionPoint.
- Signature: `public: bool CTCNamedUserActionPoints::GetFreeNamedActionPoint(CThing&, C3DVector&, CRightHandedSet&, CCharString, long&, long, bool)` (non-const, __thiscall). OLD DRAFT USED `CThing* user` -- WRONG, PDB says CThing& (AAVCThing@@). Caller CTCTavernBar_GetPickupSlot_007ba000.cpp:43 declares defaults `long = 0, bool = true` (it calls with 5 args). Caller CAIStateGroupTavernKeeper_GetOldJugPoint_00968860.cpp:66 declares it with pointers (CThing*, C3DVector*, ...long*) -- wrong spelling vs PDB, NAME_FIXES candidate (same ABI).
- 14 call sites (xref): 007b88d6 007b8ba2 007b8db2 007b8fe7 007b917b 007ba167 007ba357 007ba4a5 007ba5c0 007baa36 007d8443 008f12b6 00967390 0096892c.

## 3. Callees
| addr | landed row | landed / PDB name | draft spelling |
|---|---|---|---|
| 0x00A01B10 | 00/a0/CBaseIntelligentPointer_CBaseIntelligentPointer_00a01b10.cpp | CBaseIntelligentPointer::CBaseIntelligentPointer() | ok |
| 0x00A01B90 | 00/a0/CBaseIntelligentPointer_SetPItem_00a01b90.cpp | SetPItem(CBaseObject*) | ok |
| 0x00A01B50 | 00/a0/CBaseIntelligentPointer_GetPItem_00a01b50.cpp | `CBaseObject* GetPItem() const` | draft returned CThing* -- wrong |
| 0x00A01C10 | 00/a0/CBaseIntelligentPointer_CBaseIntelligentPointer_00a01c10.cpp | `virtual ~CBaseIntelligentPointer()` | draft non-virtual dtor + `virtual void v00()` -- wrong |
| 0x00BFEB3A | 00/bf/global_strstr_00bfeb3a.cpp | CRT strstr | ok |
| 0x0050E540 | 00/50/global_Find0050e540_0050e540.cpp | landed `Find_0050e540` (stale); bound PDB name `??$__find@PAJJ@std@@YIPAJPAJ0ABJABUrandom_access_iterator_tag@0@@Z` = std::__find<long*,long> (per 0x007FEC30 notes) | draft `find` (global) -- not PDB spelling |
| 0x00BFEAE6 | 00/bf/global_memmove_00bfeae6.cpp | memmove (inline vector::erase) | ok |
| 0x007FEC30 | 00/7f/CTCUserActionPointBase_IsActionPointIndexBlocked_007fec30.cpp | `?IsActionPointIndexBlocked@CTCUserActionPointBase@@IAE_NJ@Z` protected, non-const | draft `IsActionPointIndexInBlockedList` -- MADE-UP, WRONG |
| vtbl+0x70 (slot 28) | 00/7f/CTCNamedUserActionPoints_IsActionPointIndexBlocked_007ff0c0.cpp | `?IsActionPointIndexBlocked@CTCNamedUserActionPoints@@UBE_NAAVCThing@@AAVC3DVector@@AAVCRightHandedSet@@J@Z` (virtual const) | ok |
| 0x00485FF3 | 00/48/stdvector_pushback_00485ff3.cpp | `?push_back@?$vector@JV?$allocator@J@std@@@std@@QAEXABJ@Z` (COMDAT from a /Oy- /Os TU; here called OUT OF LINE) | draft CLongVector::push_back |
| 0x0099EAE0 | 00/99/CCharString_CCharString_0099eae0.cpp | ~CCharString | ok |
- Name-hiding trap: the derived class declares the virtual IsActionPointIndexBlocked(CThing&,...) so an unqualified `IsActionPointIndexBlocked(i)` will not find the base's (long) overload; write `CTCUserActionPointBase::IsActionPointIndexBlocked(i)` (direct call either way, it is non-virtual) or model both overloads in the local base.

## 4. Members
| off | name (struct_layouts_egor.tsv; Ego_r offsets +4 because of 16-byte vectors) | type |
|---|---|---|
| +0x0c | CTCUserActionPointBase::FreeActionPointIndexes (Ego_r +12) | std::vector<long> |
| +0x18 | BlockedActionPointIndexes (Ego_r +28) | std::vector<long> |
| +0x24 | NumberOfActionPoints (Ego_r +44) | long |
| +0x2c | PThing (Ego_r +52) | CIntelligentPointer<CThing> |
| +0x34 | CTCNamedUserActionPoints::Names (Ego_r +60) | std::vector<CCharString> |
- Shared headers: rebuild/include/engine/CTCNamedUserActionPoints.h exists but is all `_pad` (only Names named, as `unsigned char[0xc]`), no FABLE_RETAIL macros -- not usable for the base members; there is NO CTCUserActionPointBase.h. Siblings model the class locally (fine, as all landed siblings do).
- Usable shared headers: engine/CIntelligentPointer.h (CBaseIntelligentPointer exact; CIntelligentPointer<T>(T*) ctor via SetPointer, `FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN` = item-read-then-SetPItem operator=, Get()); engine/C3DVector.h; engine/CRightHandedSet.h (FABLE_RETAIL_CRIGHTHANDEDSET...; only by reference here); fable_string.h with `#define FABLE_RETAIL_CCHARSTRING` (GetAsCharArray = ternary NullChar; FABLE_RETAIL_CCHARSTRING_CONTAINS gives forceinline `Contains(other)` = strstr(this,other)!=0 -- optional); engine/CTCBase.h.
- Draft models locally (headers exist): C3DVector, CRightHandedSet, CCharString/CCharStringData, CBaseIntelligentPointer/CIntelligentPointer. Note the draft's CRightHandedSet member order differs from header (header: Up, Forward) -- irrelevant (reference only).

## 5. Callee visibility
- __find<long*,long> 0x50E540: visible in retail's TU (retail caches Free._M_finish in edi across the call and reuses it in erase; i stays in ebp across the call). Use the landed same-TU noinline template `Find` body exactly as in sibling 0x007FEFE0 / 0x007FEDE0 (template, __declspec(noinline), __fastcall, CIteratorTag). fable_algorithm.h's FABLE_RETAIL_STLP_FIND_RANDOM_DISPATCH is decl-only unless FABLE_RETAIL_CTCTARGETINGPLAYER_UPDATE_LIST (then forceable inline) -- decl-only would make i escape; don't use.
- push_back 0x485FF3: called out of line here, yet the same-TU sibling GetFreeActionPoint 0x7FEDE0 inlines push_back. The visible-but-noinline form is what keeps i in a register (c2 must see push_back does not keep &i). stlport/fable_containers.h:466 `FABLE_RETAIL_STLP_VECTOR_PUSHBACK_NOINLINE` (visible body, __declspec(noinline)) is the opt-in for exactly this; the decl-only FABLE_RETAIL_STLP_VECTOR_PUSHBACK_OUTOFLINE would make i escape. If modelling locally: push_back body as STLport (`if (_M_finish) *_M_finish = x; ++_M_finish; else _M_insert_overflow(_M_finish, x, __true_type(), 1, true)`) with __declspec(noinline), and _M_insert_overflow either a noinline same-TU stand-in (as 0x7FEDE0) -- check that the stand-in's visibility doesn't let x escape.
- vector::erase inline (memmove); CCharString::GetAsCharArray inline; CIntelligentPointer ctor/operator= inline (calls to A01B10/A01B90/A01B50/A01C10).
- 0x7FEC30 IsActionPointIndexBlocked(long): same TU, defined earlier (address before), takes long by value -- doesn't affect i.

## 6. Siblings (same class / same TU, all in rebuild/src/compiled/00/7f/)
- CTCNamedUserActionPoints_GetFullNamedActionPoint_007fefe0.cpp -- THE template: identical prologue, identical loop head/strstr/find shape, retail also keeps i in a register + frame copy in the `thing` slot (verified: 0x007FF029 `mov ebx,eax; mov [esp+18h],ebx`). Flags: none; `#define FABLE_RETAIL_CCHARSTRING` + fable_string.h; `PThing = CIntelligentPointer<CThing>(&thing); for (long i = start; i < NumberOfActionPoints; ++i) { const CCharString& element = Names[i]; if (strstr(element.GetAsCharArray(), name.GetAsCharArray())) { long* last = FreeActionPointIndexes._M_finish; if (Find(FreeActionPointIndexes._M_start, last, i, CIteratorTag()) == last) ...`
- CTCUserActionPointBase_GetFreeActionPoint_007fede0.cpp -- erase body (`if (_M_finish != next) memmove; --_M_finish`), owner as named `CThing* owner = PThing.GetPItem()` before the virtual call, push_back + same-TU _M_insert_overflow stand-in, `#pragma auto_inline(off)` around stand-ins.
- CTCUserActionPointBase_IsActionPointIndexBlocked_007fec30.cpp -- std::find/__find same-TU body under auto_inline(off), random_access_iterator_tag hierarchy.
- CTCUserActionPointBase_UpdateBlockedActionPoints_007fecb0.cpp, CTCNamedUserActionPoints_IsActionPointIndexBlocked_007ff0c0.cpp (local class model with 28 virtual slots + slot 28).
- No `#pragma optimize` in any sibling (TU is plain /O2, no frame pointer).

## 7. Old draft model errors
1. `CThing* user` instead of `CThing& thing` (PDB). Use `&thing` for the PThing temp.
2. Callee 0x7FEC30 misnamed `IsActionPointIndexInBlockedList`; real `CTCUserActionPointBase::IsActionPointIndexBlocked(long)` (protected).
3. Non-template `find` with `const random_access_iterator_tag&` declared both extern and defined -- use the siblings' template noinline Find.
4. Find's last pointer not a named local: retail compares find's result with the cached edi and erase reuses it (`mov ecx,edi`); write `long* last = FreeActionPointIndexes._M_finish;` and erase against the same value (erase that tests `_M_finish != next` after a visible find should CSE to edi).
5. Missing the name/element evaluation order idiom (`const CCharString& element = Names[i]` then strstr(element.GetAsCharArray(), name.GetAsCharArray())), local CCharString with a hand-written c_str instead of fable_string.h.
6. CBaseIntelligentPointer wrong (non-virtual dtor, extra v00, GetPItem returning CThing*); CIntelligentPointerThing non-template.
7. push_back body lacked STLport placement-new null test; its visible insert_overflow was decl-only (lets &i escape -> plausible cause of i living only in memory and checkBlocked going to bl). Prefer push_back noinline-visible with _M_insert_overflow also visible-noinline, or test FABLE_RETAIL_STLP_VECTOR_PUSHBACK_NOINLINE.
8. Draft logic (continue conditions, `!blocked || !checkBlocked` -> return true, else push_back) is correct vs disasm.

## 0x00BEAB40

# DOSSIER 0x00BEAB40 CWaterPatchMesh::GetWaterType (345 bytes)

## 1. Retail shape (xdis.py 00beab40 0x169)
- __thiscall, plain `ret` (0xBEAC8F, early-out ret 0xBEAC98). No `push ebp; mov ebp,esp` frame (FPO, no /Oy- pragma), no SEH, `sub esp,14h`.
- Saves ebx/esi/edi up front; `push ebp` is shrink-wrapped AFTER the outer-loop guard (0xBEAB67), early-out path at 0xBEAC90 returns ecx (=0) without touching ebp.
- Regs: edi=this, ebx=x (=Offset.X, outer), esi=y (=Offset.Y, inner), ebp=theme0 then index0 (same reg, `mov ebp,eax` at 0xBEABC9).
  Spills: [esp+1c]=startX+16 (outer end, computed once), [esp+18]=startY (inner end recomputed `startY+16` each iter at 0xBEAC63..6B;
  y reset from it at 0xBEAC77), [esp+0c]=waterType (init 0 via xor ecx), theme1 and index1 SHARE slot [esp+14 after push], theme2 and
  index2 SHARE slot [esp+18 after push] => source likely reassigns one variable per layer (`t1 = Mgr->GetDefClassIndexFromGlobalIndex(t1)`)
  or compiler slot-sharing; worth trying since NOTES says only "named theme locals" variants were tried.
- Both loops rotated with duplicated guards (`cmp ebx,startX+16; jg` and the dead `lea ecx,[esi+10h]; cmp esi,ecx; jg`), `<=` bounds, align padding `lea ebx,[ebx]`/`lea ecx,[ecx]`.
- Map->Map reloaded before every PeekThemeId call (`mov eax,[edi+4]; mov ecx,[eax+14h]`) => member access through this each time; Offset.X/Y read once into regs (locals).
- Theme base `mov ecx,[g_0x01436EA8+0x4B8]` loaded ONCE after the unset test and kept in ecx; index*0x38 via `imul edx,edx,38h`, WaterType at +0x30.
- Logic (matches draft): if (wt==0||wt==7) { if (T[i0].WaterType) wt=it; if (i1 && T[i1].WaterType) { if (wt==0||wt==7) wt=T[i1].WaterType;
  if (i2 && T[i2].WaterType) { if (wt==0||wt==7) wt=T[i2].WaterType; } } }. i0 is NOT tested for zero; i1/i2 are. WaterType re-read for the store (`mov edx,[edx+ecx+30h]` again at 0xBEAC35/0xBEAC5B) except layer 0 (uses ebp).
- No callers in retail (xref.py 00beab40 -> 0 refs): dead private function emitted out of line.

## 2. Name / signature (two sources)
- Ego_r.pdb egor_pdb_names.tsv 00aa2bf0 and FableWin _fablewin_api.tsv / FableWin.exe.signatures.tsv 017a59bd,02e67c80:
  `?GetWaterType@CWaterPatchMesh@@ABE?BW4EWaterType@@XZ` = private, const, returns **const EWaterType** (`?B`), no args.
- Manifest label CWaterPatchMesh::GetWaterType is CORRECT. Draft's leaf `?AW4` is WRONG (missing const on return).
- Shared header engine/CWaterPatchMesh.h (FABLE_RETAIL_CWATERPATCHMESH) already declares `const EWaterType GetWaterType() const;` (private).
- Not a vtable slot (vtable 0x012A8134 slots: ??_G 0xBEB4D0, RelocateData 0xBEAB20, CanRelocateData 0xBEAA50, Verify 0x9D5100).

## 3. Callees
- 0x00B43720 `?PeekThemeId@CEngineMap@@QBEJJJJ@Z` long(long x,long y,long layer) const, ret 0Ch (Ego_r 009e8890 + FableWin). NOT landed: pending
  DIFFER(320v324) in rebuild/candidates/manual/pending/2026-11-10_linux_hand1013b/differ/00b43720.cpp. Declared in engine/CEngineMap.h under
  FABLE_RETAIL_CENGINEMAP_PEEK. (manifest functions.tsv types return as float -- wrong; draft comment's old "PeekLandscapeHeight" label is stale.) Args pushed (layer, y, x) with x=outer.
- 0x009ACCA0 `?GetDefClassIndexFromGlobalIndex@CDefinitionManager@@QBEJJ@Z` long(long) const. Landed rebuild/src/compiled/00/9a/CDefinitionManager_GetDefClassIndexFromGlobalIndex_009acca0.cpp
  (as a __fastcall free function stand-in). Shared decl: engine/CDefinitionManager.h FABLE_RETAIL_CDEFINITIONMANAGER_GETDEFCLASSINDEX (used by landed CEnvironment_SetThemeWeight_006b3600.cpp). Draft spelling matches PDB.
- Globals: 0x01436E28 = `g_pEngineDefinitionManager` (CDefinitionManager*, descriptive, CEngine_Initialise_00b26360.cpp:108). Draft's `g_pDefinitionManager` DISAGREES -> rename.
  0x01436EA8 = CEngineLandscapeRenderer*: landed spellings disagree: `g_pLandscapeRenderer` (8 files, e.g. CLandscapeBackgroundPatch_* rows) vs
  `g_LandscapeRenderer_01436EA8` (CEngine_Initialise). Draft uses g_pLandscapeRenderer (majority). No PDB data symbol.

## 4. Members
- this: engine/CWaterPatchMesh.h. Retail opt-in FABLE_RETAIL_CWATERPATCHMESH: `const CEngineLandscapeMap* Map` +4, `C2DCoordI Offset` +8 (X +8, Y +0xC). Default (non-retail) branch flattens Offset_X/Offset_Y (draft used that + #define rename hack + local subclass; drop it, use the retail opt-in like pending Load 0x00BEAE50).
- CEngineLandscapeMap::Map (CEngineMap*) +0x14 (struct_layouts_egor.tsv). NO shared header (only engine/_quarantine/CEngineLandscapeMap.h; candidate headers landscape_map_*_compiled.h) -> local model OK. FableWin inline accessor `?PeekEngineMap@CEngineLandscapeMap@@QBEPBVCEngineMap@@XZ` (018349d3) returns const CEngineMap* -- PeekThemeId is const, so `Map->PeekEngineMap()->PeekThemeId(x,y,n)` fits.
- CEngineLandscapeRenderer::LandscapeThemes CArray<CLandscapeTheme> at retail +0x4B8 (PDB +0xD14) -- engine/CEngineLandscapeRenderer.h FABLE_RETAIL_CENGINELANDSCAPERENDERER (line 119); struct CLandscapeTheme (0x38, WaterType EWaterType +0x30) in same header line 41. CArray operator[] via begin(): FABLE_RETAIL_CARRAY (engine/CArray.h:34).
- FableWin inline `?PeekTheme@CEngineLandscapeRenderer@@QBEABUCLandscapeTheme@@K@Z` (017abb74/02cb0630) = const CLandscapeTheme& PeekTheme(unsigned long) const -- not offered by any header yet; a local inline/opt-in `return LandscapeThemes[i];` is the likely original spelling.
- Locally modelled in draft but with shared header: CWaterPatchMesh (use retail opt-in), CEngineMap (use FABLE_RETAIL_CENGINEMAP_PEEK), CDefinitionManager (FABLE_RETAIL_CDEFINITIONMANAGER_GETDEFCLASSINDEX), CLandscapeTheme + CEngineLandscapeRenderer (FABLE_RETAIL_CENGINELANDSCAPERENDERER), EWaterType (headers forward-declare `enum EWaterType;` -- enumerator names unknown).

## 5. Callee visibility
- PeekThemeId / GetDefClassIndexFromGlobalIndex are real out-of-line calls in other TUs (CEngineMap.cpp 0xB43720, DefinitionManager 0x9ACCA0): declaration only.
- Inline accessors expanded: PeekEngineMap (Map+0x14 load), PeekTheme (+0x4B8 begin, imul 0x38). Same-TU neighbours (WaterPatchMesh.cpp): CanRelocateData 0xBEAA50, RelocateData 0xBEAB20 (before), Render 0xBEACA0, Load 0xBEAE50, ctor 0xBEB490 (after). None called here.

## 6. Siblings
- rebuild/src/compiled/00/be/CWaterPatchMesh_Render_00beaca0.cpp: no pragmas, no Flags, includes fable_compiler.h + engine/CWaterPatchMesh.h.
- CWaterPatchMesh_RelocateData_00beab20.cpp / CWaterPatchMesh_CWaterPatchMesh_00beb490.cpp: no pragmas; local vptr models.
- Pending same TU: rebuild/candidates/manual/pending/2026-11-05_linux_hand779/differ/00beae50.cpp (Load) uses FABLE_RETAIL_CWATERPATCHMESH, CMOVABLERESOURCE, CARRAY, CENGINELANDSCAPERENDERER (+memory manager, vertex buffer) -- copy this include block.
- rebuild/src/compiled/00/b6/CEngineLandscapeRenderer_BuildBlendingTables_00b67270.cpp: FABLE_RETAIL_CENGINELANDSCAPERENDERER(+_BLENDTABLES), no pragmas.
- rebuild/src/compiled/00/6b/CEnvironment_SetThemeWeight_006b3600.cpp: FABLE_RETAIL_CDEFINITIONMANAGER_GETDEFCLASSINDEX.
- Default flags suffice (no pragma in any sibling); retail shows FPO, no SEH.

## 7. Draft model errors
- Wrong leaf (`?AW4` -> `?BW4`, return `const EWaterType`).
- Global name g_pDefinitionManager -> g_pEngineDefinitionManager (0x01436E28).
- Local models instead of shared retail opt-ins (see 4); #define-rename subclass hack.
- Missing inline accessors PeekEngineMap / PeekTheme (affects expression trees -> register allocation).
- theme/index as six variables; retail slot-sharing suggests three variables reassigned (theme0/index0 in ebp, 1 and 2 spilled).
- Control flow, loop form (<= 16 bounds, inner end recomputed from startY, outer end hoisted) and logic are right; residue (NOTES.md:97, STATUS 325v345) is register allocation only: retail x=ebx, y=esi, this=edi, theme0=ebp; lane spills x and this.

## 0x00BFA920

# DOSSIER 0x00BFA920 -- CLocalDetailPrimitiveMesh::AddObjectsToPrimitiveRenderer (346 bytes)

## 1. Retail shape (xdis 00bfa920 0x16a)
- thiscall, `ret` (no args), `sub esp,14h` then push ebp/esi(/edi/ebx later). NO frame pointer (/Oy on), NO SEH prologue
  (handle temp destroyed inline without an EH state -> no unwind frame; do not add /EHsc-requiring objects w/ state).
- Register plan: esi=this (later this+0x6C), ebp=0 (zero register for every null compare and both `=0` stores), edi=flags
  then the returned handle ptr, ebx=0xFF (pushed alpha `long 255` AND the four colour bytes).
- Early-out: `if (Type->Mesh == 0) return;` (Type at this+0x30, Mesh at +4).
- Fade: FadeStart (+0x20) copied by `mov`, FadeEnd (+0x24) by `fld/fstp` into a local CEngineFadeDistance at frame slot 0xC.
- flags: edi=1; `if (AlphaIsBoolean byte +0x4C) edi=0x101`; then `movzx [Type+0x50]; fild; fld 0.0f(0x0122DEDC); fucompp;
  test ah,44h; jp skip; or edi,1000h` == `if ((float)ReceivesShadows == 0.0f) flags |= 0x1000` (== not !=; jp skips when unequal).
- Frame (0x14 bytes): slot0 = fild temp, later reused for CRGBColour; slot4..0xB = returned CEnginePrimitiveHandle temp
  (slot4 also first used as a spill of AlphaMipBias: `mov edx,[Type+2C]; mov [slot4],edx; ... mov edi,[slot4]; push edi`);
  slot0xC..0x13 = fade.
- Push order (right-to-left): flags(edi), 1 (persistent), AlphaMipBias (+0x2C, via spill), AlphaRef (+0x28), &colour, 0xFF
  (ebx), &fade, Scale [this+0x68], &Matrix (this+0x38), ShadowMesh (+0x0C), Mesh (+0x04), &result (hidden ptr);
  ecx = [0x01436E80]; call 0x00B32F20. Colour bytes stored between pushes in order +2,+1,+0,+3 (R,G,B,A over B,G,R,A layout)
  = CRGBColour(255,255,255,255) ctor body `R=r;G=g;B=b;A=a`.
- After the call: PrimitiveHandle = result, inline:
  Handle::Clear(): `if (Data && Data->IVRefCount==1) Data->NotifyReleased(*this)` [vslot3 +0xC];
  `if (Data){ if(--IVRefCount==0) Delete() [vslot1 +4]; Data=0; } Data=0;` (TWO stores of 0 at bfa9fd/bfaa00);
  then CIVCountedPointer::operator=: `if (Data(=0,ecx=ebp) != r.Data) { if(Data){dec;Delete;Data=0} Data=r.Data; if(Data) ++ref; }`
  -- the compiler did NOT fold the known-0 Data, so the full Release is emitted in the assign.
- Temp dtor: vptr store 0x0122F598 (CEnginePrimitiveHandle vtable), then `if(Data){ if(refcount==1) NotifyReleased(&temp);
  reload Data; if(Data){ load/dec/store refcount (esi reg, `mov [eax],esi`), if 0 Delete } } Data=0;` then
  call ~CBaseClass 0x0099A2E0 on &temp. Note the temp's decrement is load/dec/store/compare vs `dec [ecx+4]` on the member.

## 2. Identity (two sources) -- manifest label is CORRECT
- vtable: rebuild/manifest/rtti-vtables.tsv `CLocalDetailPrimitiveMesh 012a7e04 slot5 00bfa920` (xref: abs ref at .rdata 012a7e18).
- PDB: Ego_r `?AddObjectsToPrimitiveRenderer@CLocalDetailPrimitiveMesh@@UAEXXZ` (ghidra_out/egor_signatures.tsv, debug addr 00aacc60)
  and FableWin.pdb same symbol (ghidra_out/fablewin_pdb_names.tsv 02edced0).
- Signature: `virtual void CLocalDetailPrimitiveMesh::AddObjectsToPrimitiveRenderer()` (public virtual, non-const, void).
- Mangled leaf: `?AddObjectsToPrimitiveRenderer@CLocalDetailPrimitiveMesh@@UAEXXZ`.
- Old draft's class is `LocalDetailMeshView` -> wrong spelling; must be `CLocalDetailPrimitiveMesh` deriving from
  `CLocalDetailPrimitiveBase` (as the siblings model it).

## 3. Callees
- 0x00B32F20 CEnginePrimitiveRenderer::AddStaticMesh -- landed rebuild/src/compiled/00/b3/CEnginePrimitiveRenderer_AddStaticMesh_00b32f20.cpp
  (landed with long-typed stand-in params). PDB (Ego_r + FableWin):
  `?AddStaticMesh@CEnginePrimitiveRenderer@@QAE?AVCEnginePrimitiveHandle@@PBVC3DMeshInfo@@0ABVCMatrix3x4@@MABVCEngineFadeDistance@@JABVCRGBColour@@JM_NK@Z`
  = `CEnginePrimitiveHandle AddStaticMesh(const C3DMeshInfo*, const C3DMeshInfo*, const CMatrix3x4&, float, const CEngineFadeDistance&,
  long, const CRGBColour&, long, float, bool, unsigned long)`. Draft's declaration matches. Shared header
  rebuild/include/engine/CEnginePrimitiveRenderer.h does NOT declare AddStaticMesh (only CEnginePrimitiveManagerStaticMeshesBase.h:23 does,
  for the manager) -> declare it in a local derived view like the siblings' `CLocalDetailRenderer : CEnginePrimitiveRenderer`.
- Global 0x01436E80: landed spelling `g_pEnginePrimitiveRenderer` (38 landed rows; "Codex name fix 2026-11-10"). DRAFT SPELLS IT
  `g_PrimitiveRenderer` -> WRONG, respell.
- vslot3 (+0xC) CEnginePersistentPrimitiveBase::NotifyReleased(const CEnginePrimitiveHandle&) 0x00B8FFC0; vslot1 (+4) CIVCountedPointeeBase::Delete() const 0x0042D930.
- 0x0099A2E0 CBaseClass::~CBaseClass (rebuild/include/engine/CBaseClass.h).
- Float constant 0.0f @ 0x0122DEDC.

## 4. Members
- this (CLocalDetailPrimitiveMesh, size 116, struct_layouts_egor.tsv): base CLocalDetailPrimitiveBase (56): Next +4,
  C3DBoundingBox BoundingBox +8, C3DBoundingSphere BoundingSphere +0x20, `CLocalDetailObjectCollectionType* Type` +0x30,
  `CLocalDetailCacheMap* Map` +0x34; own: `CMatrix3x4 Matrix` +0x38, `float Scale` +0x68, `CEnginePrimitiveHandle PrimitiveHandle` +0x6C.
  No engine header for CLocalDetailPrimitiveMesh / CLocalDetailPrimitiveBase / CLocalDetailObjectCollectionType exists
  (only C3DMeshInfo.h, CLocalDetailCacheMap.h); siblings model them locally.
- CLocalDetailObjectCollectionType (84 bytes): Mesh +4, ZSpriteMesh +8, ShadowMesh +0xC, ... PlainDataBlock +0x20 = {FadeStart,
  FadeEnd (float) +0x20/+0x24, long AlphaRef +0x28, float AlphaMipBias +0x2C, ..., bool AlphaIsBoolean +0x4C, LandscapeNormalLighting
  +0x4D, HasWindSkew +0x4E, CastsShadows +0x4F, ReceivesShadows +0x50} (field names as in landed 00bf9bc0 SetupPrimitiveDesc).
- FableWin.pdb inline accessors on the type (all QBE): PeekMesh, PeekShadowMesh (const C3DMeshInfo*), PeekFadeStart/PeekFadeEnd (float),
  PeekAlphaRef (long), PeekAlphaMipBias (float), PeekAlphaIsBoolean (bool), **PeekReceivesShadows returns FLOAT** (`QBEMXZ`) --
  that is the source of the movzx/fild/fucompp: original is `Type->PeekReceivesShadows() == 0.0f`. Draft's `(float)ReceivesShadows`
  is equivalent text but the accessor form (float-returning inline) may change scheduling; PeekFadeStart/End/AlphaMipBias
  returning float may explain FadeEnd via fld/fstp and the spilled mip bias.
- Shared headers with opt-ins (all in rebuild/include/engine/):
  - CEnginePrimitiveHandle.h: FABLE_RETAIL_CENGINEPRIMITIVEHANDLE (Clear = notify-if-1 then Data.Reset(0)),
    FABLE_RETAIL_CENGINEPRIMITIVEHANDLE_DTOR / _CLEAR_DTOR (dtor = Clear()), FABLE_RETAIL_CIVCOUNTEDPOINTER_ASSIGN (operator=,
    with FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_ATTACHED it calls Clear() first -- exactly retail's Clear-then-assign), _DATA.
  - CIVCountedPointer.h: FABLE_RETAIL_CIVCOUNTEDPOINTER (copy ctor, Reset, Release), _ASSIGN, _RESET_ADOPT, _RESET_FORCEINLINE,
    _RELEASE_MEMBER / _RELEASE_INNER, _DTOR_DIRECT, _CONVERT, _FROM_RAW.
  - CIVCountedPointeeBase.h: FABLE_RETAIL_DEF_CLASS_BASE, _CTOR, _RELEASE_DELETES. CEnginePersistentPrimitiveBase.h: FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE.
  - CRGBColour.h: B,G,R,A union; base ctor `CRGBColour(r,g,b,a){R=r;G=g;B=b;A=a;}` (matches retail store order); FABLE_RETAIL_CRGBCOLOUR
    (Initialise-based copy ctor / 3-arg ctor), _DWORD_COPY.
  - CEngineFadeDistance.h: `FadeBeginDistance, MaxDrawDistance` (PDB names; draft says FadeStart/FadeEnd), ctors only DECLARED
    (non-inline) -> retail inlines the 2-float ctor, so a local inline ctor (as 00bf9bc0 does) or a header opt-in is needed.
  - CMatrix3x4.h (FABLE_RETAIL_CMATRIX...), CBaseClass.h, C3DMeshInfo.h.
- Classes the old draft models LOCALLY that HAVE shared headers: CMatrix3x4, CEngineFadeDistance, CRGBColour, CBaseClass,
  CIVCountedPointeeBase, CEnginePersistentPrimitiveBase, CEnginePrimitiveHandle (+CIVPrimitivePointer instead of
  CIVCountedPointer<CEnginePersistentPrimitiveBase>), CEnginePrimitiveRenderer, C3DMeshInfo (fwd only). Note the landed siblings
  themselves use the "#define X XDonor / include / #undef" trick and model the handle locally.

## 5. Callee visibility
- AddStaticMesh 0x00B32F20 is a separate engine TU (b3 range); not visible. STATUS/NOTES "next" suggested a same-TU stand-in --
  unsupported by addresses (local-detail TU lives 0x00BF9900..0x00BFBD80).
- All handle/pointer ops are inline (no out-of-line calls besides ~CBaseClass). ~CBaseClass 0x0099A2E0 called out of line.
- Same TU (local-detail primitives, defined earlier at lower addresses): 00bf9900 operator new, 00bf9bc0 RepeatedMesh::SetupPrimitiveDesc,
  00bf9fa0 ZSpriteBatch::SetupPrimitiveDesc, 00bfa0f0 Mesh::Load, 00bfa8a0 ZSpriteBatch::RelocateData; later: 00bfaa80/bfaae0 ctor,
  00bfaba0 RepeatedMesh::AddObjectsToPrimitiveRenderer, 00bfacc0 ZSpriteBatch::AddObjectsToPrimitiveRenderer. None called here.

## 6. Sibling landed rows (rebuild/src/compiled/00/bf/)
- CLocalDetailPrimitiveRepeatedMesh_AddObjectsToPrimitiveRenderer_00bfaba0.cpp and CLocalDetailPrimitiveMeshZSpriteBatch_AddObjectsToPrimitiveRenderer_00bfacc0.cpp:
  no pragmas/Flags; local CIVCountedPointeeBase/CEnginePersistentPrimitiveBase/CIVPrimitivePointer/CEnginePrimitiveHandle with
  FABLE_FORCEINLINE Clear/operator=/dtor (same model the draft copied), `CLocalDetailRenderer : CEnginePrimitiveRenderer` local
  view, `extern CEnginePrimitiveRenderer* g_pEnginePrimitiveRenderer; // 0x01436E80`, CLocalDetailPrimitiveBase as
  `__declspec(novtable)` struct with the 7 virtuals. One-liner `PrimitiveHandle=((CLocalDetailRenderer*)g_pEnginePrimitiveRenderer)->Add...(...)`.
- CLocalDetailPrimitiveRepeatedMesh_SetupPrimitiveDesc_00bf9bc0.cpp: local `CEngineFadeDistance(float s,float e)` inline ctor,
  `CEngineFadeDistance fade(Type->FadeStart,Type->FadeEnd);`, flags `0x100 / 0x8100 if LandscapeNormalLighting` -- same idiom.
- CLocalDetailPrimitiveMesh_Load_00bfa0f0.cpp: models CLocalDetailPrimitiveMesh locally (+0x38 Matrix, +0x68 Scale), `#pragma intrinsic(memcpy)`.
- Retail-macro handle assignment idiom: NParticleEngineCPSCRenderSprite_PreparePrimitives_00afcc30.cpp (Flags: /d2inlT1537 /d2inlS15)
  defines FABLE_RETAIL_CENGINEPRIMITIVEHANDLE, _CIVCOUNTEDPOINTER, _CIVCOUNTEDPOINTER_ASSIGN, _CENGINEINTERNALPRIMITIVEBASE_ATTACHED,
  _CENGINEPERSISTENTPRIMITIVEBASE, _DEF_CLASS_BASE, _CRGBCOLOUR; also CEngineInternalAttachedPrimitive_Update_00b396e0.cpp.

## 7. Old draft model errors / residue
- Residue (STATUS.tsv: DIFFER 334v346): ours ebx=0, flags in edx, 0xFF pushed as immediate; retail ebp=0, ebx=0xFF, edi=flags,
  mip bias spilled through the frame. 12 bytes short.
- Names: class `LocalDetailMeshView` -> CLocalDetailPrimitiveMesh; `g_PrimitiveRenderer` -> g_pEnginePrimitiveRenderer; local
  `CollectionType` -> CLocalDetailObjectCollectionType; fade members FadeStart/FadeEnd -> FadeBeginDistance/MaxDrawDistance (PDB, header).
- Control flow matches (early return on null Mesh; == compare; Clear-then-assign).
- Probable missing ingredient: the PDB inline accessors (Peek* returning float/long/bool/pointer). Retail's fild of the bool, FadeEnd
  loaded through x87 and AlphaMipBias spilled are consistent with float-returning inline Peek calls (PeekFadeStart/End,
  PeekAlphaMipBias, PeekReceivesShadows); try `CEngineFadeDistance fade(Type->PeekFadeStart(), Type->PeekFadeEnd())`,
  `Type->PeekAlphaRef(), Type->PeekAlphaMipBias()`, `if (Type->PeekReceivesShadows() == 0.0f)`, `if (!Type->PeekMesh()) return;`.
- 0xFF shared in ebx between `long alpha` arg and colour bytes: suggests the alpha argument and colour came from a common
  source (e.g. a local `unsigned char`/const 255 or CRGBColour(255,255,255) 3-arg ctor with A defaulting to 255) -- try those.
- Draft's handle Clear stores Data.Object=0 explicitly after Data.Clear(): matches retail's double store; keep.

## 0x009E3DF0

# DOSSIER 0x009E3DF0 CPixelFormat::ConvertPFDDataToRGB (354 bytes, code ends 0x9E3F52 = exactly 0x162; old draft's "352" is wrong)

## 1. Retail shape (xdis 009e3df0 0x172)
- __thiscall, `ret 8` (two stack args), NO frame pointer (`push ecx; push ebx; push ebp; mov ebp,ecx`), no SEH. Pushes: ecx,ebx,ebp,esi,edi.
- `push ecx` slot [esp+10h] = local `pixel` (zeroed `mov [esp+10h],0`, then memcpy intrinsic: count = BitDepth/8 SIGNED
  (`cdq; and edx,7; add; sar 3`), `rep movsd` + `rep movsb`, i.e. memcpy(&pixel, data, GetByteColourDepth())).
- Table: GD3DFormats @0x0129BA40, stride 36 (`lea x,[x+x*8]; shl 2`): BitDepth +8 (0x129BA48), AlphaBits +0xC (..4C),
  RBits +0x10 (..50), GBits +0x14 (..54), BBits +0x18 (..58).
- Order: R -> [esi+2], G -> [esi+1], B -> [esi], then `if (AlphaBits)` A -> [esi+3] else `mov byte [esi+3],0FFh` (two epilogues).
  FormatIndex reloaded from [ebp] after each byte store (byte store may alias).
- R arm: mask = ((1<<R)-1) << (B+G) where B was loaded BEFORE the rep movs (edx=[..58]) and G after (eax=[..54]), `add edx,eax`
  (B+G order = FableWin GetRPosShift = GetGPosShift()+GetNoGBits(), GPos = BPos(0)+B). Then `and` pixel, `shr` by a RE-LOADED
  [..58]+[..54] (ecx=B; add ecx,edi(G)), then `shl al,(8-R)` byte op.
- G arm: shl/shr share ecx=B (CSE'd within the arm), `shl bl,(8-G)`.
- B arm: pure BYTE arithmetic: `mov al,1; shl al,cl; dec al; and al,dl; shl al,(8-B)` (BPosShift 0 folded away).
- A arm: AlphaBits tested with fresh row; pos shift = (B+G)+R (`add ebx,ebp(G); add ebx,ecx(R)`), row offset spilled to the
  colour-arg slot [esp+1Ch], masked value spilled to the data-arg slot [esp+18h]; shr amount RE-LOADS B,G,R (ecx=B; add G; add R).
- So the residue (per sibling rows too): positional shift sums used twice (mask + shr) are recomputed from fresh loads in R and A.
  This is the same residue chunk 1014 parked on 0x009E3F60 ConvertRGBToPFDData ("retail re-reads every GD3DFormats bit count").

## 2. Identity (proven by 2+ sources)
- Ego_r.pdb (ghidra_out/egor_pdb_names.tsv 00b35c40, egor_signatures.tsv) and FableWin.pdb
  (ghidra_out/naming_stragglers/FableWin.exe.signatures.tsv 0308df60 + 017b76e5; _fablewin_api.tsv):
  `?ConvertPFDDataToRGB@CPixelFormat@@QBEXPBEAAVCRGBColour@@@Z`
  = `void CPixelFormat::ConvertPFDDataToRGB(const unsigned char* data, CRGBColour& colour) const`. Manifest label correct.
- Order evidence: Ego_r: ConvertPFDToRGB 359e0, ConvertPFDDataToRGBLong 35af0, ConvertPFDDataToRGB 35c40, ConvertRGBToPFDData 35d90;
  FableWin same order (RGBLong 0308dc90 before RGB 0308df60). Retail: ConvertPFDToRGB 0x9E3B60, [0x9E3C80 unlisted],
  0x9E3DF0, ConvertRGBToPFDData 0x9E3F60.
- TWIN: 0x009E3C80 (missing from rebuild/manifest/functions.tsv; boundary-starts row 9e3b60 lists it) is BYTE-IDENTICAL
  (diffed, 354 bytes, same `ret 8`) = ConvertPFDDataToRGBLong(const unsigned char*, unsigned long*) by PDB order. A match here
  matches it too.
- Single caller: CTexture::CalcAlphaHistogram 0x009FBBD0 (call at 0x9FBC7A; reads colour byte +3 = alpha). Landed file
  rebuild/src/compiled/00/9f/CTexture_CalcAlphaHistogram_009fbbd0.cpp declares it WRONG: `ConvertPFDDataToRGB(unsigned char*, CRGBColour*) const`
  (line 65) and a local `class CRGBColour` (line 18) -> NAME_FIXES candidate (PDB: const unsigned char*, CRGBColour&).

## 3. Callees
- None. No call/jmp out. memcpy = intrinsic. All accessors (GetByteColourDepth, GetR/G/B/AMask, Get*PosShift, Get*ScaleShift,
  GetNo*Bits) are inline (FableWin.pdb has them as debug bodies: GetRMask/GetGMask/GetBMask/GetAMask, GetRPosShift..GetBPosShift,
  GetRScaleShift..GetAScaleShift, GetNoRBits..GetNoABits, GetByteColourDepth, all `QBEKXZ` = unsigned long () const).
- Note GetByteColourDepth exists out of line at 0x009E3800 (landed), but here it is inlined (cdq/sar idiom); do NOT enable
  FABLE_RETAIL_CPIXELFORMAT_BYTECOLOURDEPTH_OUTOFLINE.

## 4. Members / headers
- `this`: CPixelFormat::FormatIndex (long, +0) -- rebuild/include/engine/CPixelFormat.h, needs `#define FABLE_RETAIL_CPIXELFORMAT`
  (FormatIndex is `protected` there; method must be a real member).
- Table rows: rebuild/include/engine/CD3DFormatInfo.h: `class CD3DFormatInfo {_D3DFORMAT D3DFormat; EPixelFormatType PFType; long BitDepth,
  AlphaBits,RBits,GBits,BBits,StencilBits; bool Linear;}` + `extern const CD3DFormatInfo GD3DFormats[];` (matches struct_layouts_egor.tsv).
  Global name GD3DFormats: used by the shared header + 0x009E3640 landed row; old draft and 0x009E3790 spell it `g_D3DFormatInfo`
  (wrong/inconsistent -- use GD3DFormats). (No PDB global dump found in ghidra_out to double-prove the global name.)
- colour: rebuild/include/engine/CRGBColour.h (union {B,G,R,A @0..3; IntValue}); struct_layouts: CRGBColour 4 bytes, CRGBColour_u_0.
- Opt-ins already present in CPixelFormat.h:
  * FABLE_RETAIL_CPIXELFORMAT (real class), FABLE_RETAIL_CPIXELFORMAT_CONVERT (chunk 1014): GetR/G/B/AScaleShift (8-bits),
    GetGPosShift = GetBPosShift()+GetNoBBits(), GetBPosShift = 0, GetRPosShift = GetGPosShift()+GetNoGBits(),
    GetAPosShift = GetRPosShift()+GetNoRBits(), inline GetByteColourDepth = BitDepth/8, decl of ConvertRGBToPFDData.
  * GetAMask() = ((1UL<<GetNoABits())-1)<<GetAPosShift() (always present).
  * MISSING: GetRMask/GetGMask/GetBMask (FableWin names exist) and a ConvertPFDDataToRGB declaration -> author must add a new
    opt-in branch (e.g. inside FABLE_RETAIL_CPIXELFORMAT_CONVERT or a new FABLE_RETAIL_CPIXELFORMAT_CONVERT_TORGB) with
    `unsigned long GetRMask() const {return ((1UL<<GetNoRBits())-1)<<GetRPosShift();}` etc. and the method declaration.
- Old draft LOCAL models that have shared headers: `struct CD3DFormatInfo` (engine/CD3DFormatInfo.h), `struct CRGBColour`
  (engine/CRGBColour.h), `CPixelFormat_Methods : CPixelFormat` subclass (use FABLE_RETAIL_CPIXELFORMAT real class), `g_D3DFormatInfo`.

## 5. Visibility
- No out-of-line callee. Same-TU earlier functions (CPixelFormat source file, retail 0x9E3620..0x9E3F60 contiguous): FindFormatInfoIndexFromD3DFormat
  9E3620, Initialise x3, IsEquivalent 9E37F0, GetByteColourDepth 9E3800, GetColourDepth 9E3820, GetInit 9E3870, IsCompressed 9E3940,
  IsSigned 9E3970, (IsEquivalent(Init) 9E3990?), ConvertPFDToRGB 9E3B60, ConvertPFDDataToRGBLong 9E3C80 (twin). None called here.
- No STLport involvement.

## 6. Siblings
- rebuild/candidates/manual/pending/2026-11-10_linux_hand1014/differ/009e3f60.cpp (ConvertRGBToPFDData, DIFFER 179v189, SAME residue:
  ours CSEs the row address/loads; retail re-reads). Uses `#define FABLE_RETAIL_CPIXELFORMAT`, `#define FABLE_RETAIL_CPIXELFORMAT_CONVERT`,
  engine/CRGBColour.h, <string.h> memcpy, accessor-chain spelling. Its NOTES "next": give each channel its own accessor indexing
  `GD3DFormats[FormatIndex].XBits` (no shared row pointer). Solving one likely solves both (and the 0x9E3C80 twin).
- Landed: CPixelFormat_GetByteColourDepth_009e3800.cpp (#include engine/CPixelFormat.h, no pragma), CPixelFormat_IsSigned_009e3970.cpp /
  IsCompressed_009e3940.cpp (built with 4035 cl per header comment; no pragma), CPixelFormat_Initialise_009e3790.cpp (local
  struct + g_D3DFormatInfo -- don't copy), CPixelFormat_Initialise_009e3640.cpp (`// Flags:` empty, GD3DFormats). None use optimize pragmas.
- Default flags appear fine (no frame pointer -> /Oy default; no /EHsc frame).

## 7. Old draft model errors
- Local structs/global name/subclass (see 4). Draft hand-expands the fields through `GetInfo()` returning a const CD3DFormatInfo&
  (a shared row reference -> invites the row-address CSE retail does not do); retail is consistent with each FableWin accessor
  indexing GD3DFormats[FormatIndex] independently.
- Draft pos-shift sums are commuted (`GBits + BBits`, `GBits + BBits + RBits`); retail sums B+G then +R (FableWin chain, header CONVERT).
- Draft bit counts are `long` (signed shifts); header accessors return unsigned long (FableWin QBEKXZ) -> `shr` (logical) matches retail.
- Likely source (FableWin accessors): `unsigned long pixel = 0; memcpy(&pixel, data, GetByteColourDepth());
  colour.R = (unsigned char)(((pixel & GetRMask()) >> GetRPosShift()) << GetRScaleShift()); colour.G = ... GetGMask/GetGPosShift/GetGScaleShift;
  colour.B = ... GetBMask/GetBPosShift/GetBScaleShift; if (GetNoABits()) colour.A = ...GetAMask/GetAPosShift/GetAScaleShift; else colour.A = 0xFF;`
  (could also be via `colour.Initialise`/ternary for A; retail has two separate epilogues, consistent with if/else).
- Tried before (hand248 NOTES): commuted sums, memcpy inside the R expression, cast destination, direct table indexing.
  Untried: the exact FableWin accessor nesting with GetXMask inline members; a `GetFormatInfo()`-free spelling; /Og-local pragma ideas
  (G arm IS CSE'd within itself, so it is not a global "no CSE" -- more likely an inline-expansion artifact of nested accessors).

## 0x007F5150

# DOSSIER 0x007F5150 -- CTCSoundAtmosphereVillage::GetGain (355 bytes, 0x007F5150..0x007F52B2, int3 pad from 0x007F52B3)

## 1. Retail shape (xdis 007f5150 0x173)
- __thiscall, `ret 4` (one stack arg: `const CCharString&`), returns float on x87 (fld).
- NO frame pointer: `push ecx` (one frame word, used to spill `this` at [esp+0xc] -> later [esp+0x10]), push ebx/esi/edi; ebp pushed LATE (0x7F51C3, only on the recount path) and used as a GP register (the village). So /Oy (default), no `optimize("y",off)`.
- No SEH prologue. `lea ecx,[ecx]` loop-alignment pad at 0x7F520D => speed-optimised (/O2), NOT optimize("s").
- Flow:
  1. name test: `[arg]==0` -> inline strcmp intrinsic `repe cmpsb` esi="STREETLIFE"(0x01271DE8) edi=""(0x0122D70E) ecx=0xB; else `call 0x004115A0` (CBasicString<char>::operator==(const char*), push "STREETLIFE"). Not equal -> `fld -1.0f`(0x0122DEE0) via shared epilogue 0x7F517F.
  2. `NumLivingSpaces(+0x14)==0` -> return 0.0f (0x0122DEDC).
  3. `fld TimeUntilNextUpdate(+0xC); fcomp 0.0; test ah,41h; jp` => `if (TimeUntilNextUpdate <= 0.0f)` recount.
  4. recount: ParentThing(+4) -> `test byte [eax+24h],4` (HasTC TCI_VILLAGE=34) ; if present: key 0x22 stored into the DEAD ARG SLOT [esp+0x18] *after* the test, `lea esi,[eax+44h]` (map ptr local), `call 0x0040F020` (non-const LowerBound), `mov esi,[esi+4]` end via same reg, `cmp eax,esi; je; cmp [eax],22h; jle; mov eax,esi`, `mov ebp,[eax+4]`. Absent: `mov ebp,[esp+18h]` (uninitialised village read from the same arg slot).
  5. `NumVillageMembers(+0x18)=0`; list = village+0x2C (sentinel node ptr); `edi=head->next; cmp edi,head; je` guard, then do-while, end re-read each iteration `cmp edi,[ebp+2Ch]` (STLport list begin()/end() loop).
  6. `esi=[esp+18h]` before loop = uninitialised `modes` shared with arg slot; `ebx=0x31` key constant held in ebx in loop (this spilled to frame word).
     per node: `lea ecx,[edi+8]; call 0x00A01B50` GetPItem; `test dword [eax+24h],20000h` (HasTC TCI_ENTITY_MODE_MANAGER=49 -- note DWORD test here, BYTE test for 34); if present same GetTC expansion with key in [esp+18h] (`mov [esp+1Ch],ebx` after push) -> esi. Then `mov ecx,esi; call 0x0073A6E0` GetCurrentMode; `mov edx,[eax]; call [edx+4]` (CModeBase slot 1 GetType); `cmp eax,0Dh` (NCreatureMode::MODE_SLEEP_BED=13, engine/NCreatureMode.h:23); != -> `mov eax,[esp+10h]; inc dword [eax+18h]` (++NumVillageMembers via reloaded this).
  7. after loop `TimeUntilNextUpdate = 5.0f` (0x40A00000 immediate store).
  8. `fild NumVillageMembers; fidiv NumLivingSpaces` => `float gain = (float)NumVillageMembers / NumLivingSpaces` (int divisor via fidiv);
     `fcom 0.0; test ah,5; jp` => `if (gain < 0.0f) return 0.0f;`, `fcom 1.0(0x0122DED8); test ah,41h; jne -> 0x7F517F` => `if (gain > 1.0f) return 1.0f; return gain;` (return-gain path shares the -1.0 path's pop/ret tail).

## 2. Identity (name proven, 2+ sources)
- Vtable: rebuild/manifest/rtti-vtables.tsv `CTCSoundAtmosphereVillage 0124111c slot 28 = 007f5150`; xref.py: only reference is .rdata 0x0124118C = 0x0124111C+28*4.
- Ego_r.pdb (ghidra_out/egor_pdb_names.tsv 007eeae0) and FableWin.pdb (_fablewin_api.tsv 017ae158 / 0278fe00; naming_stragglers FableWin.exe.signatures.tsv 017ae158): `?GetGain@CTCSoundAtmosphereVillage@@UAEMABVCCharString@@@Z`.
- => `public: virtual float __thiscall CTCSoundAtmosphereVillage::GetGain(const CCharString&)` -- NOT const. Manifest label is right. Mangled leaf `?GetGain@CTCSoundAtmosphereVillage@@UAEMABVCCharString@@@Z`.

## 3. Callees
| addr | landed row | landed / PDB name | old draft spelling |
|---|---|---|---|
| 0x004115A0 | rebuild/src/compiled/00/41/CBasicStringchar_CBasicStringCharoperatorequals_004115a0.cpp | `??8?$CBasicString@D@@QBE_NPBD@Z` CBasicString<char>::operator==(const char*) const (fable_string.h:80 notes retail out-of-line copy 0x004115A0; FableWin 0x01899D90) | local `CBasicStringChar::operator==` -- WRONG class spelling; use fable_string.h |
| 0x0040F020 | rebuild/src/compiled/00/40/CVectorMap_LowerBound_0040f020.cpp | `?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@...@@@@QAEPAU?$pair@...@std@@ABW4ETCInterfaceType@@@Z` (non-const; const is 0x004365B0) | `CInterfaceMap::LowerBound(const long*)` -- WRONG; sibling 0x007F50B0 already respelled to engine/CVectorMap.h |
| 0x00A01B50 | rebuild/src/compiled/00/a0/CBaseIntelligentPointer_GetPItem_00a01b50.cpp | `?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ` (returns CBaseObject*) | returns CThing* in a local struct -- wrong return type; use engine/CIntelligentPointer.h (CIntelligentPointer<T>::GetPItem/Get cast) |
| 0x0073A6E0 | rebuild/src/compiled/00/73/CTCCreatureModeManager_GetCurrentMode_0073a6e0.cpp | `?GetCurrentMode@CTCCreatureModeManager@@QBEAAVCModeBase@1@XZ` -- const, returns `CModeBase&` | `CMode* GetCurrentMode()` non-const pointer -- WRONG |
| vslot 1 `[edx+4]` | -- | CTCCreatureModeManager::CModeBase::GetType() const, `unsigned long` (engine/CTCCreatureModeManager.h:16) | `CMode::GetModeType` -- WRONG name/class |

## 4. Members
- this: CTCSoundAtmosphereVillage (Ego_r 0x1C): +0x04 CTCBase::ParentThing (CThing*), +0x0C TimeUntilNextUpdate float, +0x10 PDef CDefPointer<const CSoundAtmospheresDef> (CTCSoundAtmosphere, 0x14), +0x14 NumLivingSpaces long, +0x18 NumVillageMembers long (struct_layouts_egor.tsv). NO shared header for CTCSoundAtmosphere / CTCSoundAtmosphereVillage (local model is allowed; base should derive engine/CTCBase.h, `#define FABLE_RETAIL_CTCBASE`; CTCBase.h:143 GetParentThing() inline, ParentThing +4).
- CThing +0x20 UsingComponents std::bitset<274> (word +0x24 holds bits 32..63), +0x44 Components CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..>> (engine/CThing.h:65-66). Opt-ins: FABLE_RETAIL_CTHING, FABLE_RETAIL_CTHING_GETTC_EXPANDED (GetTC(id,T**)/GetTC(T**) bodies written exactly as retail expands: key copied into a local INSIDE the test, map via pointer local, end-or-greater conditional), FABLE_RETAIL_CTHING_HASTC_FORCEINLINE.
- CTCVillage +0x2C = `VillagePeople` std::list<CIntelligentPointer<CThingAICreature> > (engine/CTCVillage.h:155, STATIC_ASSERT line 246; Ego_r type). Draft called it "VillageMembers" (WRONG name, wrong element type CThing). Opt-ins: FABLE_RETAIL_CTCVILLAGE (real CTCBase base + typed lists), FABLE_RETAIL_CTCVILLAGE_TCI (StatGetInterfaceType -> TCI_VILLAGE, enables GetTC(&village)), FABLE_RETAIL_CTCVILLAGE_PEEKVILLAGEPEOPLE (const list& accessor; gives const_iterators -- NOTE retail here reads [ebp+2Ch] directly, either works for a plain loop).
- list node: next +0, prev +4, value CIntelligentPointer +8 (`lea ecx,[edi+8]`).
- CTCCreatureModeManager::StatGetInterfaceType() = TCI_ENTITY_MODE_MANAGER (49), unconditional in engine/CTCCreatureModeManager.h:12.
- Enums: engine/ETCInterfaceType.h TCI_VILLAGE=34, TCI_ENTITY_MODE_MANAGER=49; NCreatureMode::MODE_SLEEP_BED=13.
- Locally modelled classes in old draft that HAVE shared headers: CCharString/CBasicString (fable_string.h), CThing (engine/CThing.h), CVectorMap (engine/CVectorMap.h), CBaseIntelligentPointer (engine/CIntelligentPointer.h), CTCVillage (engine/CTCVillage.h), CTCCreatureModeManager + CModeBase (engine/CTCCreatureModeManager.h), CTCBase (engine/CTCBase.h).

## 5. Callee visibility
- CVectorMap::LowerBound: header gives a same-TU `__declspec(noinline)` body by default (CVectorMap.h:170) -- matches retail's out-of-line call and lets VC7.1 fold the key compare (`cmp [eax],22h`). Do NOT define FABLE_RETAIL_CVECTORMAP_INLINE_LOWERBOUND.
- CBasicString<char>::operator==(const char*): called out of line (0x004115A0) -> leave FABLE_RETAIL_CBASICSTRING_TEXT_EQ_INLINE undefined. CCharString::operator==(const char*) is the inline header body `if(!PStringData){ if(strcmp(text,"")==0) return true; return false;} return PStringData->Data==text;` (fable_string.h:409) and is expanded here; if VC calls it out of line, FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ forceinlines it. Write the source as `if (!(name == "STREETLIFE")) return -1.0f;` (or `name != ...` if defined) rather than the draft's hand-expanded version.
- GetCurrentMode 0x0073A6E0 and GetPItem 0x00A01B50 are real out-of-line functions (other TUs).
- GetTC<T>(T**) is FABLE_FORCEINLINE in the header (owner decision 2026-11-01).

## 6. Siblings (same TU neighbourhood, retail order 0x7F50B0 / 0x7F5150 / 0x7F5370 / 0x7F53A0 / 0x7F53B0 / 0x7F5560 / 0x7F5620)
- rebuild/src/compiled/00/7f/CTCSoundAtmosphereVillage_OnInitialActivate_007f50b0.cpp -- no pragmas/Flags; `#include "fable_compiler.h"`, engine/CVectorMap.h shared, local CThing/HasTC/GetTC (FABLE_FORCEINLINE) and FABLE_FORCEINLINE `GetVillage()` returning an unset local on absent path; same CTCSoundAtmosphere local model. Its village loop is the twin of this one (VillageBuildings +0x28).
- rebuild/src/compiled/00/7f/CTCSoundAtmosphere_OnInitialActivate_007f53a0.cpp -- no pragmas.
- rebuild/src/compiled/00/7f/CTCSoundAtmosphere_scalardeletingdestructor_007f5620.cpp -- `#define FABLE_RETAIL_CTCBASE`.
- rebuild/src/compiled/00/6c/CTCBuyableHouse_FrameUpdate_006c1250.cpp (Flags: none) -- the canonical shared-header village/members idiom: FABLE_RETAIL_CTCBASE, CTHING, CTHING_GETTC_EXPANDED, CTHING_HASTC_FORCEINLINE, CINTELLIGENTPOINTER, CTCVILLAGE, CTCVILLAGE_PEEKVILLAGEPEOPLE, CCHARSTRING.
- rebuild/src/compiled/00/6e/CTCDParticleEmitter_IsParticleCreationDisabled_006e0df0.cpp -- FABLE_RETAIL_CTHING + GETTC_EXPANDED, `PeekTC(TCI_ENTITY_MODE_MANAGER,&modes)` then `modes->GetCurrentMode().GetType()`.
- Flags for this row: none (no /Oy-, no optimize("s")).

## 7. Old draft model errors (vs disassembly)
- The lane note says residual is pure register allocation (this in ebx vs retail's frame spill; village in ebp; 0x31 in ebx). Key structural lead: retail's uninitialised `village` and `modes` and both lookup KEYS all live in the dead argument slot [esp+0x18], with the key store AFTER the presence test -- exactly the FABLE_RETAIL_CTHING_GETTC_EXPANDED shape of `GetTC(ETCInterfaceType id, T** ptc)` (`ETCInterfaceType type=id;` inside `if(HasTC(id))`, map via pointer local, `*ptc = ((it==end||type<it->first)?end:it)->second`). The draft's GetTC copies the key BEFORE the test-free lookup and uses `if` re-assignment of `it`; header comment says the conditional form beats the if form. Likely original source: `CTCVillage* village; GetParentThing().GetTC(&village);` and inside the loop `CTCCreatureModeManager* modes; member->GetTC(&modes);` (non-const GetTC -- both lookups call the NON-const LowerBound 0x0040F020; PeekTC would call const 0x004365B0). Declaring `modes` OUTSIDE the loop matches `mov esi,[esp+18h]` before the loop (value carried across iterations).
- Wrong types/names: GetCurrentMode (const, CModeBase&), GetType (not GetModeType), GetPItem returns CBaseObject*, list is VillagePeople of CIntelligentPointer<CThingAICreature>, LowerBound spelled CInterfaceMap, CBasicStringChar.
- Hand-expanded string compare: draft writes `strcmp("STREETLIFE","")` directly; use the CCharString inline operator==.
- Loop: retail re-reads end() ([ebp+2Ch]) each iteration and has a top guard -> `for (it=list.begin(); it!=list.end(); ++it)` over std::list iterators (not a raw node pointer copy of head) is plausible; draft's raw-node for is fine bytes-wise but try the STLport iterator form.
- Draft modelled ParentThing as a raw CTCSoundAtmosphere member; derive from shared CTCBase.
- Gain/clamps and the 5.0f reset are correct in the draft.

## 0x00863CA0

# DOSSIER 0x00863CA0 CThingBodyReorienter::FillBodyReorientationPossibleIntersecting (358 B)

## 1. Retail shape (xdis 00863ca0 0x176)
- thiscall, plain `ret` (no args), `?...@@AAEXXZ` = private, non-const, void(void).
- Prologue `push ebp; mov ebp,esp; and esp,-8; sub esp,30h` = VC7.1 ALIGNED frame (not /Oy-): ebp is then re-pushed
  (0x863cbd) and used as a general register (loop iterator). No SEH (no /EHsc frame), though temporaries have dtors.
- Frame map (F = esp after sub): F+0x07 __false_type tag for _M_insert_overflow; F+0x08 size float, SAME slot reused for the
  8-byte CIntelligentPointer temporary in the loop; F+0x10 CThingFilter_HasPhysicsMesh (empty); F+0x14..0x1F local
  vector<CThing*> (start/finish/eos); F+0x20..0x2F C2DBoxI (TLX,TLY,BRX,BRY). F+0x00..0x06 unused (the 8-alignment cause is
  not identified; old notes: ours = 0x20 frame, box packed over the size float).
- Order: this->HasPossibleIntersectingThings(+0x14)=1, then StillChecking...(+0x08)=1; Body(+0x24)->PhysicsTC(+0x60)->Radius
  (+0x34) `fadd st0,st0` (=GetDiameter, r*2.0f); fst to frame; fcomp 1.0f [0x122DED8]; `test ah,5; jp` -> if !(size>=1)?
  actually keeps size unless size<1 then size=1.0 (stored as imm 0x3f800000). Then GetPos() call, __ftol2 of pos.X (edi),
  pos.Y (esi), size (eax); box fields computed interleaved (edx=Y-s, esi=Y+s, ecx=X-s, edi=X+s) = direct four-field init.
- Vector zero stores (`xor esi,esi`, 3 stores) are SUNK to just before the search call, after g_pMainGameComponent load.
- Loop: `begin==end` test, then do-while with ebp=it, end re-read from memory each iteration ([esp+0x28]; vector address
  escaped); after loop `mov ebp,[start]; xor esi,esi` re-materialised; then `if (PossibleIntersectingThings.start ==
  finish) { Done=1; StillChecking=0; HasPossible=0; }`; then local vector dtor `if (start) free(start)` (no element dtors).
- Loop body: temp = CIntelligentPointer<CThing>(*it): call base ctor 0xA01B10, push item, vptr 0x01238C6C stored, SetPItem;
  push_back inline: if finish!=eos { if(finish) copy-ctor inline (base ctor, vptr, GetPItem(temp), SetPItem); finish+=8 }
  else _M_insert_overflow(finish, temp, __false_type(), 1, true) 0x004AC940; then temp dtor DIRECT call 0xA01C10.

## 2. Name (two sources) -- manifest label is CORRECT
- Ego_r.pdb egor_pdb_names.tsv 0x00832BD0 and FableWin.pdb (_fablewin_api.tsv / FableWin.exe.signatures.tsv 0x02834200):
  `?FillBodyReorientationPossibleIntersecting@CThingBodyReorienter@@AAEXXZ` -> `private: void __thiscall ...(void)`.
- Only caller 0x00863E1F inside landed CThingBodyReorienter::UpdateBodyReorienation 0x00863E10 (which names it).
- No vtable slot. FableWin also has `?GetThingSearchTools@CThingBodyReorienter@@ABEAAVCThingSearchTools@@XZ` (private const
  inline accessor; retail reads g_pMainGameComponent+0x24 -> +0x20 in place) -> use it.

## 3. Callees
| addr | landed file | PDB name | draft spelling |
|---|---|---|---|
| 0x004C73D0 | rebuild/src/compiled/00/4c/CThing_GetPosition_004c73d0.cpp (Leaf ?GetPos@CThing@@QBEABVC3DVector@@XZ) | CThing::GetPos() const | WRONG: draft `GetPosition` -> use `GetPos()` (engine/CThing.h:411) |
| 0x00BFEA70 x3 | CRT | __ftol2 | (long) casts, ok |
| 0x00863E70 | rebuild/src/compiled/00/86/CThingSearchTools_GetAllThingsInRoughArea_00863e70.cpp | Leaf `??$GetAllThingsInRoughArea@VCThing@@V?$vector@PAVCThing@@...@VCThingFilter_HasPhysicsMesh@@@CThingSearchTools@@QBEXABVC2DBoxI@@ABVCThingFilter_HasPhysicsMesh@@AAV?$vector@PAVCThing@@...@@@Z` (FableWin 017a2088); Ego_r has the `$$CBVCThing`/vector<const CThing*> variant (0x00832D50) | DISAGREES: draft calls `<const CThing>` with vector<CThing*>; landed symbol is T=CThing, vector<CThing*> -> call `GetAllThingsInRoughArea<CThing>(box, filter, things)` const member, `ret 0xC` |
| 0x00A01B10 | (shared) | ??0CBaseIntelligentPointer@@QAE@XZ | ok |
| 0x00A01B90 | | ?SetPItem@CBaseIntelligentPointer@@QAEXPAVCBaseObject@@@Z | draft takes CThing* (PDB: CBaseObject*) |
| 0x00A01B50 | | ?GetPItem@...@@QBEPAVCBaseObject@@XZ | draft returns CThing* (PDB: CBaseObject*) |
| 0x00A01C10 | | ??1CBaseIntelligentPointer@@UAE@XZ (VIRTUAL dtor) | draft non-virtual dtor + dummy v00 |
| 0x004AC940 | (COMDAT, not this TU) | vector<CIntelligentPointer<CThing> >::_M_insert_overflow(pos, const T&, const __false_type&, size_type, bool) | ok |
| 0x00BFEA14 | CRT | free (STLport allocator deallocate) | ok |
- Data: 1.0f 0x0122DED8; CIntelligentPointer<CThing> vtable 0x01238C6C; g_pMainGameComponent 0x013B86A0 = static
  CMainGameComponent::PMainGameComponent (engine/CMainGameComponent.h:58, `Get()`).

## 4. Members
- CThingBodyReorienter: NO engine header (all siblings model it locally). struct_layouts_egor.tsv (Ego_r, 0x2C):
  BodyReorientationDone bool +0, ReorientingBody bool +1, ReorientationCount long +4, StillCheckingForNewBodyReorientationAngle
  bool +8, HaveFinalReorientationAngle bool +9, FinalReorientationAngleXY float +0xC, RotationDirection ERotationDirection
  +0x10, HasPossibleIntersectingThings bool +0x14, PossibleIntersectingThings vector<CIntelligentPointer<CThing>> +0x18
  (PDB 16-byte Dinkumware; retail STLport 12), Body `CThing *` +0x28 PDB = retail +0x24 (as landed 0x00863790/0x00863280).
- CThing::PhysicsTC +0x60 (engine/CThing.h:70, GetPhysicsTC() inline :402/:410) under FABLE_RETAIL_CTHING.
- CTCPhysicsBase::Radius +0x34 float (engine/CTCPhysicsBase.h:329, static_assert :360); GetRadius() inline;
  GetDiameter() = GetRadius()*2.0f behind FABLE_RETAIL_CTCPHYSICSBASE_DIAMETER (matches retail `fadd st0,st0`).
  Macros: FABLE_RETAIL_CTCPHYSICSBASE, FABLE_RETAIL_CTCBASE.
- CMainGameComponent +0x24 PWorld (scoped_ptr word): GetWorld() behind FABLE_RETAIL_CMAINGAMECOMPONENT_GETWORLD
  (PeekWorld() const behind ..._WORLD); base FABLE_RETAIL_CMAINGAMECOMPONENT.
- CWorld +0x20 PThingSearchTools_ptr: GetThingSearchTools() / PeekThingSearchTools() const (engine/CWorld.h:190-191),
  FABLE_RETAIL_CWORLD.
- C2DBoxI (engine/C2DBoxI.h): TLX,TLY,BRX,BRY; user default ctor; `C2DBoxI(const C2DCoordI&, long radius)` with
  FABLE_RETAIL_C2DBOXI_RADIUS_INITIALIZERS = direct four-field init (TLX(p.X-r)...) -- exactly the retail box; also
  FABLE_RETAIL_C2DBOXI_CTORS (4-long ctor). C2DCoordI in engine/C2DCoordI.h.
- CIntelligentPointer<T> (engine/CIntelligentPointer.h): FABLE_RETAIL_CINTELLIGENTPOINTER (pack(1), virtual dtor, PData +4),
  (T*) ctor inline via SetPointer->SetPItem; inline copy ctor (base ctor, vptr, GetPItem, SetPItem) behind
  FABLE_RETAIL_CINTELLIGENTPOINTER_COPY (exactly the retail placement copy in push_back; chunk 754 row 0x0082A890 same idiom).
- Old draft models LOCALLY although shared headers exist: C3DVector, C2DCoordI, C2DBoxI, CTCPhysicsBase, CThing,
  CBaseIntelligentPointer/CIntelligentPointer, CWorld, CMainGameComponent, CThingSearchTools (the landed callee itself aliases
  engine/CThingSearchTools.h because the header's vector is 16-byte Dinkumware).

## 5. Visibility
- Same TU (CThingBodyReorienter.cpp, retail order): IsUpdateNeeded 0x00863250, GradualReorientBody 0x00863280,
  ReorientBodyYZForSlopes 0x008633B0, ~CThingBodyReorienter 0x00863790, CheckForNewBodyReorientationAngle 0x008637D0 (not
  landed), THIS 0x00863CA0, UpdateBodyReorienation 0x00863E10, then the instantiation GetAllThingsInRoughArea 0x00863E70 (its
  template body is visible to this TU but emitted out of line; landed copy wraps STLport helpers in `#pragma auto_inline(off)`).
- Out-of-line: vector<CIntelligentPointer<CThing>>::_M_insert_overflow 0x004AC940 (shared COMDAT, low address) -> declare it
  noinline/out of line; CBaseIntelligentPointer members (0xA01Bxx) are out of line in another TU.
- Inline: CThing::GetPos is NOT inline (call 0x4C73D0); GetPhysicsTC/GetDiameter/GetRadius, World/ThingSearchTools accessors,
  CIntelligentPointer (T*) and copy ctors, C2DBoxI ctor, vector push_back/empty/begin/end/dtor, filter ctor are inline.
- STLport model with the inline `_Vector_base()` ctor and `~_Vector_base` (`if (_M_start) deallocate`) exists in the landed
  dtor rebuild/src/compiled/00/86/CThingBodyReorienter_CThingBodyReorienter_00863790.cpp; rebuild/include/stlport has only
  fable_containers.h etc. (no vector opt-in found).

## 6. Siblings
- 0x00863280 GradualReorientBody: `// Flags: none`; defines FABLE_RETAIL_CCHARSTRING, _CCLASS, _DEF_CLASS_BASE, _C3DVECTOR,
  _CTCBASE, _CTHING, _CTCPHYSICSBASE; includes fable_string.h, engine/C3DVector.h, CThing.h, CTCBase.h; local
  CThingBodyReorienter with Body +0x24; `#pragma intrinsic(fmod, fabs)`.
- 0x00863790 ~CThingBodyReorienter: local STLport vector/_Vector_base/_Destroy model + engine/CIntelligentPointer.h
  (FABLE_RETAIL_CINTELLIGENTPOINTER).
- 0x00863E10 UpdateBodyReorienation: no defines, local class, plain C++.
- 0x00863E70 GetAllThingsInRoughArea: header-evidence aliasing, local STLport vector, `#pragma auto_inline(off/on)` around
  out-of-line helpers.
- Caller-of-search idioms: 0x00877D20 CTools::FrameUpdate (box temp, vector<CIntelligentPointer> local),
  0x00666EF0 CThingCreatureBase::AutoPlaceNearBuilding (FABLE_RETAIL_CTHINGSEARCHTOOLS, FABLE_RETAIL_CWORLD,
  `GetWorld().GetThingSearchTools()`).

## 7. Old draft model errors
- Wrong callee name GetPosition (PDB GetPos); wrong template arg `<const CThing>` vs landed `<CThing>`.
- Size as raw `Radius * 2.0f` instead of GetPhysicsTC()->GetDiameter() (same arithmetic; fine) ; search tools read through raw
  members instead of the private inline CThingBodyReorienter::GetThingSearchTools() const (FableWin) -> CMainGameComponent::Get()
  ->GetWorld().GetThingSearchTools().
- Separate C2DCoordI centre + 4-long box ctor; retail = C2DBoxI(C2DCoordI(x,y), (long)size) radius ctor (header opt-in).
- CBaseIntelligentPointer local with non-virtual dtor and fake v00; header has virtual dtor + CBaseObject* Get/SetPItem.
- push_back with `_M_insert_overflow(_M_finish, x, __false_type(), 1, true)`: matches retail arg order; keep.
- Residue (NOTES.md hand254 line 64): frame 0x20 vs retail 0x30 aligned and vector zeroing at top vs sunk. Retail's F+0x00..0x06
  hole + `and esp,-8` imply an 8-aligned local VC7.1 kept a slot for (double/__int64-like or 8-aligned temp) -- the old tries
  (vector/filter placement, named box, C2DCoordI centre) did not reach it; untried: _Vector_base inline ctor (as 0x00863790),
  header C2DBoxI radius ctor, GetDiameter/GetThingSearchTools accessors.

## 0x00688D40

# DOSSIER 0x00688D40 -- CInputProcessZTarget::AddToInterface (360 bytes)

## 1. Retail shape (python3 $S/w17/xdis.py 00688d40 0x178)
- thiscall, `ret 4`, returns bool from a direct (non-virtual) call to CInputProcessGameBase::AddToInterface 0x00687A70 (call, then epilogue; not a jmp).
- NO frame pointer: `sub esp,8 ; push ebp ; mov ebp,ecx ; push esi` -> /Oy default (ebp = this). No SEH prologue, no /GS cookie. No pragmas needed.
- One 8-byte stack slot [esp+8 at entry] reused for: temp CIntelligentPointer<const CThingPlayerCreature>, the CDefPointer return temp
  (hidden ptr `lea eax,[esp+0Ch]` after `push edi`), the ETCInterfaceType key (`mov [esp+0Ch],8`), and temp CIntelligentPointer<const CThing>.
- Register roles retail: this=ebp, &PCreature (ebp+28h)=esi, CDefPointer return (ptr to temp)=edi (pushed only inside the alive block,
  popped right after the HasTC load `pop edi` before `je`), map ptr then end = esi, target = esi. Old draft had this=edi/def=ebp (the residue).
- Flow:
  1. item = MyPlayer->PeekPControlledCreature() (0x00487DD0, `mov ecx,[ebp+18h]`) evaluated BEFORE the temp ctor 0x00A01B10; vptr 0x01237F14
     stored after `push esi`; SetPItem 0x00A01B90; then op=: GetPItem(temp) FIRST, then `lea esi,[ebp+28h]` + SetPItem (dest formed after the
     read = header FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN body); temp dtor = direct call 0x00A01C10 (no vptr re-store).
  2. alive test: GetPItem(PCreature) null test, GetPItem again, `test byte [eax+91h],1 ; jne` (Dead bit) -- IS_THING_ALIVE shape, two GetPItem calls.
  3. PHeroCombatDef = PCreature->GetPHeroCombatDef() (third GetPItem, call 0x00663340, hidden ptr pushed before GetPItem). Copy operator=:
     `mov ecx,[ebp+30h]; cmp ecx,[edi]; je` / release `dec [ecx+4]; jne; call [vtbl+4]` / `mov [ebp+30h],0` / re-read `mov eax,[edi]` /
     store / `inc [eax+4]` = engine/CDefPointer.h operator= (re-reads other.Object after Release; NOT Assign).
     Temp dtor: `mov edi,[eax+4]; mov ecx,eax; add eax,4; dec edi; mov edx,edi; test edx,edx; mov [eax],edi; jne; call [vtbl+4]` -- this is the
     FABLE_RETAIL_CDEFPOINTER_RELEASE_COUNTREF shape (cf 0x0043A587), while the op= release is the in-place `dec [ecx+4]` form. Test the
     default Release first, then COUNTREF; the draft got both by giving op= and dtor different Release spellings (it matched this part).
  4. HasTC: fourth GetPItem (`mov ecx,esi`), `mov ecx,[eax+20h]; test ch,1` (UsingComponents bit 8 = TCI_TARGETING), je to tail.
  5. PeekTC expanded: `lea esi,[eax+44h]`, key 8 stored after the push, call 0x004365B0 (CONST LowerBound), `mov esi,[esi+4]` (end through the
     map pointer), `cmp eax,esi; je; cmp [eax],8; jle` (conditional expression), `mov ecx,[eax+4]` straight into ecx, `call [edx+84h]`
     = GetPTargetedThing (slot 33). Exactly the FABLE_RETAIL_CTHING_GETTC_EXPANDED PeekTC body (CThing.h ~line 318).
  6. target null / `test [esi+91h],1` dead test, then PTarget = CIntelligentPointer<const CThing>(target): ctor 0x00A01B10, vptr 0x01237F1C,
     SetPItem, GetPItem(temp), `lea ecx,[ebp+38h]` SetPItem, dtor 0x00A01C10.
  7. tail: `mov eax,[esp+14h]` (manager) push, `mov [ebp+34h],-1` (DeactivateHoldTimer = -1), call 0x00687A70.

## 2. Identity (proven, two sources)
- Ego_r.pdb (ghidra_out/egor_pdb_names.tsv): ?AddToInterface@CInputProcessZTarget@@UAE_NAAVCInputProcessManager@@@Z @Ego_r 0x006B4480.
  Donor order DeactivateZTarget 006B4300 -> Update 006B4380 -> AddToInterface 006B4480 -> ProcessInput 006B4600 matches retail
  0x00688BA0 -> 0x00688C40 (landed Update) -> 0x00688D40 -> 0x00688EB0 (landed ProcessInput).
- rebuild/manifest/rtti-vtables.tsv: CInputProcessZTarget vtable 0x01238028 slot 1 = 0x00688D40 (only xref: .rdata 0x0123802C).
- Manifest label is CORRECT. Signature: `virtual bool AddToInterface(CInputProcessManager& manager)` (public virtual, non-const, ref param).
- Base: Ego_r ?AddToInterface@CInputProcessGameBase@@UAE_NAAVCInputProcessManager@@@Z (Ego_r 0x006B2B80) = retail 0x00687A70; root
  ?AddToInterface@CAInputProcess@@UAE_N... (Ego_r 0x00B74BA0) = retail 0x00A0D2B0 (0x00687A70 tail-jumps there).

## 3. Callees
| retail | landed row | landed name | PDB name | draft spelling |
|---|---|---|---|---|
| 0x00487DD0 | rebuild/src/compiled/00/48/CPlayer_PeekPControlledCreature_00487dd0.cpp | CPlayer::PeekPControlledCreature() const -> const CThingPlayerCreature* | same | WRONG: GetControlledCreature (non-const, CThing*) |
| 0x00A01B10 | .../00/a0/CBaseIntelligentPointer_CBaseIntelligentPointer_00a01b10.cpp | CBaseIntelligentPointer ctor | same | ok |
| 0x00A01B50 | .../00/a0/CBaseIntelligentPointer_GetPItem_00a01b50.cpp | GetPItem() const -> CBaseObject* | same | draft returns CThing* (wrong type) |
| 0x00A01B90 | .../00/a0/CBaseIntelligentPointer_SetPItem_00a01b90.cpp | SetPItem(CBaseObject*) | same | draft takes CThing* |
| 0x00A01C10 | .../00/a0/CBaseIntelligentPointer_CBaseIntelligentPointer_00a01c10.cpp | virtual ~CBaseIntelligentPointer | same | draft: non-virtual dtor + bogus v00 |
| 0x00663340 | .../00/66/CThingCreatureBase_GetPHeroCombatDef_00663340.cpp | CThingCreatureBase::GetPHeroCombatDef() const -> CDefPointer<const CHeroCombatDef> | ?GetPHeroCombatDef@CThingCreatureBase@@QBE?AV?$CDefPointer@$$CBVCHeroCombatDef@@@@XZ | WRONG: CThingPlayerCreature::GetHeroCombatDef |
| 0x004365B0 | .../00/43/CVectorMap_LowerBound_004365b0.cpp | CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound(const ETCInterfaceType&) const | same (const overload) | draft: CTCMap::LowerBound(const unsigned long&) |
| vslot 33 (+0x84) | .../00/80/CTCTargetingPlayer_GetPTargetedThing_00802610.cpp | GetPTargetedThing() const -> CThing* | ?GetPTargetedThing@CTCTargetingBase@@UBEPAVCThing@@XZ | WRONG: GetCurrentTarget (sibling ProcessInput 0x00688EB0 says GetTargetThing -- also wrong, NAME_FIXES candidate) |
| 0x00687A70 | .../00/68/CInputProcessGameBase_AddToInterface_00687a70.cpp | landed as CInputProcessGameBase_Methods::AddToInterface(CInputProcessManager*) + "Helper" for 0x00A0D2B0 | ref param; 0x00A0D2B0 = CAInputProcess::AddToInterface | NAME_FIXES candidate (pointer vs reference, Helper) |

## 4. Members
- CInputProcessZTarget: shared header rebuild/include/engine/CInputProcessZTarget.h (needs FABLE_RETAIL_INPUTPROCESS, FABLE_RETAIL_CINTELLIGENTPOINTER,
  FABLE_RETAIL_DEF_CLASS_BASE). Ego_r (struct_layouts_egor.tsv, sizeof 64): PCreature CIntelligentPointer<const CThingPlayerCreature> +0x28,
  PHeroCombatDef CDefPointer<const CHeroCombatDef> +0x30, DeactivateHoldTimer long +0x34, PTarget CIntelligentPointer<const CThing> +0x38.
  Header declares NO AddToInterface virtual -> author must add a declaration (nested opt-in in the header, e.g. FABLE_RETAIL_..._ADDTOINTERFACE)
  and CInputProcessGameBase.h / CAInputProcess.h declare no AddToInterface either (virtual slot 1 after the dtor). A qualified
  `CInputProcessGameBase::AddToInterface(manager)` call needs the base declaration (a new opt-in in CInputProcessGameBase.h, decl only).
- CInputProcessGameBase.h (FABLE_RETAIL_INPUTPROCESS): MyPlayer CPlayer* +0x18.
- CThing.h: UsingComponents std::bitset<274> +0x20; Components CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<...>> +0x44;
  `unsigned char Dead : 1` at +0x91 bit0; IsDead()/IsAlive(); HasTC (FABLE_RETAIL_CTHING_HASTC_FORCEINLINE optional);
  PeekTC<T>(T**) const under FABLE_RETAIL_CTHING_GETTC_EXPANDED (matches step 5).
- CPlayer.h (FABLE_RETAIL_PLAYER): PeekPControlledCreature() const.
- CThingCreatureBase.h: GetPHeroCombatDef() const under FABLE_RETAIL_CTHINGCREATUREBASE_HEROCOMBATDEF (no landed user yet). Row 0x00487DD0 shows the
  macro set to make CThingPlayerCreature complete: CCHARSTRING, CCLASS, DEF_CLASS_BASE, CTCBASE, CTHINGPHYSICAL, CTHINGGAMEOBJECT, CTHINGCREATUREBASE, CINTELLIGENTPOINTER.
- CTCTargetingBase.h (needs FABLE_RETAIL_UPDATE_NEAREST_INTERESTING or FABLE_RETAIL_FIRE_RANGED_WEAPON): StatGetInterfaceType()=TCI_TARGETING (8), GetPTargetedThing() const.
- CDefPointer.h opt-ins: default Release, FABLE_RETAIL_CDEFPOINTER_RELEASE_COUNTREF, _RELEASE_POINTEE, _RELEASE_SELFDELETING_POINTEE, _RESET.
- CIntelligentPointer.h opt-ins: FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN (item read then SetPItem -- needed), _COMPARE_NULL_INT (for IS_THING_ALIVE `thing!=0`),
  _CTOR_OUTOFLINE / _ASSIGN_OUTOFLINE (NOT wanted: retail inlines both here). ThingAlive.h: FABLE_RETAIL_IS_THING_ALIVE.
- Locally modelled in the draft though shared headers exist: CInputProcessZTarget, CInputProcessGameBase, CPlayer, CThing, CThingPlayerCreature,
  CBaseIntelligentPointer/CIntelligentPointer, CDefPointer, CHeroCombatDef, CVectorMap (CTCMap/STCEntry), CTCTargeting (-> CTCTargetingBase).

## 5. Callee visibility
- Inline in retail (bodies expanded here): CIntelligentPointer(T*) ctor, operator=, implicit derived dtor; CDefPointer op=/dtor/Release (header forceinlines);
  HasTC / PeekTC / IS_THING_ALIVE / IsDead / Get / operator->.
- Out of line (declaration only, do not give bodies): 0x00A01B10/B50/B90/C10, 0x00487DD0, 0x00663340, 0x004365B0, 0x00687A70.
- Same TU neighbours (retail order 0x00688BA0 DeactivateZTarget, 0x00688C40 Update, this, 0x00688EB0 ProcessInput) -- no small inline helpers between them.
- Old draft note: plain `inline` CIntelligentPointer ctor/op= went out of line (332v360); __forceinline made them inline but swapped this/def
  registers (362v360). Header's ctor/op= are plain inline in a pack(1) class -- test whether header form inlines; the sibling ProcessInput 0x00688EB0
  sidesteps it by writing a scoped explicit local: `{ CIntelligentPointer<..> p; p.SetPItem(x); PTarget.SetPItem(p.GetPItem()); }` -- BUT retail here
  stores the vptr AFTER `push esi` (arg evaluated first) which is the (T*) ctor's SetPointer, same instruction stream as a default ctor + SetPItem.

## 6. Siblings
- rebuild/src/compiled/00/68/CInputProcessZTarget_ProcessInput_00688eb0.cpp (404 B, same class, no pragmas/flags; local models; scoped temp idiom,
  IS_THING_ALIVE forceinline with two GetPItem; GetInterface expanded with pointer-to-map local).
- rebuild/src/compiled/00/68/CInputProcessZTarget_Update_00688c40.cpp (80 B, no pragmas; local model).
- rebuild/src/compiled/00/68/CInputProcessControlCreatureActivateZTarget_ProcessInput_00688260.cpp: `// Flags: /GS`, macros FABLE_RETAIL_INPUTPROCESS,
  _INPUTPROCESS_ZTARGET, _PLAYER, _CGAMEPLAYERINTERFACE_TOGGLE_ZTARGET; shared headers (CPlayer.h, CInputProcess*.h). (This row has no /GS cookie.)
- rebuild/src/compiled/00/48/CPlayer_PeekPControlledCreature_00487dd0.cpp: macro set for complete CThingPlayerCreature + CIntelligentPointer.
- GETTC_EXPANDED users: rebuild/src/compiled/00/83/CThingNoise_SetCreator_008386e0.cpp, CThingAICreature_OnHeroPush_008328d0.cpp.
- COUNTREF users: rebuild/src/compiled/00/64/NPlayerGuiCDrawMoney_ClearPrimitivesIfNecessary_00644263.cpp, 00/65/CTCVillage_RemoveCrimeIfApologisable_0065c1fc.cpp.

## 7. Draft model errors
- Wrong callee names (GetControlledCreature, GetHeroCombatDef on CThingPlayerCreature, GetCurrentTarget) and wrong types (non-const CThing*,
  GetPItem returning CThing*); PCreature/PTarget are pointers to CONST types (vtables 0x01237F14 / 0x01237F1C) and LowerBound is the CONST overload.
- CBaseIntelligentPointer modelled with a non-virtual dtor plus fake v00 virtual; header has virtual dtor (slot 0) -- affects inline dtor/vptr handling.
- Local CTCMap::Find / HasTC instead of CThing::PeekTC (GETTC_EXPANDED) -- the expanded form (key local inside test, map via pointer local,
  conditional expression) matches retail byte-for-byte in step 5.
- Creature re-read: retail calls GetPItem 4 times on PCreature (null, dead, def call, HasTC) -- the draft's shape is right on this.
- Remaining residue was register assignment (this ebp vs edi); likely driven by local-model differences (CDefPointer temp handling, forceinline ctor).

## 0x006EA0C0

# DOSSIER 0x006EA0C0 (360 bytes incl. 2 pad; code ends 0x006EA228 at `ret 4`)

## 1. Identity (manifest label is WRONG)
- Real name: `CCombatManager::UpdateDefenderAttackPattern(CCombatantInfo& info)`, private, void, non-const.
  Leaf `?UpdateDefenderAttackPattern@CCombatManager@@AAEXAAUCCombatantInfo@@@Z`.
  - src 1: ghidra_out/egor_pdb_names.tsv line 14991 (Ego_r donor 0x00712D10); FableWin signatures `01839b59 ...UpdateDefenderAttackPattern@CCombatManager@@AAEXAAUCCombatantInfo@@@Z` (ghidra_out/_fablewin_api.tsv / naming_stragglers).
  - src 2: sole caller 0x006ED04D inside landed CCombatManager::UpdateAttackingCombatantsList 0x006ED030
    (rebuild/src/compiled/00/6e/CCombatManager_UpdateAttackingCombatantsList_006ed030.cpp, header comment + line 313
    `void UpdateDefenderAttackPattern(CCombatantInfo& info); // 0x006EA0C0`), called with ecx=this(CCombatManager), push info.
  - Note CCombatantInfo is a STRUCT in the mangling (`AAU`), CCombatManager a class.
- Manifest `CDefendingCombatantInfo::InitializeDefenderFromCreature` is made-up/wrong; `this` is CCombatManager, NOT CDefendingCombatantInfo.
- Same retail neighbourhood is the CCombatManager TU (CombatManager.cpp): 0x006E8600 IncrementDefenderAttackPattern, 0x006E86A0
  UpdateDefenderAttackInstruction, 0x006E8C30 ResetDefenderAttackPattern, 0x006EA2C0 PerformSequenceReset, 0x006EA430 DeregisterCombatant ...

## 2. Retail shape (xdis 006ea0c0 0x178)
- __thiscall, `ret 4`, NO frame pointer (sub esp,10h; push ebx/ebp/esi/edi) -> /Oy. NO SEH prologue although a CCharString temp and a
  CDefPointer local are destroyed -> TU built without /EHsc. Sibling flags in this TU: `// Flags: /O2 /Oy /W3`
  (CCombatManager_CCombatManagerRebuildCombatLists_006eba60.cpp, CCombatManager_GetCombatDebugInfo_006eac50.cpp). Not size-optimised.
- Stack locals (offsets from esp after the 4 pushes): +0x10 pattern (=0 at 0x6EA0DD, kept in ebp, spilled), +0x14 ownIndex,
  +0x18 saved `this`, +0x1C CDefString temp (slot-10 return), +0x24 CCharString temp then reused for the defender's creature type,
  later reused for the CDefPointer local.
- Flow:
  1. `info.DefenderInfo.PAttackPattern->GetInstantiationName()` : ecx=[info+0x64], `call [vtbl+0x28]` (CDefClassBase slot 10, CDefString
     by value via hidden ptr), no null test.
  2. CDefString -> CCharString conversion fully inlined: `push [ret]; push &tmp; mov ecx,0x013CA828; call 0x009D49B0` (GetString inlined
     through operator CCharString / CDefString::GetString, default branch of engine/CDefString.h).
  3. ownIndex = `World.PeekDefinitionManager().GetDefGlobalIndexFromName(tmp)` : ecx=[[this+0x20]+0x10], call 0x009AD410; then tmp
     dtor call 0x0099EAE0.
  4. creatureType = `info.Parent->GetCombatTypeDef().CombatCreatureType` : ecx=[info], call 0x00666D10, read [eax+0x90].
  5. for (it = Attackers.begin(); it != Attackers.end(); ++it)  -- header [info+0x34] reloaded each test (cmp eax,[ebx+34h]);
     `je` skip whole loop when begin==end (header->left == header). Fully inline _M_increment (right-subtree min / parent climb with
     `if (node->_M_right != parent) node = parent`).
     body: `const CCombatTypeDef& def = it->second->GetCombatTypeDef();` (node value +0x18 = CThingCreatureBase*; call 0x00666D10)
           `map<ECombatCreatureType,long>::const_iterator f = def.AttackPatterns.find(creatureType);` -- inline _M_find
           (header = [def+0x84], root = header->_M_parent; key at node+0x10 signed `jl`; `if (y==header || key < y->key) y=header`).
           `if (f != def.AttackPatterns.end() && pattern == 0) pattern = f->second;`  (end() re-read as [eax+84h]; found test FIRST,
           then `test ebp,ebp`; value at node+0x14).
  6. `if (pattern != 0 && pattern != ownIndex)` { CDefPointer<const CAttackPatternDef> p; (mov [p],0)
        World.PeekDefinitionManager().GetDef(pattern, &p);   // call 0x006F0E80 (push &p, push pattern), bool result ignored
        ResetDefenderAttackPattern(info, p);                 // by-value copy built in place: push ecx; mov [esp],esi; inc [esi+4]
     }  then ~p: `dec [esi+4]; jne; mov edx,[esi]; call [edx+4]`.  Note `this` reloaded from stack (mov edi,[esp+18h]) for both
     World and the Reset call.

## 3. Callees
| target | landed row | landed name | PDB name | draft spelling |
|---|---|---|---|---|
| vslot 10 (+0x28) | (CDefClassBase vtable) | `virtual CDefString GetInstantiationName() const` (engine/CDefClassBase.h:90, slot 10 0x0042DA10) | same | draft `CCombatManager::GetDefName(CCharString*)` -- WRONG class & name & return type |
| 0x009D49B0 | rebuild/src/compiled/00/9d/CDefStringTable_GetString_009d49b0.cpp | CDefStringTable::GetString(long) | same | OK, but should come from inline CDefString->CCharString conversion, global `GDefStringTable`-style name per fable_definition_table.h (draft `g_DefStringTable` local) |
| 0x009AD410 | rebuild/src/compiled/00/9a/CDefinitionManager_GetDefGlobalIndexFromName_009ad410.cpp | CDefinitionManager::GetDefGlobalIndexFromName(const CCharString&) const | same | draft non-const; reached via CWorld::PeekDefinitionManager() (const) |
| 0x0099EAE0 | rebuild/src/compiled/00/99/CCharString_CCharString_0099eae0.cpp | ~CCharString | ??1CCharString | OK |
| 0x00666D10 | rebuild/src/compiled/00/66/CThingCreatureBase_GetCombatTypeDef_00666d10.cpp | CThingCreatureBase::GetCombatTypeDef() const -> `const CCombatTypeDef&` (engine/CThingCreatureBase.h:85) | same | draft returns pointer, non-const |
| 0x006F0E80 | rebuild/src/compiled/00/6f/CDefinitionManager_GetDef006f0e80_006f0e80.cpp (landed as GetDef_006f0e80) | template `bool CDefinitionManager::GetDef<T>(long, CDefPointer<const T>*) const` (engine/CDefinitionManager.h:37; noinline body under FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY) | ??$GetDef@VCAttackPatternDef@@@CDefinitionManager@@QBE_NJPAV?$CDefPointer@$$CBVCAttackPatternDef@@@@@Z (expected) | draft `CDefPointer GetDef(long,long*)` -- WRONG: arg 2 is the out CDefPointer*, return is bool |
| 0x006E8C30 | rebuild/src/compiled/00/6e/CCombatManager_ResetDefenderAttackPattern_006e8c30.cpp | CCombatManager::ResetDefenderAttackPattern(CCombatantInfo&, CDefPointer<CAttackPatternDef>) | ?ResetDefenderAttackPattern@CCombatManager@@AAEXAAUCCombatantInfo@@V?$CDefPointer@$$CBVCAttackPatternDef@@@@@Z (private; const T) | draft arg order (def, info) REVERSED; class is CCombatManager not a member reached via Manager |
| [vtbl+4] | -- | CDefPointeeBase delete (slot 1) in ~CDefPointer | -- | inline via engine/CDefPointer.h |

## 4. Members (retail offsets; shared headers)
- `this` CCombatManager (engine/CCombatManager.h): +0x20 `CWorld& World` (private). Ego_r +0x28 (retail maps 0xC).
- CWorld (engine/CWorld.h, FABLE_RETAIL_CWORLD): +0x10 `CGameDefinitionManager* DefinitionManager`; inline accessor
  `PeekDefinitionManager() const` under opt-in FABLE_RETAIL_CWORLD_PEEKDEFINITIONMANAGER (or FABLE_RETAIL_CTCBOASTINGCROWDCONTROL).
- CCombatantInfo (engine/CCombatantInfo.h, sizeof 0x70): +0x00 Parent (CThingCreatureBase*), +0x04 AttackerInfo (0x30),
  +0x34 DefenderInfo (CDefendingCombatantInfo, engine/CDefendingCombatantInfo.h):
  DefenderInfo.Attackers (map<unsigned __int64,CThingCreatureBase*>) header +0x34 / count +0x38, node value +0x18;
  CombatWheel +0x40; LastVictimWheelUpdatePos +0x4C; LastWheelUpdateWorldFrame +0x58; PLastAttacker +0x5C;
  **PAttackPattern (CDefPointer<const CAttackPatternDef>) +0x64**; AttackPatternIndex +0x68; FrameForNextInstruction +0x6C.
  (The draft modelled +0x64 as `CombatManager` and called its vtable: WRONG -- it is PAttackPattern.Object.)
- CCombatTypeDef (engine/CCombatTypeDef.h, opt-in FABLE_RETAIL_CCOMBATTYPEDEF, sizeof 0xAC): +0x84 `std::map<ECombatCreatureType,long>
  AttackPatterns`, **+0x90 `ECombatCreatureType CombatCreatureType`** (draft called it CombatTypeNameIndex -- wrong name/type).
- CAttackPatternDef: only engine/_quarantine/CAttackPatternDef.h (non-retail); needs to be a CDefClassBase-derived complete type for
  `->GetInstantiationName()` (CDefPointer::operator-> returns const T*). IncrementDefenderAttackPattern 0x006E8600 reads AttackPattern
  vector at def+0x3C.
- Locally-modelled classes in the old draft that HAVE shared headers: CCharString (fable_string.h), CDefStringTable
  (engine/CDefStringTable.h / fable_definition_table.h), CCombatTypeDef, CThingCreatureBase, CDefPointer (engine/CDefPointer.h, needs
  FABLE_RETAIL_DEF_CLASS_BASE), CDefinitionManager, CCombatManager, CCombatantInfo, CDefendingCombatantInfo, plus hand node structs
  (use stlport/fable_map.h).
- engine/CCombatManager.h does NOT declare UpdateDefenderAttackPattern or ResetDefenderAttackPattern yet -> an opt-in must be added
  (pattern: FABLE_RETAIL_CCOMBATMANAGER_PERFORMSEQUENCERESET block, which wraps a private decl).

## 5. Visibility
- Inline in retail (expanded): CDefString->CCharString conversion and CDefString::GetString (default CDefString.h branch);
  CWorld::PeekDefinitionManager; map begin/end/find/_M_find (no FABLE_RETAIL_STLP_TREE_MFIND_DECL -- _M_find must stay inline here,
  unlike 0x006E94D0); _Rb_global::_M_increment (inline body in stlport/fable_map.h:46); CDefPointer ctor/copy/dtor (__forceinline).
- Out of line: GetCombatTypeDef 0x00666D10, GetDef<CAttackPatternDef> 0x006F0E80 (noinline template body opt-in
  FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY or just the declaration), ResetDefenderAttackPattern 0x006E8C30 (earlier same-TU
  address; retail did not inline it -- declare only), GetString 0x009D49B0, GetDefGlobalIndexFromName 0x009AD410, ~CCharString.

## 6. Siblings
- CCombatManager_DeregisterCombatant_006ea430.cpp: shared-header model, `#define FABLE_RETAIL_CCHARSTRING / CCLASS / DEF_CLASS_BASE /
  CTCBASE / CTHINGPHYSICAL / CTHINGGAMEOBJECT / CTHINGCREATUREBASE / CCOMBATMANAGER_PERFORMSEQUENCERESET`, `#include <new>`,
  engine/CThingCreatureBase.h, engine/CCombatManager.h. Best template.
- CCombatManager_ResetDefenderAttackPattern_006e8c30.cpp, CCombatManager_IncrementDefenderAttackPattern_006e8600.cpp: same class/area,
  old local models (offsets confirm +0x64 PAttackPattern etc.).
- CCombatManager_UpdateAttackingCombatantsList_006ed030.cpp (caller): local STLport model + `#pragma auto_inline(off/on)` region,
  engine/CCountedPointer.h.
- CCombatManager_CCombatManagerRebuildCombatLists_006eba60.cpp / GetCombatDebugInfo_006eac50.cpp: `// Flags: /O2 /Oy /W3` (no /EHsc).

## 7. Old-draft model errors
- Wrong class/name (`CDefendingCombatantInfo::InitializeDefenderFromCreature`); `this` is CCombatManager; `Manager` at +0x20 is
  `CWorld& World`, whose +0x10 is the DefinitionManager.
- +0x64 is info.DefenderInfo.PAttackPattern (def), slot 10 is GetInstantiationName returning CDefString; the draft's
  `*(long*)&defName` cast and explicit GetString call should be the natural `CCharString` conversion of the CDefString.
- GetDef signature wrong (out CDefPointer* param, bool return, ignored); ResetDefenderAttackPattern arg order reversed & wrong owner.
- +0x90 is CombatCreatureType (ECombatCreatureType), used as the map key; maps are std::map, not hand nodes.
- Condition order: retail `found != end() && pattern == 0` (draft equivalent, OK); keep `pattern` initialised to 0 at function entry
  before the virtual call (retail stores 0 early).
- DIFFER 379 vs 360 likely from hand-written find/increment and pointer types; use the stlport map + shared headers.

## 0x004B4260 (held back)

# DOSSIER 0x004B4260 CQuestManager::ActivateMultipleQuests (359 bytes)

## 1. Retail shape (xdis 004b4260 0x177)
- __thiscall, `ret 0Ch` (3 stack args), returns bool in al (`mov al,bl`).
- NO frame pointer (`sub esp,2Ch; push ebx/ebp/esi/edi`) -> default /Oy, no optimize("y",off).
- NO SEH prologue even though locals have destructors (CArray, CCharString temps) -> TU without EH state (same as landed
  sibling ActivateQuest 0x004B4A10, which also has a local CArray and no SEH; neither has a `// Flags:` line).
- Speed idioms (lea esp,[esp] / 7-byte nop alignment of loop heads at 0x4B429C, 0x4B4399) -> /O2, not optimize("s").
- Frame 0x2C exactly = i [esp+10] | this spill [esp+14] | CArray<CActivateQuestInfo> quests [esp+18..20] |
  CActivateQuestInfo temp A [esp+24] | temp B [esp+30]. Two distinct 12-byte temporaries (one per branch).
- Register map: ebp = &questNames (param kept in reg whole function), edi = this (spilled to [esp+14], reloaded after
  the push_back in the info!=0 branch because edi holds `info` there), bl = activated, esi = 0 constant (`push esi` =
  DisplayProgress's last bool) re-zeroed at the back-edge (0x4B429A), then reused for &questNames[i].
- KEY: the per-iteration CCharString("QuestManager: Activate Quest") temporary (string 0x01239440, verified in exe) is
  homed in the DEAD ARGUMENT SLOT of questNames ([esp+40] at body depth; `lea ecx,[esp+48]` after 2 pushes). VC does
  this only because the param lives in ebp; if ours puts the temp in the frame the frame grows.
- Loop: rotated `if (size()==0) skip; do {...} while (++i < size())` with size recomputed from [ebp]/[ebp+4] each pass
  (unsigned compare jb/jbe; `sar 2`). i is unsigned, frame-only.
- Body order per pass: CCharString(text,-1) ctor 0x99EBF0 -> DisplayProgress(temp,-1.0f,false,false) (fastcall: ecx=temp,
  dl=false, push float 0xBF800000, push esi=0) -> ~CCharString temp -> esi=&names[i] (reloaded after the calls) ->
  IsQuestRegistered(esi) -> if true: info=ScriptInfoManager(+0x78)->GetPScriptInfo(esi); `push esi` hoisted before
  `je` (both branches start with QuestName copy-ctor 0x99EC30); fill PScriptInfo (+4) = edi or 0, GameScript (+8) =
  arg2, LoadResources (+9) = arg3; vector::push_back 0x4BB720 (ecx=&quests); ~CCharString on temp (tail-merged call
  at 0x4B4358, ecx differs per branch); bl=1.
- After loop: ActivateQuests(quests) 0x4B3CE0 (ecx=edi=this); inline STLport ~vector: loop ~CCharString stride 0xC over
  [start,finish) with start kept in ebp, then `if (start) free(start)` (0xBFEA14, cdecl, add esp,4).

## 2. Identity (manifest label is RIGHT)
- Ego_r.pdb / FableWin.pdb: `?ActivateMultipleQuests@CQuestManager@@QAE_NABV?$CArray@VCCharString@@@@_N1@Z`
  = `public: bool CQuestManager::ActivateMultipleQuests(const CArray<CCharString>&, bool, bool)` (non-const).
  Sources: ghidra_out/egor_pdb_names.tsv (Ego_r addr 0x004b9b70 - NOT retail; Ego_r addresses differ), _fablewin_api.tsv,
  naming_stragglers/FableWin.exe.signatures.tsv, bsim_port_audit_final.tsv:2260 (Ego_r->retail 4b4260, sim 0.86),
  rebuild/include/engine/CQuestManager.h line ~276 (declared at 0x004B4260).
- Callers: CGameScriptInterface::ActivateMultipleQuests 0x00892EA0 (vtable slot 277, cgsi_slots_aligned.tsv) pushes
  (names, 1, 1); ...WithoutLoadingResources 0x00892EE0 (slot 279) pushes (names, 1, 0); ActivateQuest 0x004B4A10
  (landed, forwards its two bools); LoadGameState 0x004B5500 (names,false,true); CWorld::ActivateWorld 0x0049F180
  (false,true); also 0x0049EAD1, 0x004B5B84.
- PARAM MEANING: arg3 is load_resources (the WithoutLoadingResources twin passes 0) and is stored to
  CActivateQuestInfo::LoadResources (+9); arg2 is stored to GameScript (+8) (script interface passes 1, world/load pass 0).
  Suggested descriptive names: (quest_names, game_script, load_resources). PDB param names not available.
  CQuestManager.h names them (names, load_resources, from_saved_game) -> both wrong (NAME_FIXES candidate, param names only).

## 3. Callees (landed file / landed name / PDB)
| addr | landed row | landed name | PDB | draft |
|---|---|---|---|---|
| 0x0099EBF0 | 00/99/CCharString_Constructor_0099ebf0.cpp | CCharString(const char*, long) | ??0CCharString@@QAE@PBDJ@Z | ok |
| 0x009E9F40 | 00/9e/NProgressDisplay_DisplayProgress_009e9f40.cpp | NProgressDisplay::DisplayProgress | ?DisplayProgress@NProgressDisplay@@YIXABVCCharString@@M_N1@Z (__fastcall) | ok; header engine/NProgressDisplay.h declares it (FABLE_FASTCALL) |
| 0x0099EAE0 | 00/99/CCharString_CCharString_0099eae0.cpp | ~CCharString | ??1CCharString@@QAE@XZ | ok |
| 0x004B00C0 | 00/4b/CQuestManager_IsQuestRegistered_004b00c0.cpp | IsQuestRegistered | ?IsQuestRegistered@CQuestManager@@QBE_NABVCCharString@@@Z (CONST member) | draft drops `const` -> fix. NOTE landed ActivateQuest_004b4a10.cpp line 106 spells it `GetActiveQuestInfo(const CCharString*)` -> NAME_FIXES |
| 0x00CB5AD0 | 00/cb/CScriptInfoManager_GetPScriptInfo_00cb5ad0.cpp | GetPScriptInfo | ?GetPScriptInfo@CScriptInfoManager@@QAEPAVCScriptInfo@@ABVCCharString@@@Z (returns non-const CScriptInfo*) | draft `GetScriptInfo` returning const* -> WRONG name |
| 0x0099EC30 | 00/99/CCharString_CCharStringcopy_0099ec30.cpp | CCharString(const CCharString&) | ??0CCharString@@QAE@ABV0@@Z | ok |
| 0x004BB720 | 00/4b/stdvector_pushback_004bb720.cpp | std::vector<CQuestManager::CActivateQuestInfo>::push_back | ?push_back@?$vector@VCActivateQuestInfo@CQuestManager@@...QAEXABV...@Z; Ego_r also has inline ?push_back@?$CArray@VCActivateQuestInfo@CQuestManager@@@@QAEXABV...@Z (CArray::push_back forwards) | draft's CActivateList::push_back is a local stand-in |
| 0x004B3CE0 | NOT landed (manifest name CQuestManager::ActivateQuests) | - | ?ActivateQuests@CQuestManager@@AAEXABV?$CArray@VCActivateQuestInfo@CQuestManager@@@@@Z (PRIVATE, takes const CArray<CActivateQuestInfo>&), thiscall ret 4, 0x505 bytes ending 0x4B41E5 | draft takes CActivateList -> should be CArray<CActivateQuestInfo> |
| 0x00BFEA14 | 00/bf/global_free_00bfea14.cpp | free | MSVCR71 | ok |

## 4. Members / types
- this+0x78 ScriptInfoManager (CScriptInfoManager*): struct_layouts_egor.tsv names it (Ego_r +212; retail +0x78 under
  STLport sizes). In engine/CQuestManager.h it is typed+public ONLY in the generated default branch (no
  FABLE_RETAIL_CQUESTMANAGER): `CScriptInfoManager* ScriptInfoManager; // +0x78` (static_assert line 468). The
  FABLE_RETAIL_CQUESTMANAGER branch keeps +0x74..+0x7c as pad (LOADGAMESTATE: `_pad_0x74[0x7c-0x74]`) - no opt-in names it.
  Sibling idiom (LoadResourcesForQuests 0x004B2510): `#define CQuestManager CQuestManagerDonor / #include
  "engine/CQuestManager.h" / #undef CQuestManager` then `class CQuestManager : public CQuestManagerDonor { decls };`.
- CQuestManager::CActivateQuestInfo (12 bytes, struct_layouts_egor.tsv): QuestName CCharString +0, PScriptInfo
  CScriptInfo* +4, GameScript bool +8, LoadResources bool +9. PDB ctor
  `??0CActivateQuestInfo@CQuestManager@@QAE@ABVCCharString@@PBVCScriptInfo@@_N2@Z` = (const CCharString&, const
  CScriptInfo*, bool, bool) and a real dtor ??1CActivateQuestInfo@CQuestManager@@QAE@XZ (inline in retail). Not in any
  shared header (stdvector_pushback_004bb720.cpp models it locally with the PDB names). Draft member names Info/P2/P3 wrong.
- CArray<T> : public std::vector<T> in engine/CArray.h (opt-ins FABLE_RETAIL_CARRAY (operator[] via begin()),
  FABLE_RETAIL_CARRAY_PUSHBACK_NOINLINE, FABLE_RETAIL_CARRAY_PUSHBACK, ...). questNames reads [ebp]=_M_start,
  [ebp+4]=_M_finish; quests is CArray<CActivateQuestInfo> (Ego_r ??0/??1/??A/push_back of that CArray).
- Classes the draft models LOCALLY that have shared headers: CQuestManager (engine/CQuestManager.h), CArray
  (engine/CArray.h), NProgressDisplay (engine/NProgressDisplay.h), CScriptInfo (engine/CScriptInfo.h), CCharString
  (fable_string.h, pulled by CQuestManager.h). No header: CScriptInfoManager, CActivateQuestInfo.

## 5. Callee-body visibility
- Same TU in retail (CQuestManager.cpp neighbourhood 0x004B00C0..0x004B4A10): IsQuestRegistered 0x004B00C0 and
  ActivateQuests 0x004B3CE0 are defined EARLIER (visible to VC7.1 when compiling this function); ActivateQuest
  0x004B4A10 later. Landed ActivateQuest and LoadGameState both needed a same-TU `auto_inline(off)` stand-in for THIS
  function so VC knew the array address does not escape -> by symmetry a reads-only stand-in body for ActivateQuests
  (and maybe IsQuestRegistered) before this function may be what gets edi/ebp/bl as retail (old notes did not try it).
- vector<CActivateQuestInfo>::push_back 0x004BB720 is out of line (COMDAT, aligned 0x10) - retail called it out of line
  from both branches; old notes tried a noinline body. CArray.h FABLE_RETAIL_CARRAY_PUSHBACK_NOINLINE gives the
  visible-but-out-of-line wrapper idiom.
- ~vector is fully inline (_Destroy loop + free); stlport/fable_containers.h _Destroy opt-ins
  (FABLE_RETAIL_STLP_DESTROY_AUX_NOINLINE etc.) not wanted here - retail has the loop inline.
- CActivateQuestInfo ctor/dtor, CArray ctor/dtor/operator[]/size inline.

## 6. Sibling landed rows (00/4b/)
- CQuestManager_ActivateQuest_004b4a10.cpp: no Flags; local models; `#pragma auto_inline(off)` same-TU stand-ins for
  _M_insert_overflow and ActivateMultipleQuests.
- CQuestManager_LoadResourcesForQuests_004b2510.cpp: CQuestManagerDonor include trick on engine/CQuestManager.h default
  branch (public ScriptInfoManager/World/DayActive), same-TU noinline stand-in for set::insert, index loop
  `for (unsigned int i = 0; i < v.size(); ++i) { const CCharString& s = v[i]; ... }`.
- CQuestManager_LoadGameState_004b5500.cpp: FABLE_RETAIL_CQUESTMANAGER(+LOADGAMESTATE) branch; noinline stand-in of
  this function.
- CQuestManager_DeactivateQuest_004b43d0.cpp (next row, includes fable_compiler.h, engine/CCountedPointer.h),
  NQuestManagerCRuntimeQuestPersistence_ClearAll_004b41f0.cpp (previous row), CQuestManager_Update_004b4490.cpp
  (#define FABLE_RETAIL_CCHARSTRING). None carry `// Flags:`.

## 7. Old draft model errors
- Names: GetScriptInfo -> GetPScriptInfo (non-const CScriptInfo* return); IsQuestRegistered missing const; members
  Info/P2/P3 -> PScriptInfo/GameScript/LoadResources; CActivateList -> CArray<CActivateQuestInfo> (vector-derived);
  ActivateQuests is private and takes const CArray<CActivateQuestInfo>&; param names p2/p3 -> game_script/load_resources.
- Local structs instead of shared headers (CQuestManager with _pad00[0x78], CArray, CCharString, NProgressDisplay).
- Control flow matches retail (two temporaries, info/null branches, activated=true after both). Residue (336 v 359)
  is register allocation: retail keeps &questNames in ebp with the progress-string temp in its dead arg slot, `this` in
  edi (spilled), activated in bl, i frame-only. Ours put i in edi / this in ebp / activated in frame. Untried levers:
  same-TU visible body for ActivateQuests (and IsQuestRegistered) defined before this function; real
  CActivateQuestInfo ctor (PDB signature, const CScriptInfo*) and dtor; CArray from the shared header.

