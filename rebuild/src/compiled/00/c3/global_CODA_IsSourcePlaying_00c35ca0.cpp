// CODA_IsSourcePlaying(source) -- the CODA C API wrapper: nonzero when the source's +0x10 state flags have any of 0x217
// Leaf: _CODA_IsSourcePlaying@4
// Name (chunk 586): LogStatus 0x00C27760 gates its "CODA reports source IS playing(" line on this call; a `__stdcall` CODA API
// wrapper (ret 4) like CODA_GetSourcePosition 0x00C35CC0 (& 0x216); 0x00C278BE tests the full eax, so it returns long.
struct CODASource { unsigned char pad[0x10]; unsigned int flags; };

extern "C" long __stdcall CODA_IsSourcePlaying(CODASource* source)
{
    return (source->flags & 0x217) != 0;
}