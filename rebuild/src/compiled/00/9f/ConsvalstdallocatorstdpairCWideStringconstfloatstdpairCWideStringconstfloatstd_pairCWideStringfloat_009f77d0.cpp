// Byte-exact reconstruction of _Cons_val<...> @ 0x009f77d0
// Member-style (this in ecx, no edx), one local slot (push ecx), two stack args
// (ret 8), only first stack arg used. If dest != 0, forward to cdecl destroy
// helper (call 0xbfea14; add esp,4 => cdecl caller-cleanup).

struct Pair; // std::pair<CWideString const,float>

extern "C" void __cdecl free(void* block);   // 0x00BFEA14 (MSVCR71)

// Modeled as a real non-static member => __fastcall: this in ecx, args on stack,
// no edx spill. Retail was emitted at /Od (frame kept + dead this-store).
struct AllocT
{
    void cons_val(Pair* dest, Pair* val);
};

#pragma optimize("", off)
void AllocT::cons_val(Pair* dest, Pair* val)
{
    (void)val;
    if (dest != 0)
    {
        free(dest);
    }
}