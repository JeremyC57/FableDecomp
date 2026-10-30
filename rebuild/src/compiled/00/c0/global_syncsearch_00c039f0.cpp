// _global::syncsearch(unsigned* have, unsigned char* buf, unsigned len) -- zlib 1.2.1 inflate.c syncsearch: scan buf for the 00 00 FF FF stored-block marker, carrying the match count in *have, and return the bytes consumed
// Shape (retry1): parked as "LTCG custom register convention"; inside `extern "C" { static __declspec(noinline) }` with a same-TU caller VC7.1 /O2 gives the zlib static retail's private register convention itself (chunk 585/586/593 lever).
// Leaf: _syncsearch
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C039F0, 82 bytes. zlib 1.2.1 inflate.c `local unsigned syncsearch(...)`, a static
//   helper of z_inflateSync, compiled whole-program: the register arguments (edi = have, esi = buf) are the LTCG
//   private convention, which VC7.1 /O2 picks itself for a same-TU static.
// Identity (proven): the zlib 1.2.1 body exactly -- `got < 4 && next < len`, `buf[next] == (got < 2 ? 0 : 0xff)` as
//   `cmp ecx,2 ; sbb ebx,ebx ; and ebx,0xffffff01 ; add ebx,0xff`, `else if (buf[next]) got = 0; else got = 4 - got;`,
//   then `*have = got; return next;`.
extern "C" {
static __declspec(noinline) unsigned syncsearch(unsigned* have, unsigned char* buf, unsigned len)
{
    unsigned got;
    unsigned next;

    got = *have;
    next = 0;
    while (next < len && got < 4) {
        if ((int)(buf[next]) == (got < 2 ? 0 : 0xff))
            got++;
        else if (buf[next])
            got = 0;
        else
            got = 4 - got;
        next++;
    }
    *have = got;
    return next;
}

// Same-TU caller (the syncsearch calls of inflate.c inflateSync), present so the static gets retail's private
// convention. Not graded.
unsigned __cdecl z_inflateSync_syncsearch_caller(unsigned* have, unsigned char* buf, unsigned len) { return syncsearch(have, buf, len); }
}