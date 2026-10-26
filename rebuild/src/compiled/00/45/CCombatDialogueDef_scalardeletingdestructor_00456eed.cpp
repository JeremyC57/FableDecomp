#pragma optimize("s",on)
class CParentDefClassBase {
public:
    virtual ~CParentDefClassBase();   // 0x00430300
};
class CCombatDialogueDef;

void* __fastcall CCombatDialogueDef_scalar_deleting_destructor(CCombatDialogueDef* def, int /*edx*/, unsigned int flags)
{
    ((CParentDefClassBase*)(def))->CParentDefClassBase::~CParentDefClassBase();
    if (flags & 1)
        operator delete((void*)def);
    return def;
}