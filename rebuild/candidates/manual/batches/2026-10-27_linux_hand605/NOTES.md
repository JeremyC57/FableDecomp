# Hand chunk 605 (CPersistContext_TransferableVector .. CComponent): 1 of 10 byte-exact in src/ (+1 byte-exact in name_review/)

This chunk was 10 PARKED same-length rows from the 2026-10-30/31 retry sweep. Every row was reproduced from its best attempt
first (all ten grades reproduced exactly), then changed one thing per grade.

## What matched (1 + 1 in name_review)
- **0x0057F70F CPersistContext_TransferableVector<long>::TransferOut** (RELOCATION_MATCH, test PASS g++ + VC7.1). The row was
  a non-template stand-in with an invented `CRetailVector`; vtable 0x0124F33C's RTTI type descriptor is
  `.?AV?$CPersistContext_TransferableVector@J@@`, so the row is the `<long>` instantiation (the manifest's
  `<ECreatureGeneratorGenerateType>` is propagated). Rewritten on the landed `<int>` twin 0x00566360 model
  (`CPersistTraits<T> traits; traits.TransferOut(context, out, (*TransferableVector)[index]);`). For T = long the traits
  body must copy the element (retail homes a copy in the dead index slot): `long temp = value; SLONG::TransferOut(.., temp)`
  inside `CPersistTraits<long>::TransferOut(..., const long&)` (Ego_r.pdb spelling, emitted as an unreferenced COMDAT with
  exactly that name). The size TU (optimize "s" + frame pointers) will not inline that helper by itself; retail did, so it is
  `__forceinline` -- and that is also what schedules the index load ahead of the m_pFirst dereference, the residue all five
  earlier levers left.
