// CSkeletalMorphDef::Copy 0x004e28b6
class CDefClassBase { public:
    CDefClassBase& operator=(const CDefClassBase& other);   // 0x431f10, __fastcall(this, src)
};
struct Sub {
    void Copy(const void* src);   // 0x46078c, __fastcall(this, src)
};

class CSkeletalMorphDef { public:
    char pad[0x28];
    Sub sub;
};

void __fastcall CSkeletalMorphDef_Copy(CSkeletalMorphDef* self, void* edx, const char* src)
{
    ((CDefClassBase*)self)->operator=(*(const CDefClassBase*)(src));
    self->sub.Copy(src + 0x28);
}