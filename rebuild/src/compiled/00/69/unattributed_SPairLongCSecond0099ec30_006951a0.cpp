// SPairLongCSecond_0099ec30::SPairLongCSecond_0099ec30(const SPairLongCSecond_0099ec30&) -- copy constructor of a {unsigned long, CSecond_0099ec30} pair: scalar copied inline, CSecond_0099ec30 copy ctor @ 0x0099EC30
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006951A0, oracle length 29 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  mov eax,[esp+4] ; push esi ; mov esi,ecx ; mov ecx,[eax] ; add eax,4 ; mov [esi],ecx ; push eax ; lea ecx,[esi+4] ; call CSecond_0099ec30::CSecond_0099ec30(const CSecond_0099ec30&) ; mov eax,esi ; pop esi ; ret 4
// Copy constructor of a std::pair-like {unsigned long, CSecond_0099ec30} value (the map node value of the
// manifest label `pair<unsigned_long,...>`): the scalar first member is copied inline and the
// second member through its out-of-line copy constructor @ 0x0099EC30; `this` is returned.
// The second type is named from the constructor's label when unique. One of 13 bodies.
struct CSecond_0099ec30 {
    void* m_data;
    CSecond_0099ec30(const CSecond_0099ec30& other);           // 0x0099EC30
};

struct SPairLongCSecond_0099ec30 {
    unsigned long first;                   // +0
    CSecond_0099ec30 second;                         // +4
    SPairLongCSecond_0099ec30(const SPairLongCSecond_0099ec30& other);
};

SPairLongCSecond_0099ec30::SPairLongCSecond_0099ec30(const SPairLongCSecond_0099ec30& other)
    : first(other.first)
    , second(other.second)
{
}