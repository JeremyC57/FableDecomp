#pragma optimize("s",on)
class CParentDefClassBase {
public:
    virtual ~CParentDefClassBase();   // 0x00430300
};
class CSpecialAbilitiesGhostSwordDef { public: void* vftable; };

void* __fastcall CSpecialAbilitiesGhostSwordDef_scalar_deleting_destructor(CSpecialAbilitiesGhostSwordDef* def, int edx, unsigned int flags) {
    (void)edx;
    ((CParentDefClassBase*)(def))->CParentDefClassBase::~CParentDefClassBase();
    if (flags & 1) {
        operator delete(def);
    }
    return def;
}