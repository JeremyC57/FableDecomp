// CCreatureAction_ShowToSheatheTrophy::Clone() -- new CCreatureAction_ShowToSheatheTrophy(*this) with the implicit copy constructor inlined
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006DFCB0, oracle length 47 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push edi ; push 0xc0 ; mov edi,ecx ; call operator new ; mov esi,eax ; add esp,4 ; test esi,esi ; je N ; push edi ; mov ecx,esi ; call CCreatureAction_ShowTrophy::CCreatureAction_ShowTrophy(const CCreatureAction_ShowTrophy&) ; pop edi ; mov dword ptr [esi],offset CCreatureAction_ShowToSheatheTrophy::`vftable' ; mov eax,esi ; pop esi ; ret ; N: pop edi ; xor eax,eax ; pop esi ; ret
// Virtual clone of a class that adds no data to its base: the implicit copy constructor is
// inlined as the base copy constructor @ 0x006DFB90 plus this class's vtable store
// (0x0125F9F4, named through rtti-vtables.tsv). sizeof = 0xc0. One of 196 bodies of this shape.
struct CCreatureAction_ShowTrophy {
    virtual void v00();
    unsigned char _body[0xc0 - 4];    // sizeof(CCreatureAction_ShowTrophy) = sizeof(CCreatureAction_ShowToSheatheTrophy) = 0xc0
    CCreatureAction_ShowTrophy(const CCreatureAction_ShowTrophy& other);     // 0x006DFB90
};

struct CCreatureAction_ShowToSheatheTrophy : CCreatureAction_ShowTrophy {
    virtual void v00();                    // own vtable; copy constructor implicit
    CCreatureAction_ShowTrophy* Clone();
};

CCreatureAction_ShowTrophy* CCreatureAction_ShowToSheatheTrophy::Clone()
{
    return new CCreatureAction_ShowToSheatheTrophy(*this);
}