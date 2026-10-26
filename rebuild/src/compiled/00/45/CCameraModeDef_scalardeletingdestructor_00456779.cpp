#pragma optimize("s",on)
class CParentDefClassBase {
public:
    virtual ~CParentDefClassBase();   // 0x00430300
};
class CCameraModeDef { public:
    int f0;
    void* vf;
};
void* __fastcall CCameraModeDef_scalar_deleting_destructor(CCameraModeDef* mode, int edx_unused, unsigned int flags)
{
    ((CParentDefClassBase*)(mode))->CParentDefClassBase::~CParentDefClassBase();
    if (flags & 1)
        operator delete(mode);
    return mode;
}