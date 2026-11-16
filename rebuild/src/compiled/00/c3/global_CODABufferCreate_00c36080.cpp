// CODA_BufferCreate(const _CBUFFERDESC*) -- create a CODA buffer from its descriptor (forwards to the pool allocator 0x00C3CD00)
// Leaf: _CODA_BufferCreate@4
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C36080, 16 bytes: `push [esp+4] ; call 0x00C3CD00 ; add esp,4 ; ret 4` (extern "C" __stdcall).
// Name-fix NF38: was `operator_new(unsigned)` (a propagated label naming another function). Descriptive CODA C-API name, as the
//   landed CCodaFrameStreamer::SubmitFrame 0x00C49470 binds it (it passes a _CBUFFERDESC and keeps the result as the frame's
//   buffer handle); same family as the landed CODA_SourcePlay 0x00C35DD0. The callee keeps its landed spelling.
struct _CBUFFERDESC;
extern "C" void* __cdecl helper_00c3cd00(const _CBUFFERDESC* desc);   // 0x00C3CD00
extern "C" void* __stdcall CODA_BufferCreate(const _CBUFFERDESC* desc)
{
    return helper_00c3cd00(desc);
}
