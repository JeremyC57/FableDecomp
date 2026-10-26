class CDefClassBase {
public:
    CDefClassBase& operator=(const CDefClassBase& other);   // 0x00431F10
};
struct CSub { void Copy(const CSub* o); };
class CTextureReplacementDef { public:
    char base[0x28];
    CSub sub;
    void CTextureReplacementDef_Copy(const CTextureReplacementDef* o);
};

void CTextureReplacementDef::CTextureReplacementDef_Copy(const CTextureReplacementDef* o)
{
    ((CDefClassBase*)(this))->operator=(*(const CDefClassBase*)(o));
    this->sub.Copy(&o->sub);
}