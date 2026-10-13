// SPairLongCSecond_005b12f4::SPairLongCSecond_005b12f4(const SPairLongCSecond_005b12f4&) -- copy constructor of a {unsigned long, CSecond_005b12f4} pair: scalar copied inline, CSecond_005b12f4 copy ctor @ 0x005B12F4
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005B1653, oracle length 29 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  mov eax,[esp+4] ; push esi ; mov esi,ecx ; mov ecx,[eax] ; add eax,4 ; mov [esi],ecx ; push eax ; lea ecx,[esi+4] ; call CSecond_005b12f4::CSecond_005b12f4(const CSecond_005b12f4&) ; mov eax,esi ; pop esi ; ret 4
// Copy constructor of a std::pair-like {unsigned long, CSecond_005b12f4} value (the map node value of the
// manifest label `pair<unsigned_long,...>`): the scalar first member is copied inline and the
// second member through its out-of-line copy constructor @ 0x005B12F4; `this` is returned.
// The second type is named from the constructor's label when unique. One of 13 bodies.
struct CSecond_005b12f4 {
    void* m_data;
    CSecond_005b12f4(const CSecond_005b12f4& other);           // 0x005B12F4
};

struct SPairLongCSecond_005b12f4 {
    unsigned long first;                   // +0
    CSecond_005b12f4 second;                         // +4
    SPairLongCSecond_005b12f4(const SPairLongCSecond_005b12f4& other);
};

SPairLongCSecond_005b12f4::SPairLongCSecond_005b12f4(const SPairLongCSecond_005b12f4& other)
    : first(other.first)
    , second(other.second)
{
}