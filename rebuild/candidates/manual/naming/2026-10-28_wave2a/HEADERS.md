# Header items (agent A, wave 2) -- not edited; each needs every includer rechecked

## 1. `rebuild/include/engine/CDataInputStream.h`: the private ReadWithSrcChunkOverflow (0x00993CA0)
Retail: `?ReadWithSrcChunkOverflow@CDataInputStream@@AAEXPAEJ@Z` (private; Ego_r + FableWin). The generated header has
only the six data members, so 36 rows that include it model the call through a derived view struct that declares
the member itself (`?ReadWithSrcChunkOverflow@<View>@@QAEXPAEJ@Z`). Proposed lines, after `SourceChunkBytesRemaining`:
```
#ifdef FABLE_CDATAINPUTSTREAM_FRIEND
    friend FABLE_CDATAINPUTSTREAM_FRIEND;     // the row's inline-Read view (retail inlines CDataInputStream::Read)
#endif
private:
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0
```
(a non-virtual member function: no layout change; the FABLE_STATIC_ASSERTs stay true). Each affected row then drops
its own declaration and puts `#define FABLE_CDATAINPUTSTREAM_FRIEND struct <View>` before the include (the call
`ReadWithSrcChunkOverflow(...)` inside the view then resolves to the base member). The principled alternative is the
retail inline `CDataInputStream::Read` in the header calling the private member.
Affected rows (emit the view spelling today): CDataInputStreamReader 00403eb0 0044fc00 00458f89 00469d47 0046abd6
0046c3b9 0046fee9 004e9021 004ea2c9 006778a0 0067f870 009945e0 00994670 00994c80 00994d20 00995030 009950f0
00995770 00996390 009964e0 009d4ab0 009fd160 00bf5a30; CInputStreamAccess 00aa4e40 00aa5030 00aa66b0 00aa6960
00aa6a60 00abcf40 00abd150; CDataInputStream_Methods 00995fb0 00996230; CMemoryDataInputStream 006dc070 (wave-1
KEEP binding); InputCalls 00b1b1b0; InputReader 009c8470 (AAE already, wrong class); 00bde190 (includes the header and
also models a local class). 78 sources include the header in total: all must be rechecked with the edit.

## 2. `rebuild/include/engine/CWorld.h`: `static long __fastcall GetFrame();   // 0x0049D870`
The generated header declares no member functions; a row that includes it and calls the frame getter must use a free
stand-in. Proposed line (inside `class CWorld`, after `public:`): `static long __fastcall GetFrame();   // 0x0049D870`
(static member: no layout change). Affected row today: 0097d360 (`GetGameTick`, `long FABLE_FASTCALL`); 214 sources
include the header.
