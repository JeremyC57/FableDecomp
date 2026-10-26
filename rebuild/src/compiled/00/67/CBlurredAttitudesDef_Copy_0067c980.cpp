// CBlurredAttitudesDef::Copy(const CDefClassBase* source) -- the virtual def copy: memberwise assignment from the source cast to this class
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0067C980, length 61 bytes.
// Callees: CBase_00431f10::operator= @ 0x00431F10 (the def base), retail vector<long>::operator=
// @ 0x00431F90 on the packed member at +0x29, CSubObject_00471e44::operator= @ 0x00471E44 on +0x3c.
// Same body as operator= @ 0x0067C490 without the `mov eax,esi`: `*this = *source` with the
// class's own inline operator= expanded in place (an implicit one is emitted as a COMDAT and
// tail-jumped to, not inlined) -- the void virtual Copy does not return `this`. Layout as in
// 0067c490 (vtable 0x01243A44 = CBlurredAttitudesDef; the manifest label CBlurredAttitudesDef is propagated):
// +0x25 long, then the vector (+0x29) and its flag byte (+0x36) in one sub-object whose inline
// assignment gives retail's `lea ebx/ebp` addressing, +0x3c a sub-object with its own operator=.
#pragma pack(push, 1)
struct CBase_00431f10 {
    unsigned char _body[0x25];
    CBase_00431f10& operator=(const CBase_00431f10& other);   // 0x00431F10
};

struct CRetailVector_long {                     // VC6-era Dinkumware vector: first / last / end
    long* first;
    long* last;
    long* end;
    CRetailVector_long& operator=(const CRetailVector_long& other);   // 0x00431F90
};

struct SAttitudeBlock {                        // +0x29..+0x36: the vector and its flag share one sub-object
    CRetailVector_long m_attitudes;             // +0x00 (this+0x29)
    unsigned char      m_pad0c;                 // +0x0c (not copied)
    unsigned char      m_flag;                  // +0x0d (this+0x36)
    SAttitudeBlock& operator=(const SAttitudeBlock& other)
    {
        m_attitudes = other.m_attitudes;
        m_flag = other.m_flag;
        return *this;
    }
};

struct CSubObject_00471e44 {
    unsigned char _body[0x0c];
    CSubObject_00471e44& operator=(const CSubObject_00471e44& other);   // 0x00471E44
};

class CDefClassBase;

class CBlurredAttitudesDef : public CBase_00431f10 { public:
    long                m_id;                   // +0x25
    SAttitudeBlock      m_block;                // +0x29 (vector + flag)
    unsigned char       m_pad37[5];             // +0x37..+0x3b
    CSubObject_00471e44 m_tail;                 // +0x3c
    void Copy(const CDefClassBase* source);
    CBlurredAttitudesDef& operator=(const CBlurredAttitudesDef& other)   // inline: expanded into Copy (the out-of-line copy is @ 0x0067C490)
    {
        CBase_00431f10::operator=(other);
        m_id = other.m_id;
        m_block = other.m_block;
        m_tail = other.m_tail;
        return *this;
    }
};
#pragma pack(pop)

void CBlurredAttitudesDef::Copy(const CDefClassBase* source)
{
    *this = *reinterpret_cast<const CBlurredAttitudesDef*>(source);
}