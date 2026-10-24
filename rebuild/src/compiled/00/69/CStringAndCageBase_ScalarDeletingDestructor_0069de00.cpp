// CStringAndCageBase::`scalar deleting destructor'(unsigned int) -- empty virtual destructor over CBase_0056e0e0::~CBase_0056e0e0, this copy from one translation unit
// Leaf: ScalarDeletingDestructor
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0069DE00, oracle length 36 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; mov dword ptr [esi],offset CStringAndCageBase::`vftable' ; call CBase_0056e0e0::~CBase_0056e0e0 ; test byte ptr [esp+8],1 ; je K ; push esi ; call operator delete ; add esp,4 ; K: mov eax,esi ; pop esi ; ret 4
// Compiler-generated deleting destructor for an empty inline `virtual ~CStringAndCageBase() {}`: installs this
// class's vtable (0x0125C37C, rtti-vtables.tsv), runs the base destructor @ 0x0056E0E0 (its
// manifest label is kept only when unique) and frees the object when bit 0 of the flag is set
// (operator delete @ 0x00BFE9BC, landed-trusted). Re-emitted per translation unit; this file is the
// copy at this address. One of 12 bodies of this shape.
struct CBase_0056e0e0 {
    virtual void v00();
    ~CBase_0056e0e0();                          // 0x0056E0E0
};

struct CStringAndCageBase : CBase_0056e0e0 {
    virtual ~CStringAndCageBase() { }
};

// VC7.1 emits a class's vtable (and the deleting destructor it references) in every translation
// unit that constructs the object; this helper is that construction, nothing more.
void* MakeOne()
{
    return new CStringAndCageBase;
}