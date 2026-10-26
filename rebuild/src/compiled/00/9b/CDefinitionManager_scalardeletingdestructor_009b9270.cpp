class CDefinitionManager { public:
    void* vftable;

public:
    virtual ~CDefinitionManager();   // 0x009B02E0
};

void operator delete(void*);

void* __fastcall CDefinitionManager_scalar_deleting_destructor(CDefinitionManager* manager, int edx_unused, unsigned int flags)
{
    ((CDefinitionManager*)(manager))->CDefinitionManager::~CDefinitionManager();
    if (flags & 1)
        operator delete(manager);
    return manager;
}