- **0x00C88CD0 CMixKernelsSSE::Add** (MATCH, test PASS g++ + VC7.1) -- in **name_review/**, not src/: the six landed
  CMixKernelsSSE siblings' fvec.h `F32vec4` model with `*d++ += *s++;` matched on the first grade (the parked raw
  `_mm_load_ps` form put the count straight into esi). The name is the repo's existing one-name for the address (vtable
  0x012BF1F4 slot 5, all six landed siblings declare `virtual void Add(float*, const float*, unsigned long); // 0x00C88CD0`)
  but it is DESCRIPTIVE (libpk: no PDB, no RTTI), so it cannot be proven by two sources; the coordinator may move it to src/
  under the same rule the landed siblings were landed with. Source + test: name_review/00c88cd0.cpp, 00c88cd0.test.cpp.

## Parked for Fable 5.1 (8)
- 0x0070C4E0 CAnimateThing::GetFirstCombinationAnimFromType DIFFER(81v81): return-0 block `pop edi ; xor eax,eax` vs retail `xor eax,eax ; pop edi` | tried: result variable, for/break loops, shared `return &*it`, inverted last test, STLport __find_if tag dispatch, /Oa /Ow /G5 /G7 /Op /Ob1 | next: lane gap: epilogue scheduling of the shared return-0 block
- 0x00754D80 std::for_each<pair<float,const CAnimationEntry*>*, (anon)::CGetBestTurningAnim> DIFFER(92v92, 9 lines, was 17): m_bestAngle loaded/stored every iteration (`fld [esp+14h]` before the test, `fstp [esp+14h]` after `add edx,8`), compare reloads memory; retail hoists the load / sinks the store and compares with `fld st(1)` | tried: 11 operator() forms (named angle, best local, const best, casts, reversed compare, assignment order), /Oa /Ow /Op /G7 | next: a form that lets VC7.1 promote the scalar-replaced float field across the loop (the int field m_pBest is promoted to esi)
- 0x00C5E2C0 CWound_00c5eca0::WriteBuffer DIFFER(102v102): DeviceIoControl temps ecx/edx + handle in edx before the size push (retail) vs edx/ecx + handle in eax after the block push | tried: inline CKsObject::WriteData wrapper, decl order, pointer arithmetic, 5 flags/pragma | next: lane gap: register colouring
- 0x00C75F80 CCombFilter::Allocate DIFFER(111v111): memset count (`mov ecx,esi ; shl ecx,2 ; mov ebx,ecx`) scheduled before the m_pos store + m_data reload | tried: Init helper, operand order, block local, m_pos reset inside Clear, shift form, /Oa /G7 | next: lane gap: scheduling
- 0x00C52BB0 CCpuInfo::GetAMDModelName DIFFER(140v140): K5 test `cmp eax,4 ; jae` vs `cmp eax,3 ; ja` (every cmp-3 spelling moves Model to ecx) | tried: 9 spellings (casts, 3u, inverted if, switch range, nested switch, range forms) | next: lane gap: compare canonicalisation tied to register choice (same family as 0x00C52C40)
- 0x00A852B0 CVectorMap<unsigned __int64, list<C2DLineF> >::Insert DIFFER(143v143): sentinel store `temp.second._M_node._M_data = n` not sunk past the x.begin() load | tried: STLport direct base-ctor form, store-first, decl-only _M_insert_dispatch, member-template insert, named tag | next: lane gap: store sinking past an aliasing load
- 0x006AFE60 CTCPhysicsBase::RespondToCollision DIFFER(145v145): velocity pointer load after the first fld; Z dot term n.Z*v.Z vs retail v.Z*n.Z | tried: PDB respell (protected const references, engine header), 8 dot spellings / DotProduct orders (all identical) | next: lane gap: x87 operand canonicalisation
- 0x0052EA30 NUISystem::CComponent::ChangePositionDelta DIFFER(181v181): by-value arg copy loads d.Y after `sub esp,8`; result copy stores d.X before the push | tried: PDB respell of every name, copy-ctor bodies, pack(1), extra ctor, named converted local | next: lane gap: copy scheduling

## Skipped
- none.

## Names and evidence
- Proven (two sources): 0x0057F70F (RTTI + Ego_r.pdb), 0x009E2560 CPersistTraitsSLONG::TransferOut (PDB + call shape),
  0x0070C4E0 (PDB + sibling), 0x006AFE60 (PDB + shape), 0x0052EA30 and its callees CManager::GetInstance (RTTI of the
  singleton's vtable + PDB), CManager::GetUIScale (PDB + call), CUIScreenUnitConverter::UnConvertVector (FableWin.pdb + inverse
  of the bound ConvertVector), ChangePosition / IsUsingRelativePosition (vtable slots + PDB), 0x00754D80 (PDB name sets: the
  functor is in the anonymous namespace `?A0x247890bc`; STLport for_each).
- Descriptive (no PDB/RTTI, libpk or no evidence): 0x00C88CD0 CMixKernelsSSE::Add, 0x00C75F80 CCombFilter::Allocate (landed
  binding kept), 0x00C52BB0 CCpuInfo::GetAMDModelName (landed sibling class), 0x00C5E2C0 CWound_00c5eca0::WriteBuffer
  (address-suffixed placeholder class kept so the class has one spelling; would go to name_review if it matched),
  0x00A852B0 CVectorMap::Insert (the parked header cites an Ego_r.pdb spelling; not re-proven this chunk).

## Label corrections
- 0x0057F70F / 0x0057F6E9: vtable 0x0124F33C is CPersistContext_TransferableVector<long>, not <ECreatureGeneratorGenerateType> /
  <CHeroLogBook::CHeroLogBookEntry::ECategory>.
- 0x00C88CD0: `_Copy_impl<std::pair<float,CSubtitledMessage::CCameraTag>*,...>` is propagated; it is the SSE kernel Add.
- 0x0052E530: manifest `ConvertX` (global float) is CUIScreenUnitConverter::UnConvertVector; 0x0041E5F2 is CManager::GetInstance
  (not CFrontEndManager); 0x0052E9C0 ChangePosition is virtual. All in NAME_FIXES.tsv.
- 0x00754D80: catalog `_For_each<...>` is the Dinkumware (Ego_r) spelling; retail is STLport std::for_each.

## Header debt
- 0x006AFE60 uses engine/CTCPhysicsBase.h through the layout-rename include (`#define CTCPhysicsBase CTCPhysicsBase_Layout`) and
  derives the real class to declare the protected const member; PhysicsDef_Object is typed CDefPointeeBase* in the header, so the
  body casts it to a local CPhysicsDef model (Friction +0x2C, Elasticity +0x30; no CPhysicsDef header).
- 0x0052EA30: no CComponent header exists; the vtable is modelled with padding slot declarations except slots 31/32/117.
- 0x0070C4E0: no CAnimateThing header exists (local model from Ego_r.pdb with STLport list sizes).
