// CIdleSchedulerDef::Copy @ 0x004e60ee
// this-call member. Base copy then member-at-0x3c copy.
// Model callees as real thiscall member methods so VC7.1 emits ecx+stack (no edx).

struct CDefClassBase {
    void Copy_00431ed0(const CDefClassBase* src);   // 0x431ed0 (declared extern, defined elsewhere)
};

struct SubMember {
    void Copy_004e610d(const SubMember* src);       // 0x4e610d
    char pad[0x20];
};

struct CIdleSchedulerDef : public CDefClassBase {
    char basepad[0x3c];
    SubMember member;                       // at +0x3c
    void Copy(const CDefClassBase* src);
};

void CIdleSchedulerDef::Copy(const CDefClassBase* src)
{
    CDefClassBase::Copy_00431ed0(src);
    this->member.Copy_004e610d(&((const CIdleSchedulerDef*)src)->member);
}