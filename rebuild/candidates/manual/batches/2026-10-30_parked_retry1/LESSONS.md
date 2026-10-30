# retry1 (wave 1) -- lessons

## New rules
- Same-TU static private convention: a parked row whose callee is a file static (or whose caller passes args in odd registers) matches once the static body and its real caller(s) sit in one TU; VC7.1 picks the private convention from the body (88 wins). C TUs need /Gr where retail is fastcall.
- Callee-first mod-ref: define the same-TU callee body BEFORE the caller; VC7.1 tracks which registers the callee clobbers and allocates the caller around them (22 wins).
- Parameter declaration order of a static drives its register/stack assignment (00c5d1f0: initial before step).
- libpk/engine fistp helpers: use the LANDED inline-asm helpers verbatim (CeilToLong / FloorToLong / FloatToLong); every 'inline-asm fistp lane gap' row attempted is reproducible (grade retail_asm).
- MXCSR save/round/restore: _mm_getcsr/_mm_setcsr as a single initializer reproduces retail (00c5d2e0).
- A copy constructor written memberwise vs implicit changes which register carries the pointer (copy-ctor lever).
- libvorbis todB: the pre-union pointer-cast body matches the 1.0-era scales.h, not the union version.
- 00c6b7c0: named shared (1-t) local fixes x87 fxch order of a later lerp
- purity.classify reports single-underscore _asm blocks as genuine (only __asm is detected) -- spelled __asm
- 00c0b130/00c0b420: a fastcall callee's parameter DECLARATION order (float first; integral args keep ecx/edx, stack order unchanged) drives the call site's argument evaluation order -- fixes 'register arg formed between the pushes' scheduling residues; respell the callee (NAME_FIXES)
- block placement: VC7.1 places the tail-merged shared 'return K' block (also reached from an earlier path) right after the last compare; retail places the local tail first (0085fb30, 0085fa00) -- no spelling moves it
- retail_asm ceil rows (00516830/00516200/005115e0/00511a10): with the landed CeilToLong asm in place VC7.1 sinks the bound conversions into the && chain; growing the COPIED box in place (area.X1 = (long)(area.X1 + r) ...) keeps retail's order and frame
- 00b1bc80: VC7.1 folds x * (1.0f/2047.0f) * 20.0f into one constant multiply inside one expression; retail's two fmuls come from the fraction held in a named local
- 005ee77c: inline GetInterface has two retail selection shapes -- 'if (it == last || wanted < it->type) it = last' (jle ; mov eax,last) and 'found = last ; if (it != last && !(wanted < it->type)) found = it' (jg ; mov edi,eax)
- 00654438: the landed CeilToLong local DECLARATION order (result, one, half, back) is what assigns retail's frame slots for the helper's by-value copy and constants
- 0085fbc0: even a single textual 'return false' reached by goto lands after the last compare, so VC7.1's placement of the shared false block is not driven by where the return statement is written (strengthens the 0085fb30/0085fa00 block-placement gap).
- 004385f0/00439bb0: the PDB accessor GetCrime() (inline, returns *PCrime) as the object of an inline setter whose argument is a call pins the object pointer in a callee-saved register before the call (retail's push ebp / mov ebp,[esi+2ECh]); PCrime-> lets VC load it after the call.
- 00774610: in a hand STLport _Rb_tree model, the mem-initializer _M_key_compare() value-initialises the empty less<> and VC7.1 stores a zero byte; STLport's own spelling _M_key_compare(_Compare()) stores nothing (retail).
- 00914650: a GetTC written inline over m_interfaces keeps the owning thing in edi; the same select as a CInterfaceMap member (Find) keeps &m_interfaces in edi and reads last as [edi+4], matching retail.
- 0066cd50: CRGBColour follows the PDB: user copy ctor (R,G,B,A byte stores) + implicit operator= (dword copy); modelling it as POD loses the byte-wise argument copies.
- 00a88d10: the constant-folding lesson holds for x*0.5f*1024.0f too (named half-angle local keeps two fmuls).
- 00a76e00: map-relative cell coordinates written as C2DVector operator- arithmetic reproduce retail's spill order (scalar locals do not).
- 00b16220: indexing Orbits[i] / other.Orbits[i] directly (no o/p reference locals) gives retail's two pointer IVs plus delta register.
- 006cc800: the frame-stagger idiom is the landed `__int64 uid = thing->UniqueID; (CWorld::GetFrame() + uid) % period` (uid loaded before the call, __allrem).
- purity.classify reports a single-underscore `_asm` block as genuine: always spell `__asm` (pipeline gap worth fixing in crawl/purity.py).

## Gap labels disproved (row is NOT a lane gap)
- inline-asm fistp / FtoL / ceil / floor rounding ("asm_bake family", GOTCHAS 006472b2 family): disproved on every row tried -- 00516830, 00516200, 005115e0, 00511a10, 00774610 (all matched), and 006378e0, 0057b26f, 00b947e0, 007e2df0, 00f36540, 00b94bf0, 00b84720, 00b84580, 00b1bc80, 00b18820, 005ee77c, 006b2960, 00654438, 0066cd50, 00439bb0, 004385f0, 00b51ad0, 00f36010, 00a88d10, 007727e0, 00a76e00, 00914650, 00b16220, 006cc800 (rounding block now byte-exact; other residue remains).
- 00cb0310 memmoveInternal: not asm_bake -- the DX9 SDK wxutil.cpp __asm body is retail_asm and matches.
- add-ecx-between-pushes scheduling (00c0b420/00c0b130): fixed by the fastcall callee declaration order.
- 00c6b7c0 x87 order: fixed by a named shared (1-t) local.
- ABI gap "caller-cleaned register arg" on the mix kernels (00c5d1f0 family): same-TU static convention.

## Confirmed gaps (instruction pattern no source form produced; levers in STATUS.tsv)
- 009d8650: 42v36: VC saves ebx/esi/edi around any __asm that names them, and the retail pops/ret run on the other stack; needs naked or ret-in-asm (both forbidden)
- 009fd660: 91v91: same select-destination residue as 009fd3a0: retail and eax,edx (value register keeps the result) then the store, then the ReduceSizeFrameDelay load into eax; every spelling (ternary, if-assign, explicit mask either order) gives and edx,eax and hoists the load
- 0085fbc0: 142v142: retail lays the shared xor al,al/ret block right after the first flags test and jumps back to it; VC7.1 lays it after the 0x51 compare in every spelling, even with one textual return false (goto) -- confirmed block-placement gap (0x0085FB30 family)
- 009fdba0: 61v61 (base kept): retail reloads GraphicInfo for the header before the mipmapLevel load and takes sete dl; every spelling loads mipmapLevel first and sets al -- argument-load order inside one call is not source-steerable here
- 00a2ea20: 63v63: the second end() computes into the Size register in ours (lea ecx,[ecx*5]; lea ecx,[edx+ecx*4]) where retail swaps (Size in edx, Elements in ecx); the ternary form is needed for setne and no end spelling moves the registers
- 00d259a0: 64v64: retail loads ParentClass over this (mov esi,[esi+14h]; mov [esi+48h],4), ours into eax; setter shape and direct store give the same eax choice -- a dead-this register reuse no spelling selects
- 00c52c40: 75v75: retail compares Model in eax with cmp in the family-5 block (ecx in family 6); if-chains load into ecx, only a nested switch picks eax and that emits sub/je instead of cmp/jne -- cmp-chain-in-eax not reachable
- 005e9b20: 74v74: the 47 arm's vtable fetch lands in edx because the lane keeps state (eax) live into the arm; retail reuses eax. Shared-tail spelling is not duplicated by VC7.1, so the two-copy layout needs two LeaveGame statements, which always give edx
- 00bc9b30: 66v66: retail colours key=esi, index=edx; every form (loop shapes, recursion, operand order, locals) gives key=edx, index=esi -- a register colouring swap of two loop-invariant/temporary values
- 00be1e20: 77v77: the block copy of the zero temporary into AlphaMaskPositionFactors alternates edx/eax in the lane and schedules lea ecx,[esi+14h] before mov edx,eax; retail forwards all four dwords through edx -- copy-register choice of VC's struct-assignment expansion, no source spelling found (with the parked forms: copy-construct, user operator=, implicit assign)
- 006b5890: 76v76: retail hoists val.second into st(0) above the loop (fcom [middle+4], fstp st(0) at exit) though the float compare is conditional; the lane hoists only val.first and reloads val.second on the equal-key path in every operator< spelling -- conditional x87 load hoisting not reachable
- 00c462e0: c1 load hoist; 009fd3a0: select destination register (and eax,edx); 00cbe583: sum canonicalisation; 0099bb80: lea vs mov+add; 0072aaf0: thiscall object load after the pushes; 0085fb30/0085fa00: shared return block placement; 00c6da40 family: unrolled ramp IV order; 00a4b670: Newton r spill; 00c0acc0: pointer/count register swap (all DIFFER rows, 2-3 grades each).
- 00a48ba0: the STLport <map> header is not in the lane (lane map != STLport: malloc 0x18 header + inline _M_erase); with the asm helper VC also aligns the frame (and esp,-8).
