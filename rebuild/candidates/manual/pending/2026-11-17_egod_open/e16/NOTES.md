# e16 notes (egod lane, IDENTIFY batch)

## 0x009C37A0 CFileInstaller::OnFileReadCompletion -- RELOCATION_MATCH (1 grade), test PASS, vland RELOCATION_MATCH
- Identity (proven, two sources): Ego_d lib_file_installer.obj `CFileInstaller::OnFileReadCompletion` (params err / no_bytes / pover,
  locals cs / pread); Ego_r.pdb `?OnFileReadCompletion@CFileInstaller@@KGXKKPAU_OVERLAPPED@@@Z` at donor 0x00B11C60 between
  StartWritingFile 0x00B11B20 and OnFileWriteCompletion 0x00B11CF0 (retail 0x009C3660 / 0x009C37A0 / 0x009C3820), FableWin.pdb same symbol.
- What matched: the shared engine/CFileInstaller.h (already carries the volatile cross-thread flags: ExitThread volatile keeps
  0x009C37D3's discarded read), opt-ins FABLE_RETAIL_CCRITICALSECTIONLOCK + FABLE_RETAIL_CTBASESINGLETON_GET_DECL (Get is the
  0x00404440 COMDAT call), SetDirtyDiscError(bool on) DEFINED in the TU before the row (Ego_d local `on`, `cs`) so it expands inline,
  Ego_d names. The old draft's DIFFER(125v128) was a local model without volatile. No flags, no header changes.
- Name fix: reloc_bindings.tsv binds 009c37a0 as `?OnFileReadCompletion@CFileInstaller@@SGXKKPAUOVERLAPPED_Fable@@@Z` (caller
  0x009C3AE0 ProcessActiveFileForRead): wrong access (S vs K) and struct tag.

## 0x009C3820 CFileInstaller::OnFileWriteCompletion -- RELOCATION_MATCH (1 grade), test PASS
- Identity (proven, two sources): Ego_d lib_file_installer.obj `CFileInstaller::OnFileWriteCompletion` (err / no_bytes / pover;
  cs / pwrite); Ego_r.pdb `?OnFileWriteCompletion@CFileInstaller@@KGXKKPAU_OVERLAPPED@@@Z` donor 0x00B11CF0 right after
  OnFileReadCompletion 0x00B11C60 (retail 0x009C37A0 -> 0x009C3820), FableWin.pdb same symbol.
- Same recipe as 0x009C37A0 (shared header + the two opt-ins + SetDirtyDiscError defined in the TU). Done out of brief order
  (right after its twin) because it is the same TU and recipe.

## 0x00C62370 GetRandomPointInSphere (DESCRIPTIVE) -- RELOCATION_MATCH (2 grades), test PASS
- Identity: libpk ambient-sound code, NOT combat_sequence_isvalid_predicates.obj (that object came from the propagated label
  NBalverineCombat::SetLungedThisFrame on the rand()*1/32767 helper 0x00C62350). No PDB / RTTI source has libpk names, so the name
  stays descriptive; it is the spelling the landed caller CAmbientTrigger::Play 0x00C62530 already binds in reloc_bindings.tsv
  (one address, one name). The catalog's `Quaternion_FromAxisAngle` is an autoname: the body is a random point in a sphere.
