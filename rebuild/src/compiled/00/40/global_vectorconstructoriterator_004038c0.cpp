// `vector constructor iterator'(void* arr, int size, int count, ctor) -- default-construct `count` elements of `size` bytes in place
// Retail: Fable.exe (TLC, VC7.1) @ 0x004038C0, 48 bytes. Callees: `ctor` (indirect, __fastcall).
//
// The runtime helper the compiler emits for `new T[n]` and for arrays of class-type members: it walks
// the storage element by element, calling the class constructor on each. Nothing in the game calls it
// directly; constructors of objects that contain arrays of non-trivial members do. A zero count is a
// no-op.
//
// Matching: the loop is written as `count-1 >= 0` then a do/while on `m+1` so the compiler emits the
// `dec; js` guard followed by a counted loop rather than a top-tested `for`.
typedef void (__fastcall *VecCtor)(void* self);

void __stdcall vector_constructor_iterator(void* arr, int size, int count, VecCtor ctor)
{
    int m = count - 1;
    if (m >= 0) {
        char* p = (char*)arr;
        int n = m + 1;
        // Construct each element, stepping by the element size.
        do {
            ctor(p);
            p += size;
            --n;
        } while (n != 0);
    }
}
