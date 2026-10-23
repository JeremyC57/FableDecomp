// CVtable_012b5868::`scalar deleting destructor'(unsigned int) -- empty virtual destructor, this copy from one translation unit
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C3C840, oracle length 31 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  test byte ptr [esp+4],1 ; push esi ; mov esi,ecx ; mov dword ptr [esi],offset CVtable_012b5868::`vftable' ; je K ; push esi ; call operator delete ; add esp,4 ; K: mov eax,esi ; pop esi ; ret 4
// The compiler-generated deleting destructor for an empty inline `virtual ~CVtable_012b5868() {}`: it
// restores the class's vtable (0x012B5868, named through rtti-vtables.tsv), frees the object when
// the flag's bit 0 is set (operator delete @ 0x00BFE9BC, landed-trusted) and returns `this`. The
// inline destructor is re-emitted in every translation unit that needs it, which is why 72
// byte-identical copies exist; this file is the copy at this address.
struct CVtable_012b5868 {
    virtual ~CVtable_012b5868() { }
    // Identity: this copy's deleting destructor frees through 0x00C3C370 (labelled AudioFree, the
    // allocator twin of 0x00C3C360), not the global operator delete @ 0x00BFE9BC: a class-level
    // operator delete, defined out of line.
    static void operator delete(void* p);   // 0x00C3C370
};

// VC7.1 emits a class's vtable (and the deleting destructor it references) in every translation
// unit that constructs the object; this helper is that construction, nothing more.
void* MakeOne()
{
    return new CVtable_012b5868;
}