// SPairLongCSecond_005b12f4::SPairLongCSecond_005b12f4(const SPairLongCSecond_005b12f4&) -- copy constructor of a {unsigned long, CSecond_005b12f4} pair: scalar copied inline, CSecond_005b12f4 copy ctor @ 0x005B12F4
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005B1653, oracle length 29 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  mov eax,[esp+4] ; push esi ; mov esi,ecx ; mov ecx,[eax] ; add eax,4 ; mov [esi],ecx ; push eax ; lea ecx,[esi+4] ; call CSecond_005b12f4::CSecond_005b12f4(const CSecond_005b12f4&) ; mov eax,esi ; pop esi ; ret 4
// Copy constructor of a std::pair-like {unsigned long, CSecond_005b12f4} value (the map node value of the
// manifest label `pair<unsigned_long,...>`): the scalar first member is copied inline and the
// second member through its out-of-line copy constructor @ 0x005B12F4; `this` is returned.
// One of 13 bodies.
// Name fix (agentNF54, nf44:52): the second member is the shared STLport std::pair<CWideString, long> (stlport/fable_containers.h +
//   fable_string.h; the CSecond_005b12f4 stand-in is gone), whose copy constructor this size-optimised TU calls out of line: the landed
//   0x005B12F4 ??0?$pair@VCWideString@@J@std@@QAE@ABU01@@Z (body: CWideString copy, then the long). The enclosing {4-byte key, pair}
//   value keeps its descriptive stand-in name: its key type is not proven (the neighbouring 0x005B17C6 builds a
//   pair<NUISystem::CComponent* const, pair<CWideString, long> >, but no second source ties this copy to that map).
#pragma optimize("s", on)
#include "fable_string.h"
#include "stlport/fable_containers.h"

struct SPairLongCSecond_005b12f4 {
    unsigned long first;                             // +0
    std::pair<CWideString, long> second;             // +4
    SPairLongCSecond_005b12f4(const SPairLongCSecond_005b12f4& other);
};

SPairLongCSecond_005b12f4::SPairLongCSecond_005b12f4(const SPairLongCSecond_005b12f4& other)
    : first(other.first)
    , second(other.second)
{
}
