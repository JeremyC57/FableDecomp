// CRangeString_0065fa5e::Construct(SIterator, SIterator, void*) -- forwards to the __fastcall range initialiser @ 0x0065EBE4 with an empty tag in edx, the two 4-byte iterator classes copied by value into reserved slots; returns this
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0065FA5E, oracle length 42 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push ebp ; mov ebp,esp ; push esi ; push [ebp+10h] ; mov esi,ecx ; push ecx ; mov ecx,[ebp+0Ch] ; mov eax,esp ; mov [eax],ecx ; push ecx ; mov ecx,[ebp+8] ; mov eax,esp ; mov [eax],ecx ; lea edx,[ebp+13h] ; mov ecx,esi ; call RangeInitialize ; mov eax,esi ; pop esi ; pop ebp ; ret 0Ch
// STLport-style string range constructor forwarder: the two iterators are 4-byte classes with a
// user copy constructor, so each by-value argument is passed by reserving its slot (`push ecx`)
// and constructing the copy in place (`mov eax,esp ; mov [eax],ecx`); the helper @ 0x0065EBE4
// is a __fastcall member taking the empty dispatch tag by reference in edx (homed in the high
// byte of a dead argument slot) and returning nothing used -- `this` is returned. The frame
// pointer comes from this translation unit's `optimize("y", off)`. The manifest label
// (`CPersistTraits<C2DVector>::TransferIn`) is propagated across the family. One of 9 bodies.
#pragma optimize("y", off)              // frame pointer kept in this TU

struct SScalarTag { SScalarTag() { } };   // user constructor: no zero-initialisation of the 1-byte temporary

struct SIterator {                          // 4-byte iterator class with a user copy constructor
    const unsigned short* m_pCurrent;    // wide character (wchar_t is a typedef under VC7.1 defaults)
    SIterator(const SIterator& other) : m_pCurrent(other.m_pCurrent) { }
};

struct CRangeString_0065fa5e {
    void* m_pData;
    void __fastcall RangeInitialize(const SScalarTag& tag, SIterator first, SIterator last, void* allocator);   // 0x0065EBE4
    CRangeString_0065fa5e* Construct_CRangeString_0065fa5e(SIterator first, SIterator last, void* allocator);
};

CRangeString_0065fa5e* CRangeString_0065fa5e::Construct_CRangeString_0065fa5e(SIterator first, SIterator last, void* allocator)
{
    RangeInitialize(SScalarTag(), first, last, allocator);
    return this;
}
