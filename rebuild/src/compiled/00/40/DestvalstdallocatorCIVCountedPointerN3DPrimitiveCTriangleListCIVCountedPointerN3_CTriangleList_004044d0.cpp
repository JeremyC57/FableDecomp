// CIVCountedPointer<T>::Release() -- drop our reference to the intrusively counted object and empty the pointer
// Retail: Fable.exe (TLC, VC7.1) @ 0x004044D0, 27 bytes. Callees: vtable slot 1 of the pointee (its self-delete), indirect.
//
// CIVCountedPointer holds a single word: an object that carries its own reference count right after
// its vtable. Release() is what the destructor and reassignment use: decrement the object's count and,
// if we were the last holder, have the object delete itself through vtable slot 1, then null our word.
// An already-empty pointer does nothing. Named for the N3DPrimitive::CTriangleList instantiation but
// shared by every CIVCountedPointer<T>.
//
// Matching: `this` in ecx, no arguments; `--cur->rc == 0` folds to `dec [ecx+4]; jne`; the virtual
// call is `mov eax,[ecx]; call [eax+4]` with the object in ecx.
// Intrusively counted object: vtable, then the count.
struct Obj {
    virtual void v0();
    virtual void v1();      // slot 1: delete yourself
    long rc;                // reference count
};
struct CIVCP {
    Obj* p;                 // the shared object, or null
    void Release();
};
void CIVCP::Release() {
    Obj* cur = this->p;
    if (cur) {
        // Last holder: the object deletes itself.
        if (--cur->rc == 0) cur->v1();
        this->p = 0;
    }
}
