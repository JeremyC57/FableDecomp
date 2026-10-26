#pragma optimize("s",on)
class CParentDefClassBase {
public:
    virtual ~CParentDefClassBase();   // 0x00430300
};
class CInventoryCategoryDef { public: int f0; };

void* __fastcall CInventoryCategoryDef_scalar_deleting_destructor(CInventoryCategoryDef* def, int /*edx*/, unsigned int flags)
{
    ((CParentDefClassBase*)(def))->CParentDefClassBase::~CParentDefClassBase();
    if (flags & 1)
        operator delete(def);
    return def;
}