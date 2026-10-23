# Chunk 304 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- A class with a `double` member and a virtual function gets 8 bytes for the vfptr under VC7.1, which shifts every member by 4. Model retail's 4-byte vptr as a plain `void* vftable` member (005e4430 CTCQuestCompletionUI, LastRenderTime +0x48).
- Def-pointer release written as a `__forceinline void Release() { if (--RefCount == 0) DeleteThis(); }` member on the def base gives retail's `dec [reg+4] ; jne`. An inline `--P->RefCount == 0` in the template re-compared memory against a zero register (005e4430: 225 to 139 difflines).
- Clearing a CDefPointer to null in retail stores 0 twice, once inside the release branch and once after it. Write `if (P) { P->Release(); P = 0; } P = 0;`; an operator= with a P != o.P check merges the two stores.
- A same-TU noinline GetDef stand-in fixed the def-local zero-store placement (after the pushes) and the frame (005e4430: 139 to 42 difflines). Copying a CDefPointer member from another object is `PCardDef = CDefPointer<T>(card->PDef)` (copy temp + operator= + temp dtor).
- STLport list erase: write `next = it->next; prev = it->prev; prev->next = next; next->prev = prev;` to keep retail's load order.
- `CreateComponent("NAME", false)` with an implicit const char* to `const CCharString&` builds the temporary before the `false` push, as retail does (the explicit temporary interleaves).
- `test ah,5 ; jnp` around an x87 compare is `!(a > b)`; `a <= b` gives `test ah,1 ; jne` and `a < b` gives `test ah,0x41 ; jne` (00726ce0).
- Copying a member C3DVector into a local twice makes VC7.1 CSE `lea edi,&member`. Element-wise init `C3DVector v(m.X, m.Y, m.Z)` breaks it, and reusing one local for the second copy matches retail's slot reuse (00726ce0).
- A two-bitfield bool in if-form (`bool b = A; if (!b) b = B;`) gives `shr/and bl,1` with a jne skip (007f8890). `long t = 1; if (b) t = 9;` gives the branch form, not setne/lea.
- An 8-byte packed intelligent-pointer temporary gives the `and esp,-8` frame. It must be a named block local `{ CIntelligentPointer<CThing> p(x); list.push_back(p); }` (007f8890).
- A 1e-4 floor through `Max(range, g_MinSplineRange)` with an extern const global gives retail's `mov eax,&const` pick (00766660).
- `#include <string.h>` is needed for `wchar_t`/L"" to be a real type under the lane's VC7.1 flags (0055d180).
- A `const CWideString&` parameter's dead argument slot can host a local CCharString returned by hidden pointer (0055d810).
- `#pragma optimize("s")` / `("gsy")` does not remove loop-head alignment in these rows; it switches to an ebp frame and is worse (007243c0, 00726980, 0075aaf0).
- The out-param GetInterface without a result check reads an uninitialised emitter from an argument slot. Where retail zero-initialises, write `emitter = 0` first (0079deb0).
- CTCPhysicsRigidBody has CRigidBody at +0xa0 (OneOverMass +0xa0, CoR +0xa4, InvBodyInertia +0xa8, CurrentConfig +0xf0, LastConfig +0x1a4). CRigidBodyConfig is 0xb4 bytes (CMVelocity +0x3c, AngularMomentum +0x48, InvWorldInertia +0x6c, AngularVelocity +0x9c, BoundingVertices +0xa8). CollisionNormal is at +0x40 and CollidingVertexIndex at +0x4c.
- CTCQuestCompletionUI retail offsets: Active +0xc, the wait flags +0x1d..0x1f, Completion +0x20, PCardDef +0x2c, Boasts +0x30, RenderedObjectDef +0x34, the summaries +0x38/0x3c/0x40, GoldReward +0x58, RenownReward +0x5c, PBlack +0xbc, QuestCardDef +0xc4.

Review at landing:
- 005e4430 (775v775, 18 lines) and 00726ce0 (1002v1011) are near-miss sweep candidates.
- Two rows have /GS: 007e6450 and 0055d180. Rows with jump tables: 007e6450, 0079e5f0, and the three skipped rows.
- Matched rows use same-TU noinline stand-ins (SetHeadTurnState, GetSubDef, LowerBound); check the one-symbol rule. The LowerBound stand-ins are non-const (0x40F020).
- 007f8890 references GetSubDef<CParticleAttacherDef> 0x7F87A0, and the existing label for it is wrong.
