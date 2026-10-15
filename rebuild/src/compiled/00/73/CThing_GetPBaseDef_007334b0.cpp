// CThing::GetPBaseDef() -- return the owner's counted def pointer (+0x48) by value: copy the pointer into the hidden result and add a reference
// Retail: Fable.exe (TLC, VC7.1) @ 0x007334B0, 19 bytes. No callees.
// Identity: the catalog label is propagated over byte-identical "return a CDefPointer member" bodies; the retail
// CThing keeps PBaseDef at +0x70 (see 0x004C7B80), so this +0x48 getter belongs to another def-owning class. The
// owner is modelled locally with a descriptive name for the member.
// Shape: CDefPointer<T> has a user copy constructor, so the result goes through the hidden pointer ([esp+4]);
// the inline copy constructor stores the pointer first and then bumps the def's reference count at +4 when it is
// non-null (`mov [eax],ecx ; je ; inc dword ptr [ecx+4]`).
struct CDefPointee {
    void* __vftable;                                 // +0x00
    long  m_refCount;                                // +0x04
};

template <class T> struct CDefPointer {
    T* m_p;
    CDefPointer(const CDefPointer& other) : m_p(other.m_p)
    {
        if (m_p)
            ++m_p->m_refCount;
    }
};

struct CThing {
    unsigned char             _pad00[0x48];
    CDefPointer<CDefPointee>  PBaseDef;              // +0x48
    CDefPointer<CDefPointee>  GetPBaseDef() const;
};

CDefPointer<CDefPointee> CThing::GetPBaseDef() const
{
    return PBaseDef;
}