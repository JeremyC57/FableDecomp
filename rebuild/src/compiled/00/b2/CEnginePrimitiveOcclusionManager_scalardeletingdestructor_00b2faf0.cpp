class CEngineComponent {
public:
    virtual ~CEngineComponent();   // 0x00B59730
};
class CEnginePrimitiveOcclusionManager { public:
    void* vftable;
};

void operator delete(void*);

void* __fastcall CEnginePrimitiveOcclusionManager_scalar_deleting_destructor(CEnginePrimitiveOcclusionManager* manager, int edx_unused, unsigned int flags)
{
    ((CEngineComponent*)(manager))->CEngineComponent::~CEngineComponent();
    if (flags & 1)
        operator delete(manager);
    return manager;
}