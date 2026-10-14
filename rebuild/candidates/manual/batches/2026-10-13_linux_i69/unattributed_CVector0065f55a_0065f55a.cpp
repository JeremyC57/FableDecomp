// CVector_0065f55a::CVector_0065f55a(SIterator, SIterator) -- range constructor forwarding both by-value iterators and an input-iterator tag (by const reference, homed in the high byte of the dead second slot) to the __thiscall Construct @ 0x0065E757; frame-pointer size TU
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0065F55A, oracle length 40 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push ebp ; mov ebp,esp ; push esi ; lea eax,[ebp+0Fh] ; push eax ; push ecx ; mov esi,ecx ; mov ecx,[last] ; mov eax,esp ; mov [eax],ecx ; push ecx ; mov ecx,[first] ; mov eax,esp ; mov [eax],ecx ; mov ecx,esi ; call Construct ; mov eax,esi ; pop esi ; pop ebp ; ret 8
// Range constructor of a vector-like container: the two 4-byte iterator classes (user copy
// constructor) are copied into reserved argument slots (`push reg ; mov eax,esp ; mov [eax],value`)
// right to left, the category tag is an empty temporary with a user constructor passed by
// const reference and homed in the high byte of the now-dead `last` slot (`lea eax,[ebp+0Fh]`),
// and the out-of-line Construct @ 0x0065E757 does the work; the constructor returns `this`.
// Size optimisation keeps the `first` load next to its copy (the speed TU hoists it to the
// prologue); the frame pointer comes from the TU's pragma. The manifest label is a mangled
// template-argument fragment and is not used; the element type is opaque. One of 11
// bodies of this shape.
#pragma optimize("y", off)              // frame pointer kept in this TU
#pragma optimize("s", on)

struct SIterator {                         // 4-byte iterator class with a user copy constructor
    void* m_pNode;
    SIterator(const SIterator& other) : m_pNode(other.m_pNode) { }
};

struct SInputIteratorTag { SInputIteratorTag() { } };   // user constructor: no zero-initialisation of the 1-byte temporary

struct CVector_0065f55a {
    void Construct(SIterator first, SIterator last, const SInputIteratorTag& tag);   // 0x0065E757
    CVector_0065f55a(SIterator first, SIterator last);
};

CVector_0065f55a::CVector_0065f55a(SIterator first, SIterator last)
{
    Construct(first, last, SInputIteratorTag());
}
