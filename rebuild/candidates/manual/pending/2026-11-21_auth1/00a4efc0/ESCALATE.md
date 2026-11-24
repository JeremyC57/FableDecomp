# ESCALATE 0x00A4EFC0 C3DAnimationSequenceData::InitialiseFromFileData (3491 B) -- AUTH1, 2026-11-21
S = /tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad

## Best source
S/w22/auth1/00a4efc0/e1.cpp, /Gr, FABLE_HDR_OVERLAY=S/w22/auth1/00a4efc0/hdr: DIFFER(3491v3491), 16 differing lines = ONE residue (+ the
alignment pad it shifts). Equivalent to B's reported p25P state (their r5/best_nb.cpp is not in the repo, so my AllocateType text may
differ from theirs): repo draft + CARRAY_INDEX_ASSERT +
ASSERT_DEAD_ARM + an Ego_d-shaped AllocateType (new add-only overlay opt-in FABLE_RETAIL_CSIMPLEMEMORYPOOL_ALLOCATETYPE_EGOD, decoded
from Ego_d AllocateType<char/unsigned char/CQuaternion> lines 21..34; it removed residue (3), the scaling |s-1| sum order).

## Remaining diff (0xA4F7DF..0xA4F810, the position compressed / uncompressed sizes, Ego_d 1206/1207)
```
-mov dl, byte ptr [esi+0Ah] ; or dl,40h ; mov ebx,[esp+54h] ; mov [esi+0Ah],dl     (AnimatedFlag; paletted finish -> ebx)
+mov al, byte ptr [esi+0Ah] ; or al,40h ; lea edx,[ecx+ecx*2] ; shl edx,1 ; mov ebx,edx ; mov edx,[esp+54h] ; mov [esi+0Ah],al ;
+sub ebx,ebp ; add ebx,edx                                                          (ours: compressed first, into ebx)
 mov edx,[esp+6Ch] ; sub edx,edi ; imul 0x2AAAAAAB ; ... ; add eax,edx              (uncompressed / 6)
-lea edx,[eax+eax*2] ; lea eax,[ecx+ecx*2] ; shl eax,1 ; sub eax,ebp ; shl edx,1 ; add eax,ebx ; cmp eax,edx
+lea eax,[eax+eax*2] ; shl eax,1 ; cmp ebx,eax
-lea esp,[esp]   /  +lea ebx,[ebx]                                                   (pad, +1 B knock-on)
```

## Tried this pass (LOG.md): a0 3481/43; a1 (+index assert, dead arm) 3491/26; e1/e2 (+Ego_d AllocateType, two dead-arm conditions)
3491/16; x1 compressed operands swapped 3491/16; x2 `uncompressed > compressed` 3491/17; x3 `!(compressed >= uncompressed)` 3491/16.

## Hypotheses
p25P: a colouring split (the paletted finish piece dropped in ours, kept in ebx in retail), decided by c2 symbol / temp numbers that
come from the whole function's helper texts (a kind-3 IV temp takes its number from the function's temp count; the AllocateType text
moved residue (3) the same way). FableWin (3936 B) != Ego_d (3872 B): FableWin is the oracle for the remaining helper texts and is on
the Windows request. The AllocateType text is size-checked line for line against Ego_d (LOG.md step 8). The other expanded helpers' Ego_d tables are
one-statement bodies like the overlay's (read, not yet calibrated at /Od byte level: that is the open lever).

## Question for the advisor
(1) Is the AllocateType opt-in acceptable as an add-only Inferred header text (Ego_d line sizes for three instantiations, results equal
to the default; the dead assert's condition is unconstrained -- which text should it carry)? (2) For the last residue, is a temp-count
dial (one more Ego_d-shaped helper symbol somewhere) admissible if it comes from an Ego_d / FableWin line-table decode, or does the row
wait for FableWin's lines?
