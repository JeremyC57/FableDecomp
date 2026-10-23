# Chunk 301 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- A TC class with an `unsigned __int64` member must get vptr/Thing from a CTCBase base class; a flat struct with a vptr pads the vptr to 8 and shifts every member by +4/+8.
- Derived CThing subclasses start at sizeof(CThing)=0x94; compute member pads from there, not from the PDB offset of the first named member.
- CRGBColour ctor written as a body `{ R=r; G=g; B=b; A=a; }` (not a mem-init list) reproduces retail's right-to-left `_ftol` channel order.
- Read vector elements directly (`def->V._M_start[i]`) instead of via a named pointer local -- the local costs ebp and shifts the whole allocation.
- Cache a loop's end pointer in a local to match retail's hoisted compare.
- `IsSaving()` as an inline (`Mode==1||Mode==3`) gives the save-branch-first block order in OnSerialise.
- `Transfer(name, T&, const T&)` with literal defaults parks the default temporaries in dead parameter slots like retail.
- CDefPointer dtor: `if (Object && --Object->RefCount==0) Object->Destroy();` gives compact `dec [reg+4]`; an inline Release() member gives load/dec/store. Pick per call site.
- An out-of-line `_Find` with sret in ecx models as a member `Find(first,last,value)` on a block-scoped iterator local.
- `__forceinline` CDefPointer ctor/dtor keeps them from being emitted out of line in a /O2 TU.
- OnSerialise rows are the hardest class here: all four landed near-misses (2-34 bytes) are pure regalloc/slot-reuse.
- A same-TU `#pragma auto_inline(off)` stand-in for GetSubDef (and for __fastcall callees that take a local vector's address) reproduces retail's link-time non-escape knowledge: def pointers stay cached and `_M_finish` stays in a register.
- A CDefTempPointer dtor `if (Object) { Object->ReleaseTemporary(); Object = 0; }` gives the release-then-null-store pair for GetDef temporaries.
- Destruction order reveals declaration order: declare the def before the sub-def even when the sub-def's zero store comes first.
- `distSq < D*D` gives `fld D; fld st0; fmul; fld distSq; fcompp; test ah,5; jp`.
- A __forceinline lookup wrapper taking `const CCharString&` constructs the string temporary before the argument pushes.
- STLport sort internals model as inlines: `__lg` as `for(k=0;n!=1;n>>=1)++k` on `last-first`; final_insertion_sort calls out-of-line __fastcall Insertion/UnguardedInsertion sorts; an uninitialised empty comparator local is pushed from a stack slot like retail.
- GetTC-style `Find(long type)`: copy `type` into a local key for LowerBound and compare against the original; `IsAlive(const CBaseIntelligentPointer&)` by reference CSEs the address like retail.
- `fmod(float, double)` needs an explicit `(double)` cast (C2666 otherwise).
- Index-vs-iterator register roles (ebp/ebx) in list loops did not respond to declaration-order swaps (007c3fb0).
- A class with a virtual function and an `unsigned __int64` member pads the vfptr to 8 bytes in VC7.1 (UID lands at +0x20, not +0x18); model such engine classes with a plain `void* _vfptr` word.
- A static member called through an object (`GetWorld()->GetFrame()`) still evaluates the object call; hoist it into a named local before loading other operands to match retail's call/load order.
- A reference local to a list's node pointer (`CFollowerNode*& head = Followers._M_node`) reproduces retail's cached `lea edi,[list]`; a loop written with begin()/end() instead reloads the head every pass.
- Named locals for a value read before an argument list (`CThing* follower = p.Get(); CThing* thing = Thing;`) move those reads ahead of the pushes like retail.
- A two-argument `Transfer(name, CCharString&)` overload that builds the empty default inside puts the default after the key expression, as retail does.

Review at landing:
- Label corrections for 0055ea80 / 00561dc0 / 00564570 (see NOTES.md).
- 0055e850, 0055ea80, 0055fc90 need /GS.
- Every row has at least a first-pass probe; parked rows carry tried/next notes in NOTES.md.
