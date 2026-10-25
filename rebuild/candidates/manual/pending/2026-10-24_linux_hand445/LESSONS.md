# Chunk 445 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- **A memset count of `mov ecx,R ; shl ecx,2` means an inline member of a sub-object.**
  - Retail's two-pass DSP allocators clear their ring with `mov ecx,esi ; shl ecx,2 ; ... shr ecx,2 ; rep stosd ; ... and ecx,3 ; rep stosb` after the m_size/m_pos stores. The pattern occurs 5 times in the exe, all in libpk; `lea ecx,[R*4]` before a memset occurs 0 times.
  - Only a memset inside an inline method of a *sub-object* reproduces it: `struct CRingBuffer { ... void Clear() { memset(m_data, 0, m_size * sizeof(float)); } }`, called as `m_ring.Clear()` after `m_ring.m_size = size`. VC forwards the stored value late, giving `mov ecx,esi ; shl`, and keeps the stores first.
  - A memset of `size * 4`, `m_size * 4`, `size << 2` (drops the stosb tail) or an inline helper on `this` gives `lea ecx,[esi*4]` scheduled above the stores. So do /G5 / /G6 / /G7 / /Ox (0x00C89110, 0x00C896C0).
- **Inner constant loop = outer loop stays rolled.** `float sum = in[i]; for (j = 0; j < 4; j++) sum += blocks[j][i] * row[j];` fully unrolls the inner loop in source order (in first, then block 0..3) and leaves the outer variable-count loop rolled. The single-expression sum is reassociated *and* the outer loop is unrolled x4 (0x00C89AA0).
- **`do { } while (++i < 4)` stays rolled; `for (i = 0; i < 4; i++)` with a small x87 body is fully unrolled.** The do-while keeps a `mov eax,4 ... add eax,8 ; cmp eax,24h` byte-offset counter (0x00C780C0 probes).
- **extern "C" callees under /EHsc are nothrow.** An EH ctor whose body calls C entry points after a member with a destructor loses its `mov byte [esp+N],1` state store if those callees are declared `extern "C"`. Declare them with C++ linkage, or /EHs would be needed. The landed 0x00C885C0 dtor in the same TU already models its ov_clear call as a C++ member (0x00C88780).
- **Named locals steer register choice in EH ctors.** `vorbis_comment* vc = ov_comment(..); char** p = vc->user_comments; vorbis_info* vi = ov_info(..);` plus an empty `for (; *p; ++p) {}` gives retail's ebp (member, then cursor, then index) / edi (file). The one-expression spelling swaps ebp and edi (0x00C88780).
- **libpk vorbisfile = libvorbis 1.0.1 built /Gr with LTCG statics.**
  - The public ov_* functions match verbatim as `extern "C" __fastcall` (`// Leaf: @ov_clear@`).
  - `vf->bittrack / vf->samptrack * rate + .5` already compiles to retail's `fild rate ; fdiv ; fmul`.
  - The private statics (_get_data esi, _seek_helper esi, _get_next_page eax) are an LTCG custom-convention lane gap: park after 1 grade.
  - libpk's OggVorbis_File has no ov_callbacks (0x2C0 bytes; ov_clear's `mov ecx,0B0h ; rep stosd`).
- **An unterminated function body hangs cl.exe** until `timeout 200` kills it (exit 143). Never `sed` away closing braces in probes; build probe variants with a script that keeps the braces balanced.
- **`long long` / `LL` literals in VC7.1 tests.** Use `__int64` under `#ifdef _MSC_VER`, and write 64-bit constants as `((ogg_int64_t)1 << 32)`. g++ tests drop `__fastcall` / `__cdecl` (x64 g++ has no such keywords).

Review at landing:
- Reconcile the shared reverb model with agent444. This chunk models the ring as a `CRingBuffer` sub-object, with members `m_ring.m_data / m_size / m_pos`. agent444 has flat `m_data / m_size / m_pos` members in CReverbDiffuserStage / CReverbFilterTap / CReverbTap. The offsets are identical; the bodies need the sub-object.
- agent444's parked 0x00C75F80 CReverbTap::Allocate should match with the CRingBuffer::Clear model (same `mov ecx,esi ; shl ecx,2` count).
- Fix the landed 0x00C885C0 model and its class name to COggVorbisStream: +0x78 is the opened flag, the +0x80 member is the OggVorbis_File + ov_clear, and the +4 reader is 0x70 bytes. Rename 0x00C88870 as its scalar deleting destructor, and 0x00C4B270 as CStreamReader's dtor.
- 0x00C88780 declares the vorbisfile entry points with C++ linkage, but 0x00C8A7E0 / 0x00C8A960 / 0x00C8AA80 define them `extern "C" __fastcall`. Bytes are identical either way; pick one symbol spelling for the TU at landing (the EH state store needs either C++ linkage or /EHs).
- The landed 0x00C7B8F0 (Is32vec4::operator>>=) is declared void; retail returns *this in eax.
