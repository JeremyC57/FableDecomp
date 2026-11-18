# Relocation audit

The exact 733-byte leaf has 13 verified COFF operands. The virtual AddPrimitive calls have no relocations. They reach slot 20 at the native receiver.

| Offset | Retail target | Emitted symbol | Verification |
|---|---|---|---|
| +0x041 | 0x0122F59C | `__real@3f000000` | The retail literal equals the compiler constant. |
| +0x04B | 0x0122F59C | `__real@3f000000` | The retail literal equals the compiler constant. |
| +0x055 | 0x0122F59C | `__real@3f000000` | The retail literal equals the compiler constant. |
| +0x095 | 0x0125A15C | `__real@3d4ccccd` | The retail literal equals the compiler constant. |
| +0x09A | 0x00BFECE2 | `__CItanh` | Exact package binding. |
| +0x0A0 | 0x01231678 | `__real@4014000000000000` | The retail literal equals the compiler constant. |
| +0x0A6 | 0x01238010 | `__real@3fd0000000000000` | The retail literal equals the compiler constant. |
| +0x0ED | 0x00BFEA70 | `__ftol2` | Exact package binding. |
| +0x114 | 0x0063A300 | `?GetGroundSizeZAt@CWorldMap@@QBEMABVC3DVector@@@Z` | Exact package binding. |
| +0x11A | 0x0126A1F4 | `__real@3e19999a` | The retail literal equals the compiler constant. |
| +0x19A | 0x0122DED8 | `__real@3f800000` | The retail literal equals the compiler constant. |
| +0x1F3 | 0x0063A300 | `?GetGroundSizeZAt@CWorldMap@@QBEMABVC3DVector@@@Z` | Exact package binding. |
| +0x1F9 | 0x0126A1F4 | `__real@3e19999a` | The retail literal equals the compiler constant. |
