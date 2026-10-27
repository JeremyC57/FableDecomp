# Chunk 505 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- A named count local (`unsigned long count = m_rows.m_count; if (row < count)`) gives retail's `cmp ecx,eax` order, where the direct member compare is reversed (0x00C34F30).
- With an implicit C3DVector copy, X is routed through an integer register plus esi. A user member-wise copy ctor (`X(o.X), Y(o.Y), Z(o.Z)`) keeps X in st0 (0x00C40650).
- /GS cookie: local `float` arrays get no cookie, but local `unsigned char[0x100]` blocks with align(16) do (0x00C54F30).
- In unrolled mixer loops, the indexed `dst[i] += ...` form gives retail's two-element pointer bias. Bumped pointers do not (0x00C58140).
- For a bool returned from an HRESULT test:
  - `hr == 0 ? true : false` gives `neg ; sbb al,al ; inc al`.
  - `!hr` gives the full-register `sbb eax,eax ; inc eax` (0x00C56D70).
- `while (n-- > 0)` gives the trailing-decrement loop (0x00C57D80).
- A `switch` whose `default:` zeroes a local threads straight to the loop exit (CEnvelope rows).
- For the CAudioObject set-or-clear records, operator== must be `__forceinline` and must keep retail's odd field order. The 8-float record repeats field 0 (0x00C430E0, as 0x00C43000).
- A 7-word struct assignment `m_desc = *buffer->GetDesc();` gives `mov esi,eax ; lea edi ; mov ecx,7 ; rep movsd` (0x00C5A2E0).
- An __int64 member right after the vptr needs `#pragma pack(4)`. Otherwise VC aligns it to 8, which shifts every later offset by 4 (0x00C5B250).
- An 8-bit PCM widen `(s - 0x80) << 8` with retail `movsx` reads the sample as `signed char` (0x00C5A5B0).
- A RELOCATION_MATCH can hide swapped loads of two fields of the SAME global (`mov edi,[g+0Ch] ; imul edi,[g+18h]`): the mask hides the addends, and only vland's RELOC_IDENTITY ("symbol bound to 3 addresses") catches it. `a * b` loads b first here, so write `m_writePos * m_frameBytes` (0x00C5A5B0).
- In mangled leaves, a forward `struct X;` is `PAU`, not `PAV`. Check every leaf with objdump -t. A miss silently grades the only or largest function (0x00C5B250).

Review at landing:
- 0x00C29DC0: the catalog row holds seven functions (int3-separated). It needs re-splitting.
- RELOC_CONFLICT is expected on:
  - 0x00C49100: SDecoderMessage vs the landed SQueuedSoundEvent.
  - 0x00C42A60 / 0x00C42B50: CPlaybackEntry vs CProfileMember / T::Init / CUnprovenPtrList_00c42b50.
  - Settle each by evidence. The queue owning 0x00C49100 is PKCStaticQueue<SDecoderMessage> with a 0x40-byte record.
- Landed PKCQueue declarations give one-shot Push a `long` return. Retail sets no eax, so it is void.
- Landed 0x00C25730 C3DVector::operator!= is declared int, but its callers test `al`.
- 0x0139CA20 has three landed spellings: g_performanceFrequency, g_PerformanceFrequency (LARGE_INTEGER) and g_AudioClockRate. This chunk uses g_performanceFrequency.
- 0x0143BF88 has two landed spellings: g_monoRing and g_CStatic_00c5d0a0_0143bf88. This chunk uses g_monoRing.
- 0x00C0BA40 / 0x00C0BFE0 have two landed spellings: CDebugStream, used by the trace rows, and CString, used by the catalog and the map rows. All nine rows in this chunk use CDebugStream, so it has one name per address. Landing should pick one spelling.
- Rows needing more than 4 grades: none.
- Local models of GameCODA classes have no shared header. See NOTES "Header debt".
