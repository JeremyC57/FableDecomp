// CDefRecordVector::Resize(unsigned long) -- STLport vector<T>::resize(n) for a 0x34-byte def record: forwards to resize(n, T()) with a default-constructed temporary
// Leaf: Resize
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F33D90, 44 bytes (+4 int3 padding).
// Callees: 0x00F33DC0 record constructor, 0x00F33DE0 vector::resize(size_type, const T&),
// 0x00D34F80 record destructor.
//
// The manifest label `CPersistContext::Transfer<CAnimationSet>` is propagated and wrong. Called
// from 0x00F33C93 with a count, this is the one-argument STLport `resize(size_type n)
// { resize(n, T()); }` emitted out of line (speed TU: the count is reloaded into eax and pushed).
// The element is a 0x34-byte def record: its constructor runs CDefClassBase's constructor
// (0x0042D9E0, vtable 0x01230BCC), stores its own vtable (0x012CCC50) and clears a three-pointer
// vector at +0x28; its out-of-line destructor frees that buffer and finishes in the
// CDefPointeeBase / CResource destructors. The temporary is the constructor's returned `this`
// (`push eax`); no EH frame (module built without C++ exceptions).
struct CDefRecord34 {
    unsigned char m_body[0x34];
    CDefRecord34();                        // 0x00F33DC0
    ~CDefRecord34();                       // 0x00D34F80
};

struct CDefRecordVector {
    CDefRecord34* m_pFirst;
    CDefRecord34* m_pLast;
    CDefRecord34* m_pEnd;
    void Resize(unsigned long count, const CDefRecord34& value);   // 0x00F33DE0
    void Resize(unsigned long count);
};

void CDefRecordVector::Resize(unsigned long count)
{
    Resize(count, CDefRecord34());
}