// CThingRefVector::Resize(unsigned long) -- STLport vector<T>::resize(n): forwards to resize(n, T()) with a default-constructed 16-byte element temporary
// Leaf: Resize
#pragma optimize("y", off)
#pragma optimize("s", on)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CD3DA4, 41 bytes. Pragmas: frame pointer kept
// (`optimize("y",off)`) in a size-optimised TU (`push dword ptr [ebp+8]`, `leave`).
// Callees: 0x007E72A0 element constructor, 0x00CD3CCB vector::resize(size_type, const T&),
// 0x007E74D0 element destructor.
//
// The manifest label `CPersistContext::Transfer<CAnimationSet>` is propagated and wrong. The three
// callers (0x00CCC73B, 0x00CCCA4F, 0x00CCD1C6) zero a three-pointer local (first/last/end) and call
// this with a count: it is the one-argument STLport `resize(size_type n) { resize(n, T()); }`,
// emitted out of line. The two-argument resize @ 0x00CD3CCB is the STLport body
// (`n < size() ? erase(begin()+n, end()) : insert(end(), n-size(), x)`, 16-byte elements).
// The element is a 16-byte intelligent-pointer record: its constructor runs the
// CBaseIntelligentPointer constructor (0x0099A380, vtable 0x0129A7D8), stores its own vtable
// (0x0127094C) and clears +0x8/+0xC; its destructor releases the counted block at +0xC. The
// temporary is passed as the constructor's returned `this` (`push eax`); the module is built
// without C++ exceptions (no EH frame around the temporary).
struct CThingRef {
    void*         m_vptr;                  // +0x0
    void*         m_pData;                 // +0x4  (CBaseIntelligentPointer::PData)
    void*         m_pObject;               // +0x8
    unsigned long* m_pCount;               // +0xC
    CThingRef();                           // 0x007E72A0
    ~CThingRef();                          // 0x007E74D0
};

struct CThingRefVector {
    CThingRef* m_pFirst;
    CThingRef* m_pLast;
    CThingRef* m_pEnd;
    void Resize(unsigned long count, const CThingRef& value);   // 0x00CD3CCB
    void Resize(unsigned long count);
};

void CThingRefVector::Resize(unsigned long count)
{
    Resize(count, CThingRef());
}
