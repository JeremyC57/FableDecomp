class CTCFamilyMember { public:
    void* vftable;
};

void __fastcall CTCFamilyMember_dtor(CTCFamilyMember* member);
void operator delete(void*);

void* __fastcall CTCFamilyMember_scalar_deleting_destructor(CTCFamilyMember* member, int edx_unused, unsigned int flags)
{
    CTCFamilyMember_dtor(member);
    if (flags & 1)
        operator delete(member);
    return member;
}