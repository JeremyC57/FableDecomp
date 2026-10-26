// CReplaceableMeshDef::Copy  0x004e5f51
// void __fastcall CReplaceableMeshDef::Copy(CReplaceableMeshDef* this, CDefClassBase const* src)


// base class (its Copy is 0x431f10), src passed on stack (native __fastcall)
class CDefClassBase { public:
    CDefClassBase& operator=(const CDefClassBase& other);   // 0x00431F10
};

// member at +0x28 (its Copy is 0x4e5f70), src passed on stack (native __fastcall)
struct Member28 {
    void Copy_004e5f70(Member28 const* src);
};

class CReplaceableMeshDef : public CDefClassBase { public:
    // base occupies [0,0x28); member at +0x28
    char basePad[0x28];
    Member28 member; // at +0x28
};

void __fastcall CReplaceableMeshDef_Copy(CReplaceableMeshDef* self, void* /*edx*/, CDefClassBase const* src)
{
    self->CDefClassBase::operator=(*(const CDefClassBase*)(src));
    self->member.Copy_004e5f70((Member28 const*)((char const*)src + 0x28));
}