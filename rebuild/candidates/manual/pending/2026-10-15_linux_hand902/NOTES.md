# Hand chunk 902 (CTCPerceiveThing .. CEnginePrimitiveManagerAnimatedMeshesBase): 3 of 5 byte-exact

Chunk 902 is a skipped-ledger chunk with 5 rows, not 40: three tiny CTCPerceiveThing pieces split from one catalogue row
(007460d0) and two whole RenderPrimitive bodies that earlier family scans had cut as 28-byte prefixes.

Grading note: `$S/tools/q.py` takes the FIRST batch oracle row for an address. `chunk98_oracle.tsv` still holds the old
48-byte, three-function row for 007460d0, and `chunk34`/`chunk35` hold the 28-byte prefixes of 00bd71b0/00ba1140. I graded
with `bin/q902.py`, a copy of q.py that reads `chunk902_oracle.tsv` first. `g.sh`/`gall.sh` in this directory call it. The
payload's `oracle.tsv` (from finish.py) carries the correct split bytes.

## What matched (3)
- **CTCPerceiveThing** (vtable 0x012688CC)
  - 007460d0 `vfunc_10`: slot 10 calls the component's own slot 9 (FrameUpdate, 0x00745C70). VC7.1 tail-jumps a virtual
    call that makes up the whole body (`mov eax,[ecx] ; jmp [eax+24h]`). This is the same pair as the landed CTCChest/CTCDoor
    vfunc10. `// Leaf: vfunc_10`.
  - 007460e0 `vfunc_8`: slot 8 returns a constant true (`mov al,1 ; ret`, a bool with no `this` use). `// Leaf: vfunc_8`.
  - 007460f0 `Piece_007460f0`: STLport `list<T>::back()`, i.e. `&_M_node->_M_prev->_M_data` (`mov eax,[ecx] ; mov eax,[eax+4] ;
    add eax,8`). The list base holds only the sentinel pointer, and node data sits at +8. Nothing calls or references it; it is
    a leftover COMDAT between the class's vtable bodies. `// Leaf: back`.

## Parked for Fable 5.1 (2)
- 00ba1140 CEnginePrimitiveManagerStaticMeshesBase::RenderPrimitive DIFFER(2372v2344): retail `fstp [esp+20h] ; fld [esp+20h] ; fistp dword [esp+38h]` and `fstp [esp+40h] ; fld [esp+40h] ; mov edx,[esp+64h] ; fistp dword [edx]` (the engine's inline-asm FtoL) v lane `call __ftol2 ; mov [..],eax`; retail LOD loop `cmp edi,3 ; jae` at the top + `jmp` back v lane rotated `cmp edi,3 ; jb`; `sub esp,298h` v `294h` with slots shifted (`this` +0Ch v +10h, distance +14h v +18h, shadowed +1Bh v +17h); ebx/ebp colouring swapped (retail ebp = 0x4000/zero/primitive, ebx = occlusion record/manager/visibility/flags; lane parks `this` in ebp at entry); plane dot product x,z,y v z,y,x; `movzx edi,al ; and ; or ; shr` v `and eax ; or ; shr ; mov edi,eax` | tried: v1 plain (1962 bytes, nothing inlined), v2 __forceinline helpers + by-value X/Z/Y Length + local-copy transform + `!(d > 1e-4f)` LOD clamp + out-of-line first bounding-box ctor / inline second + explicit out-of-line/inline mesh release, v3 fade as early-return helper + `(unsigned char)(flags >> 9) & 1`, v4 fade if/else chain + `shadowed` declared early (worse), v5 CArray `GetData()` object + switch on type, v6 volatile alpha scale (best: 576 normalised diff lines) | next: lane gap: inline asm (FtoL rounding helper) + loop rotation; once a lane accepts the asm oracle, regrade v6 and re-tune the ebx/ebp colouring
- 00bd71b0 CEnginePrimitiveManagerAnimatedMeshesBase::RenderPrimitive DIFFER(2121v2116): retail `fstp [esp+34h] ; fld [esp+34h] ; mov edx,[esp+58h] ; fistp dword [edx]` (inline-asm FtoL) v `call __ftol2`; retail loads the sphere radius first (`fld [eax+34h]` ... `fadd st(1)` ... `fstp st(0)`), so the transform terms then start z*m20 v lane y*m10; `sub esp,294h` v `290h` with slots shifted (`this` +0Ch v +10h, radius +10h v +18h); ebx/ebp swapped (retail ebx = view/visibility, ebp = new record/mesh kept across the loop; lane spills the mesh pointer); plane dot-product order; `movzx esi,al` form | tried: v1 from the static model (504 lines), v2 if/else type chain 11/38 (best: 486 lines); radius forms probed (by value, by reference, local radius, sphere copy, member helper, explicit sqrt) all load R last | next: lane gap: inline asm (FtoL) + compiler-build x87 scheduling (hoisted radius load)

## Skipped
- None. The three CTCPerceiveThing pieces were already split, one function per row.

## Label corrections
- 007460f0 `CTCPerceiveThing::Piece_007460f0` is an STLport `list<T>::back()` COMDAT, not a CTCPerceiveThing member (its `this` is
  a list whose +0 is the sentinel node).
- 00b98b10 is labelled `CEngineSubPrimitive2DClockSprite::CEngineSubPrimitive2DClockSprite` (propagated). It is the
  `CEngineSubPrimitiveBoundingBox(CEngineInternalPrimitiveBase*)` constructor (vtable 0x012A3E84).
- 00a36ec0 is labelled `_Dest_val<allocator<CCountedPointer<COptimisedPrimitive>>>` (propagated). Here it is the out-of-line
  `CCountedPointer<const C3DMesh2>` destructor body (release, then clear both words).
- 00b84720 is labelled `Engine_AddClockVertex`. It is the four-argument `CEngineSubPrimitiveRenderer::AddSubPrimitive(pass, sub,
  distance, manager)` and uses the same inline-asm FtoL.
- 00a23c50 is labelled `$E2`. It is an empty `ret 4` member, `C3DMeshInfo::RequestLoad(long)` in retail.
- 00b9cb10 is labelled `CEngineSubPrimitiveAnimatedMesh` (ctor). The static path calls it, so it is the
  `CEngineSubPrimitiveStaticMesh` constructor. The animated constructor is 0x00BCE740.
- 00b38600 is labelled `CShadowRendererSceneData::CShadowRendererSceneData(CShadowRendererSceneData*)`. It is the copy constructor
  of the 0x23C-byte shadow scene data (const reference). The label is fine except that the argument is a reference.
