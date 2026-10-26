class CDefinitionManager {
public:
    virtual ~CDefinitionManager();   // 0x009B02E0
};
extern "C" void __fastcall MemberDtor_686df0_00cbeeb0(void* self);
// Leaf: ??1CInputProcessManager@

class CInputProcessManager { public:
    void** vtbl;
};

extern const void* const g_vtbl_CInputProcessManager_012c2648[];

extern "C" void __fastcall DtorInputProcessManager(CInputProcessManager* self, int /*edx*/)
{
    self->vtbl = (void**)&g_vtbl_CInputProcessManager_012c2648;
    MemberDtor_686df0_00cbeeb0(self);
    ((CDefinitionManager*)(self))->CDefinitionManager::~CDefinitionManager();
}