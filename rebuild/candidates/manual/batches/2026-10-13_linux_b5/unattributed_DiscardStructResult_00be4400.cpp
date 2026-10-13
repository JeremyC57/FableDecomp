// <unattributed>::DiscardStructResult() -- calls a 24-byte-struct-returning method and drops the result
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BE4400, oracle length 16 bytes.
// Grade: landed -- byte-exact (MATCH/RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13); the lander's winning pragma, if any, heads the file.
// The manifest label for this body is propagated across byte-identical bodies and is NOT trusted;
// the class is modelled from the bytes only and the name below describes what the code does.
//
// Retail bytes:  sub esp,18h ; lea eax,[esp] ; push eax ; call Method_00be4380 ; add esp,18h ; ret
// A __thiscall method @ 0x00BE4380 returning a 24-byte struct through the hidden pointer is called
// for its side effects; the temporary lives in the 0x18-byte frame and is not used.
struct CResult24 {
    long Values[6];                         // 24 bytes, trivially destructible
};

struct CDiscarder_Retail {
    CResult24 Method_00be4380();            // 0x00BE4380
    void DiscardStructResult();
};

void CDiscarder_Retail::DiscardStructResult()
{
    Method_00be4380();
}
