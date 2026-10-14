// CRetailVector_dword_0074cbc0::at(unsigned long) -- range-checked element reference (STLport vector::at with exceptions off)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0074CBC0, length 42 bytes.
// Callees: puts @ 0x00BFEAF2 with the literal "vector" @ 0x01268AE4, abort @ 0x00BFEAEC.
// The manifest label (CCombatWheel::GetRing) is propagated; the caller is CTCContainerRewardHero::GetRewardItems @ 0x0074E860 (0x0074D120 is the byte-identical copy used by CTCCreatureGenerator::CreateCreatures). The body is
// STLport's `_M_range_check` (`if (n >= size()) __stl_throw_out_of_range("vector")`, which without
// exceptions is `puts(msg); abort()`) followed by `*(begin() + n)` on 4-byte elements. `abort` must be
// `noreturn`: the element address is formed after the call from the registers loaded before it.
// Named by what it does.
extern "C" int puts(const char* text);                      // 0x00BFEAF2
extern "C" __declspec(noreturn) void abort(void);           // 0x00BFEAEC

struct CRetailVector_dword_0074cbc0 {               // retail std::vector of 4-byte elements
    unsigned long* first;
    unsigned long* last;
    unsigned long* end_of_storage;
    unsigned long size() const { return (unsigned long)(last - first); }
    unsigned long& at(unsigned long n);
};

unsigned long& CRetailVector_dword_0074cbc0::at(unsigned long n)
{
    if (n >= size()) {
        puts("vector");
        abort();
    }
    return first[n];
}
