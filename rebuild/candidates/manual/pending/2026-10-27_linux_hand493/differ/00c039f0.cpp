// _global::syncsearch(unsigned* have, unsigned char* buf, unsigned len) -- zlib 1.2.1 inflate.c syncsearch: scan buf for the 00 00 FF FF stored-block marker, carrying the match count in *have, and return the bytes consumed
// Grade: DIFFER(92v82) -- LTCG custom convention: retail passes have in edi and buf in esi (len on the stack, plain `ret`); the lane compiles without /GL so the static gets __cdecl stack arguments
// Leaf: _syncsearch
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C039F0, 82 bytes. zlib 1.2.1 inflate.c `local unsigned syncsearch(...)`, a static
//   helper of z_inflateSync, compiled whole-program: the register arguments (edi = have, esi = buf) are the LTCG
//   private convention, which the Linux lane cannot reproduce (no /GL link step).
// Identity (proven): the zlib 1.2.1 body exactly -- `got < 4 && next < len`, `buf[next] == (got < 2 ? 0 : 0xff)` as
//   `cmp ecx,2 ; sbb ebx,ebx ; and ebx,0xffffff01 ; add ebx,0xff`, `else if (buf[next]) got = 0; else got = 4 - got;`,
//   then `*have = got; return next;`.
// Graded here as an external C function so the body is emitted at all.
extern "C" unsigned __cdecl syncsearch(unsigned* have, unsigned char* buf, unsigned len)
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