- What matched: the row is a same-TU `static __declspec(noinline) __stdcall` (retail's private convention: hidden result in esi) and
  needs its real caller in the TU, so src/00c62370.cpp is the landed 0x00C62530 TU text verbatim with this row's header lines.
  The old draft's DIFFER(124v118) used an invented `_caller` function and different local types (double phi).
- Coordinator: 0x00C62350 is still spelled `SetLungedThisFrame` (propagated NBalverineCombat label on a libpk rand helper) in the
  landed caller and so in this row; a descriptive respell (e.g. a libpk random-unit name) belongs to the naming queue, not here.

## 0x00749620 std::vector<std::pair<CIntelligentPointer<const CThing>,NStealth::CDangerStatus> >::push_back -- RELOCATION_MATCH (6 grades), test PASS
- Identity (proven): Ego_d tc_stealth.obj lists push_back of exactly this vector; Ego_r.pdb names push_back of the same vector
  (donor 0x0075B740, Dinkumware `$$Q` rvalue form; retail is STLport so the leaf takes `const value_type&`); the caller
  CTCStealth::SetAsInDanger 0x00747B80 pushes onto CTCStealth::DangerThings; the inline copy stores the
  CIntelligentPointer<const CThing> vtable 0x01237F1C. The catalog's "CTargetPoint_Vector_PushBack" was wrong about the element.
- What matched: an explicit instantiation over stlport/fable_containers.h through the shared engine/CTCStealth.h (include set
  of the landed 0x00691D70 row: DEF_CLASS_BASE, CCHARSTRING, CTCBASE, CTHING, CINTELLIGENTPOINTER, CTHINGPHYSICAL,
  CTHINGGAMEOBJECT, CTHINGCREATUREBASE) plus FABLE_RETAIL_CINTELLIGENTPOINTER_COPY + _COPY_ORIGINAL_CHAIN (existing opt-ins:
  copy ctor `SetPointer(item.GetPointer())`). With the plain COPY body (`SetPItem(other.GetPItem())`) it is the old draft's
  DIFFER(98v105): this/__x swap ebx/edi and `add [edi+4],10h` instead of load/add/store. No header changes, no flags.
- vland: non-blocking `WARN STANDIN_LEAF ... names another function` (the catalog still names the address
  CTargetPoint_Vector_PushBack); the leaf is the STLport spelling, the coordinator applies the label.

## 0x0099F7A0 operator+(const CCharString& string, char ch) -- OPEN, best DIFFER(130v130) (differ/0099f7a0.cpp), 17 grades
- Identity (proven): Ego_d lib_char_string.obj `operator+` with params `const CCharString& string` (0x2318), `char ch` (0x70) and
  the one local `CCharString result_string` (debug length 124; its mirror (char ch, string) is 132); Ego_r.pdb / FableWin.pdb
  `??H@YI?AVCCharString@@ABV0@D@Z`, the last of the five CCharString operator+ overloads right after RemoveWhiteSpaceBeginEnd
  (retail neighbours RemoveWhiteSpaceBeginEnd 0x0099F560, GFIntToCharString 0x0099F830). Catalog `CWideString_AppendChar` is wrong
  (narrow string, NoStrings 0x013BD800, UnassignString 0x0099E9B0, operator+=(char) 0x0099F180).
- Retail shape: two inline copies. The return copy (via the copy ctor) is `mov [esi],eax ; inc dword ptr [eax+0Dh]` (store then
  RMW, the out-of-line copy ctor 0x0099EC30 / operator= 0x0099EFB0 form). The FIRST copy is load / inc / store-pointer / store
  (0x0099F7BF `mov eax,[edx+0Dh] ; inc eax ; mov [esp+4],edx ; mov [edx+0Dh],eax`) with string.PStringData loaded INTO edx.
- Tried (shared fable_string.h, FABLE_RETAIL_CCHARSTRING + _CCHARSTRINGDATA_FRIEND + _CCHARSTRING_APPEND_CHAR, copy ctor / dtor /
  default ctor / operator= defined in the TU): `result_string(string)` copy ctor; `result_string; result_string = string;`; copy ctor
  as `*this = string`; operator= bodies: store+prefix/postfix/`+= 1`, re-read string.PStringData, inc-then-store (132), inline
  CCharStringData::IncRef (new opt-in FABLE_RETAIL_CCHARSTRINGDATA_INCREF, FableWin ?IncRef@CCharStringData@@QAEXXZ) -- all
  126v130 with `inc dword ptr [edx+0Dh]`. Only the old draft's `long refs = string.PStringData->NoRefs + 1;` operator= text gives
  retail's instruction shape (130v130), but with the CSE in ecx (retail edx) and a local Ego_d does not list.
- Next (pass 2): the TU's other bodies (operator+=(char), MakeOwnCopyOfData, Append) defined before the row for c2 numbering;
  c2prio on the first-copy temp; Ego_d SetString(const CCharString&) (debug 412, no locals) as the inlined operator= body.
- The IncRef opt-in is in hdr/ but no row needs it now (see hdr/README.md).

## 0x009F18F0 CGameEventDataPackage::GetData -- OPEN, best DIFFER(166v166) (differ/009f18f0.cpp), 14 grades
- Identity (proven): Ego_d lib_game_event.obj order InitFromCompressedBuffer (0x009F1870) -> GetData -> CGameEventPackageSet::
  CompressIntoBuffer (0x009F19A0); Ego_r.pdb the same three in a row (donor 0x00B45490 / 0x00B454F0 / 0x00B45570); FableWin.pdb
  0x030A2200 (debug size 0xA30 = 2608 bytes; Ego_d 2081: the source carries many debug-only statements, most likely asserts).
- Ego_d locals: params no_bytes (long), area (void*); bytes_to_read (long), byte_pointer (unsigned char*), read_pos (unsigned long)
  at function scope; overflow_check (__int64) in the one block. Retail: unsigned wrap test, (read_pos+no_bytes) & 31, the 64-bit
  range check of read_pos + bytes_to_read zero-extended (early return), two rep-movs copies, StartPos / Length updates.
- Residue (unchanged from the 2026-11-10 draft): pure register permutation, retail this ebx / read_pos edx / no_bytes ebp, ours
  no_bytes ebx / this edx / read_pos esi (earlier c2prio: read_pos 61 < this 75).
- Tried (all inert, 166v166 same hunks): Ego_d declaration orders (read_pos first / bytes_to_read first), declare-then-assign,
  byte_pointer = area vs &Data[read_pos], byte_pointer += bytes_to_read walk, overflow operand order, operand order of the wrap test,
  a bytes_to_read = no_bytes copy, dead-arm FABLE_RETAIL_ASSERT sets (1 / 3 / 3 asserts). Not tried: c2prio measurement of the
  priority inputs, the TU's other bodies (CGameEvent::GetDataPackage which fills it) defined before the row.
- Pass-1b (advisor): the landed wide twin 0x0099C120 operator+(const CWideString&, wchar_t) (local model, /Gr) has the SAME
  split first copy (0x0099C148 `mov eax,[edx+0Ch] ; inc eax ; mov [esp+8],edx ; mov [edx+0Ch],eax`) and a store+RMW return copy
  (0x0099C195) from ONE copy-ctor text (`CWideStringData* data = other.PStringData; if (data) { PStringData = data; ++data->NoRefs; }`
  plus a same-block test), so the two shapes are context, not two source functions. Ported to CCharString (shared header and
  the draft's local model, packed and unpacked, /Gr and not, `data` local, same-block test 135v130, probes with same-TU noinline
  UnassignString): all 126v130 RMW. Difference left vs the twin: retail CCharString bumps NoStrings with an RMW
  `inc dword ptr [013BD800h]` first while the twin splits its counter too. The IncRef overlay was removed (no row needs it).
- Pass-1b measurement (hand892 c2prio_op.py on the best text): colouring order bytes_to_read 85 (eax), this 75 (edx), read_pos 61
  (esi), temps 56 / 44, area 28 (ebp), no_bytes 25 (split, ebx). Retail needs read_pos coloured before this (read_pos -> edx,
  then this -> ebx; no_bytes -> ebp), i.e. prio(read_pos) > 75 or prio(this) < 61. Reassigning read_pos for the StartPos stores
  (3 variants) leaves every number unchanged (c2 forwards the copies). Retail has the same member accesses as ours (this refs 9),
  so the missing priority must come from read_pos uses or live-range weights in the real (assert-heavy, debug 2608 bytes) source.
