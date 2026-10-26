class CBaseClass {
public:
    virtual ~CBaseClass();   // 0x0099A2E0
};
class C3DPositionInterpolator { public:
    void* vftable;
};

void operator delete(void*);

void* __fastcall C3DPositionInterpolator_scalar_deleting_destructor(C3DPositionInterpolator* interpolator, int edx_unused, unsigned int flags)
{
    ((CBaseClass*)(interpolator))->CBaseClass::~CBaseClass();
    if (flags & 1)
        operator delete(interpolator);
    return interpolator;
}