# Chunk 587 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- A same-TU `static __declspec(noinline)` helper whose retail copy ends `ret N` (LTCG callee-pop) needs `__stdcall` on the static. It keeps the private register convention and gives the `ret N` (0x00C9C2F0 mdct_butterfly_generic: plain static 267v269, `__stdcall` MATCH).
- Which register carries the first parameter of a same-TU static is chosen by the compiler from the callee's body, not the caller. A retail `mov eax,esi` before each call (vf passed in eax) cannot be forced from the caller side (0x00C89D70 _get_prev_page: lane gap).
- A head object at offset 0 of a part with NO vptr store of the part's own is a member, not a base. A base makes VC emit the derived vptr store (0x00C39130: heads as bases 301v265, as members RELOCATION_MATCH).
- The live-slot tag rule (a `__false_type` tag homed on a live argument slot needs a same-TU noinline body for the tag-taking callee) applies to STLport `_M_fill_insert` -> `__uninitialized_copy` (0x00AE82C0).
- Retail's queue test `je ; je ; cmp ; je` is `cap != 0 && !(cap != 0 && cnt == 0)`: an inline IsEmpty helper that rechecks the capacity, called under an outer capacity test (0x00C43A10).
- SGI `__merge_without_buffer`: the first recursive call is real and the second becomes a loop. lower_bound/upper_bound/rotate are inline front ends that pass the `(Distance*)0` / `(T*)0` tag pointers to the out-of-line `__lower_bound` / `__upper_bound` / `__rotate` (0x00B86140).
- A CEnumPins-style helper (Refresh) defined out of line ABOVE its same-TU caller gets inlined into the caller as in retail (0x00CAAFC0).

Review at landing:
- 0x00CAAFC0: vland warns SHARED_HEADER_AVAILABLE CBaseObject. That is the strmbase CBaseObject (a different class from engine/CBaseObject.h with the same spelling); both spellings `?...@CBaseObject@@` collide in the name set.
- 0x00C39130: the 0xC39BA0..0xC39CE0 part dtors carry two spellings each. One is CPair_<addr> from their own landed rows; the other is the CReceptorBase_00c38b20 / CPart*_00c38b20 names bound by dtor 0xC38B20. One address needs one name, so pick one and respell the other.
- 0x00C39130: the landed dtor 0xC38B20 model splits the +0xCC part into 0xC + 0x20, and the landed head models declare virtual dtors. The retail heads are trivially destructible, so re-check those landed sources.
- 0x00AE82C0 family: resize 0xAE8540 binds `?insert@` for 0xAE82C0; 0xAE3330 binds `?Reserve@CVector16@@` for 0xAE80F0; 0xAE8490 binds `?push_back@CArrayGroup2@@` for 0xAE83D0. The vector<C3DVertexBlend2> copy ctor 0xAE5D80 has three spellings across callers. See NAME_FIXES.tsv.
- 0x00AC4070 is landed as `?__EH_epilog3@CClass_00ac4070@@QAEXJ@Z`; it is C3DGroup2::SetBoneIndex(long).
- 0x00C432F0 is landed as `CMarkedBufferOwner_00c43810::ReportMark`; this chunk calls it CAudioObject::ReportQueueInfo (descriptive). Pick one.
- No row needed more than 4 grades. Local models stand next to an existing engine header only for 0x00AE3920 (layout-rename include of engine/CParticleProgram.h).
