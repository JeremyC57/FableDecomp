#pragma optimize("s",on)
class CParentDefClassBase {
public:
    virtual ~CParentDefClassBase();   // 0x00430300
};
class CHeroPostcardGeneratorDef { public:
    int f0;
    int Parent;
    void* f8;
    char inner[4];
    unsigned char _pad_0x10[0xa8];
    void* tex;
};


void* __fastcall CHeroPostcardGeneratorDef_scalar_deleting_destructor(CHeroPostcardGeneratorDef* def, int edx, unsigned int flags)
{
    (void)edx;
    ((CParentDefClassBase*)(def))->CParentDefClassBase::~CParentDefClassBase();
    if (flags & 1)
        operator delete(def);
    return def;
}