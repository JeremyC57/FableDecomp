// SPairLongCSecond_0042bf06::SPairLongCSecond_0042bf06(const SPairLongCSecond_0042bf06&) -- copy constructor of a {unsigned long, CSecond_0042bf06} pair: scalar copied inline, CSecond_0042bf06 copy ctor @ 0x0042BF06
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0042CD84, oracle length 29 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  mov eax,[esp+4] ; push esi ; mov esi,ecx ; mov ecx,[eax] ; add eax,4 ; mov [esi],ecx ; push eax ; lea ecx,[esi+4] ; call CSecond_0042bf06::CSecond_0042bf06(const CSecond_0042bf06&) ; mov eax,esi ; pop esi ; ret 4
// Copy constructor of a std::pair-like {unsigned long, CSecond_0042bf06} value (the map node value of the
// manifest label `pair<unsigned_long,...>`): the scalar first member is copied inline and the
// second member through its out-of-line copy constructor @ 0x0042BF06; `this` is returned.
// The second type is named from the constructor's label when unique. One of 13 bodies.
struct CSecond_0042bf06 {
    void* m_data;
    CSecond_0042bf06(const CSecond_0042bf06& other);           // 0x0042BF06
};

struct SPairLongCSecond_0042bf06 {
    unsigned long first;                   // +0
    CSecond_0042bf06 second;                         // +4
    SPairLongCSecond_0042bf06(const SPairLongCSecond_0042bf06& other);
};

SPairLongCSecond_0042bf06::SPairLongCSecond_0042bf06(const SPairLongCSecond_0042bf06& other)
    : first(other.first)
    , second(other.second)
{
}