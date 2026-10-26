// CAnimatingObjectDef::Copy 0x004ebbf2 reconstruction
class CDefClassBase;

struct SubMember {
    char pad[4];
    void Copy(const SubMember* src);
};

class CDefClassBase { public:
    char pad[0x28];
    SubMember member;
};

class CAnimatingObjectDef : public CDefClassBase { public:
    void CopyBase(const CDefClassBase* src);
    void Copy(const CDefClassBase* src);
};

void CAnimatingObjectDef::Copy(const CDefClassBase* src)
{
    CopyBase(src);
    this->member.Copy(&src->member);
}