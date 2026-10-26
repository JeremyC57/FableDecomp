// CCreatureAction_SummonerFlameSlice::Clone() -- new CCreatureAction_SummonerFlameSlice(*this)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006F89C0, oracle length 34 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0x11c ; mov esi,ecx ; call operator new ; add esp,4 ; test eax,eax ; je N ; push esi ; mov ecx,eax ; call CCreatureAction_SummonerFlameSlice::CCreatureAction_SummonerFlameSlice(const CCreatureAction_SummonerFlameSlice&) ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Virtual clone: allocates sizeof(CCreatureAction_SummonerFlameSlice) = 0x11c, null-checks and copy-constructs from
// `*this` through the out-of-line copy constructor @ 0x006F89F0; the constructor's `this` return
// is the result. One of 104 bodies of this shape (the copy constructor's label names the class).
class CCreatureActionBase { public:
    virtual void v00();
};

class CCreatureAction_SummonerFlameSlice : public CCreatureActionBase { public:
    unsigned char _body[0x11c - 4];    // sizeof(CCreatureAction_SummonerFlameSlice) = 0x11c
    CCreatureAction_SummonerFlameSlice(const CCreatureAction_SummonerFlameSlice& other);             // 0x006F89F0
    CCreatureActionBase* Clone();
};

CCreatureActionBase* CCreatureAction_SummonerFlameSlice::Clone()
{
    return new CCreatureAction_SummonerFlameSlice(*this);
}