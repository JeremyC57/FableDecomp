# Chunk 341 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- A class model that declares virtuals must NOT also carry an explicit `void* __vftable` member. VC adds its own vptr and every member shifts by 4 (`[esi+5Ch]` for `[esi+58h]`). Put the comment `// +0x00 vftable` instead (0x004C01E0, 0x009C82C0).
- An inline `CatchMessage(const CMessageX&) { SetAsChanged(8); }` called from HandleMessage folds the reason constant into `test byte ptr [esi+58h],8` / `or al,8`. The out-of-line CatchMessage keeps it in a register (0x004C01E0).
- An if-chain of `return CCharString(literal, -1);` arms has VC hoist the common `push -1` above the first branch (0x009C79E0).
- In a ctor, split zeroing to place the zero register: members zeroed by inline member ctors + one word set in the BODY puts `xor edi,edi` between the base and derived vtable stores. With all members in inline ctors the xor comes after both stores; with all of them in the body the stores sink below the next call's `push` (0x009C85A0).
- A Dinkumware `erase(begin(), end())` written as an inline `clear() { erase(First, Last); }` (`kept = _Move(last, Last, first, tag(), false); _Destroy(kept, Last, alloc()); Last = kept;`) keeps the member loads below the `push 0`. Explicit `last`/`first` locals hoist them above it (0x009C8660).
- An inlined scalar-pointer move on `(Last, Last, First)` keeps the dead `cmp ecx,ecx ; je` only when the operands are the members read in place; one shared local folds the test away (0x009C8660).
- The CMeshDataBank::NotifyBankSize model transfers to every bank: under `if (ReadOnly)`, `Entries.resize(n, 0)` (the zero homed in the frame), then an index loop over {type, count} that is strength-reduced to a pointer + `dec eax ; jne` (0x009C82C0).
- STLport `_List_base(alloc) : _M_node(alloc, 0) { node = malloc(12); node->next = node->prev = node; _M_node.m_data = node; }` reproduces the zero store before the malloc and the self-link (CClassFactory ctor 0x00A373B0, first try). A counter cleared in the body before a call is stored after the call's argument push.
- Body scalar stores in a ctor follow source order across the scheduler's interleaving. Moving one assignment moves its store (CEngineScene ctor 0x00B94A80). `1.0f / arg` is `fld [1.0] ; fdiv [arg]`, kept on the x87 stack until the last store.
- `switch (WaitForSingleObject(h, INFINITE)) { case 0: ..; case WAIT_FAILED: ..; default: .. } CloseHandle(h); return hr >= 0 ? 0 : hr;` lays out default first, WAIT_FAILED second and case 0 last. VC7.1 duplicates the CloseHandle + `setge ; dec ; and` tail into each arm; `return LastErrorToHResult();` before any locals is a tail `jmp` (0x00CAC400).
- A counted-pointer member read through an inline `operator->` (`struct { T* Data; void* Info; T* operator->() const; }`) schedules differently from a raw pointer member (0x00B1B0C0).
- Inline `bool IsDead() const { return Dead; }` over an `unsigned short` bit-field gives `mov al,[..] ; shr al,5 ; test al,1` (0x00B0C830).
- A const out-param `GetConstInterface(&p, type)` gives `mov eax,[eax+4] ; lea esi,[eax+0Ch]` for a member of the result (0x008D9410).
- `X = Y = Z = 0.0f` stores Z first (0x00B0A6D0). One shared float local for two stream reads saves the second frame slot (0x008156A0). A memberwise user copy ctor on C3DVector gives `return pos;` the edx copy path (0x00814BC0).

Review at landing:
- 004bfc50 is parked (component load in ecx vs eax) after 4 grades.
- Local models next to existing engine headers:
  - 00a373b0 does not use engine/CClassFactory.h, because that header has the PDB's 16-byte vector and 12-byte list (retail is 12 and 4, so MaxClassTypeID sits at +0x10, not +0x14).
  - 009c85a0 models CResourceList / CResourceBank locally (engine/CResourceList.h holds the vptr as a plain word, as the landed CResourceBank ctor 0x009FC5F0 notes).
- 009c85a0: the retail type at CDataBank+0x210 (the PDB NarratorMap) is uncertain. The ctor writes only one dword there, in the body. It is modelled as a header word, so check it against the dtor/AddNarrator when those land.
- 00cac400: the class name CAudioThreadOwner_00cab870 is descriptive and follows the landed 0x00CAB870. The library behind it (DirectShow-style event/attach, HRESULTs 0x80040293/4) is not identified. The ALREADY-LANDED callee 0x00CAB870 carries the wrong manifest label NHitLocations::CPhysicalPrimitiveCylinder::TransferBinaryOutAsync.
- 00814d90: CSphere is declared in the global namespace, as in the landed twin 0x00814D10 (the real class is NBBBMaths::CSphere); the call symbol differs from the PDB spelling.
- Test-only scaffolding, kept out of the src: a default ctor added to CCharString (008d75d0) and to C3DVector (00b0a6d0, 00b0a710) so the objects can be built; the 00cac400 test uses `int` for HRESULT (LP64 g++ host) and renames the Win32 entry points (kernel32.lib collision under VC).
