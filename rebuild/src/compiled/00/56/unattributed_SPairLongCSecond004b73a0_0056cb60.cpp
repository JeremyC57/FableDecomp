// SPairLongCSecond_004b73a0::SPairLongCSecond_004b73a0(const SPairLongCSecond_004b73a0&) -- copy constructor of a {unsigned long, CSecond_004b73a0} pair: scalar copied inline, CSecond_004b73a0 copy ctor @ 0x004B73A0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0056CB60, oracle length 29 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  mov eax,[esp+4] ; push esi ; mov esi,ecx ; mov ecx,[eax] ; add eax,4 ; mov [esi],ecx ; push eax ; lea ecx,[esi+4] ; call CSecond_004b73a0::CSecond_004b73a0(const CSecond_004b73a0&) ; mov eax,esi ; pop esi ; ret 4
// Copy constructor of a std::pair-like {unsigned long, CSecond_004b73a0} value (the map node value of the
// manifest label `pair<unsigned_long,...>`): the scalar first member is copied inline and the
// second member through its out-of-line copy constructor @ 0x004B73A0; `this` is returned.
// The second type is named from the constructor's label when unique. One of 13 bodies.
struct CSecond_004b73a0 {
    void* m_data;
    CSecond_004b73a0(const CSecond_004b73a0& other);           // 0x004B73A0
};

struct SPairLongCSecond_004b73a0 {
    unsigned long first;                   // +0
    CSecond_004b73a0 second;                         // +4
    SPairLongCSecond_004b73a0(const SPairLongCSecond_004b73a0& other);
};

SPairLongCSecond_004b73a0::SPairLongCSecond_004b73a0(const SPairLongCSecond_004b73a0& other)
    : first(other.first)
    , second(other.second)
{
}