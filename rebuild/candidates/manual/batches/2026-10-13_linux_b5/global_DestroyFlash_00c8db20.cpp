// std::_Destroy<NCreatureHitNotification::CFlash> (allocator destroy helper)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C8DB20, oracle length 12 bytes.
// Grade: candidate -- authored and graded in the Linux lane (Wine + DDK 13.10.4035 cl.exe); the
//        batch README records the lane verdict.
//
// Retail bytes:  mov eax,ecx ; push 0 ; mov ecx,edx ; call CFlash::`scalar deleting destructor' ; ret
// __fastcall(allocator&, CFlash*): the allocator (ecx) is unused, the element (edx) has its
// destructor run in place -- the compiler reaches the destructor through the scalar-deleting
// entry with flag 0 (no free), which is how VC7.1 emits a qualified destructor call on a
// polymorphic class.
struct CFlash {
    virtual ~CFlash();                      // scalar deleting destructor @ 0x00C8D9D0
};
struct CFlashAllocator {};

void __fastcall DestroyFlash(CFlashAllocator& allocator, CFlash* element)
{
    element->CFlash::~CFlash();
}
