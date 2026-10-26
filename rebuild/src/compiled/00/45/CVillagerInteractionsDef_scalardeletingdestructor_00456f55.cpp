#pragma optimize("s",on)
class CParentDefClassBase {
public:
    virtual ~CParentDefClassBase();   // 0x00430300
};
class CVillagerInteractionsDef { public:
    int f0;
};


void* __fastcall CVillagerInteractionsDef_scalar_deleting_destructor(CVillagerInteractionsDef* def, int edx, unsigned int flags) {
    (void)edx;
    ((CParentDefClassBase*)(def))->CParentDefClassBase::~CParentDefClassBase();
    if (flags & 1) {
        operator delete(def);
    }
    return def;
}