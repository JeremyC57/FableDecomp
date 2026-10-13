// SPairLongCSecond_0099b720::SPairLongCSecond_0099b720(const SPairLongCSecond_0099b720&) -- copy constructor of a {unsigned long, CSecond_0099b720} pair: scalar copied inline, CSecond_0099b720 copy ctor @ 0x0099B720
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00564B20, oracle length 29 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  mov eax,[esp+4] ; push esi ; mov esi,ecx ; mov ecx,[eax] ; add eax,4 ; mov [esi],ecx ; push eax ; lea ecx,[esi+4] ; call CSecond_0099b720::CSecond_0099b720(const CSecond_0099b720&) ; mov eax,esi ; pop esi ; ret 4
// Copy constructor of a std::pair-like {unsigned long, CSecond_0099b720} value (the map node value of the
// manifest label `pair<unsigned_long,...>`): the scalar first member is copied inline and the
// second member through its out-of-line copy constructor @ 0x0099B720; `this` is returned.
// The second type is named from the constructor's label when unique. One of 13 bodies.
struct CSecond_0099b720 {
    void* m_data;
    CSecond_0099b720(const CSecond_0099b720& other);           // 0x0099B720
};

struct SPairLongCSecond_0099b720 {
    unsigned long first;                   // +0
    CSecond_0099b720 second;                         // +4
    SPairLongCSecond_0099b720(const SPairLongCSecond_0099b720& other);
};

SPairLongCSecond_0099b720::SPairLongCSecond_0099b720(const SPairLongCSecond_0099b720& other)
    : first(other.first)
    , second(other.second)
{
}
