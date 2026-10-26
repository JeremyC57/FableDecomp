#pragma optimize("t", on)
// Leaf: `scalar_deleting_destructor'
class CBaseClass {
public:
    virtual ~CBaseClass();   // 0x0099A2E0
};
class CEngineInternalPrimitiveMeshBase {
public:
    virtual ~CEngineInternalPrimitiveMeshBase();   // 0x00BEBC50
};
void* __fastcall CEngineSubPrimitiveText_CompositeScalarDeletingDestructor(void* self, int, unsigned int flags)
{
    ((CBaseClass*)(static_cast<unsigned char*>(self) + 212))->CBaseClass::~CBaseClass();
    ((CEngineInternalPrimitiveMeshBase*)(self))->CEngineInternalPrimitiveMeshBase::~CEngineInternalPrimitiveMeshBase();
    if (flags & 1)
        operator delete(self);
    return self;
}