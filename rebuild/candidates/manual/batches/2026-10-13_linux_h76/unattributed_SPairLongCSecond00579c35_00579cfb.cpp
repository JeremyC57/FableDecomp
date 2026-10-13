// SPairLongCSecond_00579c35::SPairLongCSecond_00579c35(const SPairLongCSecond_00579c35&) -- copy constructor of a {unsigned long, CSecond_00579c35} pair: scalar copied inline, CSecond_00579c35 copy ctor @ 0x00579C35
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00579CFB, oracle length 29 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  mov eax,[esp+4] ; push esi ; mov esi,ecx ; mov ecx,[eax] ; add eax,4 ; mov [esi],ecx ; push eax ; lea ecx,[esi+4] ; call CSecond_00579c35::CSecond_00579c35(const CSecond_00579c35&) ; mov eax,esi ; pop esi ; ret 4
// Copy constructor of a std::pair-like {unsigned long, CSecond_00579c35} value (the map node value of the
// manifest label `pair<unsigned_long,...>`): the scalar first member is copied inline and the
// second member through its out-of-line copy constructor @ 0x00579C35; `this` is returned.
// The second type is named from the constructor's label when unique. One of 13 bodies.
struct CSecond_00579c35 {
    void* m_data;
    CSecond_00579c35(const CSecond_00579c35& other);           // 0x00579C35
};

struct SPairLongCSecond_00579c35 {
    unsigned long first;                   // +0
    CSecond_00579c35 second;                         // +4
    SPairLongCSecond_00579c35(const SPairLongCSecond_00579c35& other);
};

SPairLongCSecond_00579c35::SPairLongCSecond_00579c35(const SPairLongCSecond_00579c35& other)
    : first(other.first)
    , second(other.second)
{
}
