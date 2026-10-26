#pragma optimize("s",on)
// MSVC vector deleting destructor for CSpecialAbilitiesDrunkennessDef
class CParentDefClassBase {
public:
    virtual ~CParentDefClassBase();   // 0x00430300
};
class CSpecialAbilitiesDrunkennessDef { public:
    void* vftable;
};


void* __fastcall CSpecialAbilitiesDrunkennessDef_scalar_deleting_destructor(CSpecialAbilitiesDrunkennessDef* def, int /*edx*/, unsigned int flags)
{
    ((CParentDefClassBase*)(def))->CParentDefClassBase::~CParentDefClassBase();
    if (flags & 1)
        operator delete(def);
    return def;
}