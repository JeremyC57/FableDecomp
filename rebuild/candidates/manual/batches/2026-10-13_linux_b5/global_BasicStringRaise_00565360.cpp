// STL basic_string exception trampoline (exceptions compiled out): puts("basic_string"); abort()
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00565360, oracle length 18 bytes.
// Grade: candidate -- authored and graded in the Linux lane (Wine + DDK 13.10.4035 cl.exe); the
//        batch README records the lane verdict.
// The manifest label for this body is propagated across byte-identical bodies and is NOT trusted;
// the class is modelled from the bytes only and the name below describes what the code does.
//
// Retail bytes:  push offset "basic_string" ; call puts ; add esp,4 ; jmp abort
// Three byte-identical copies (0x00565360, 0x00565A30, 0x0099C550), one per translation unit that
// instantiated basic_string: with exceptions disabled the library's _THROW expands to printing the
// class name and aborting. `abort` is declared without noreturn so the compiler tail-jumps to it.
extern "C" int  __cdecl puts(const char* text);     // 0x00BFEAF2 (landed-trusted)
extern "C" void __cdecl abort();                     // 0x00BFEAEC

void BasicStringRaise()
{
    puts("basic_string");
    abort();
}
