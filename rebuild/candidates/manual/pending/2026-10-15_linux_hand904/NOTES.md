# Chunk 904 -- skipped-ledger pieces (9 bodies from 3 multi-function rows), hand-authored by the coordinator (Fable 5.1)
All three rows carried the propagated label CThingFilter_IsValid; none of the bodies is a thing filter.
- 005271b0 (70-byte row): a slot-43 forwarder (caller 0x00525D37 loops it over an array of objects) and a six-argument 0x18-byte record constructor (two flags, two longs, two CCharString copies through the landed 0x0099EC30). 2 of 2 matched.
- 00ac20d0 (38-byte row): a slot-4 forwarder with 7+ engine-library callers, a 16-bit element getter and a getter returning the static "MPRT" descriptor at 0x0129EF40. 3 of 3 matched.
- 00caef70 (51-byte row, DirectShow base-class region): three `return this` accessors matched; the __stdcall COM forwarder thunk (0x00CAEF70, 9 bytes) is parked -- the lane re-stores the object into its argument slot before the tail jump (7 forms, differ/).
## Parked
- 00caef70 func_00caef70 DIFFER(13v9): argument-slot re-store before a tail-called __stdcall method | tried: __stdcall/__cdecl free function, static member, __stdcall member, C-style COM tables (typed / void*), size pragma | next: lane gap (argument-slot reuse)
