// CThingFilter_IsValid::operator()() -- no-argument forwarder to the object's own virtual slot 1 (tail jump); returns its pointer result
// Retail: Fable.exe (TLC, VC7.1) @ 0x0043E430, 5 bytes. Callees: virtual slot 1 (+4) of `this`.
// Shape: `return Slot01();` through the vtable becomes `mov eax,[ecx] ; jmp [eax+4]`.
// Label note: the propagated label says operator()(CThing*) returning bool, but every caller (0xC09D25,
// 0xC09D30, 0xC09D55 ... sort comparators) calls it with ecx only, no stack argument, and dereferences
// the returned eax as a pointer; 0x004382CD also passes its address as a callback. It is an ICF-folded
// "call virtual slot 1" body shared by several classes.
struct CThingFilter_IsValid {
    virtual void  Slot00();
    virtual long* Slot01();     // slot 1
    long* operator()();
};

long* CThingFilter_IsValid::operator()()
{
    return Slot01();
}
