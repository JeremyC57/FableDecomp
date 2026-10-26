class CDynamicObjectHeapBase {
public:
    virtual ~CDynamicObjectHeapBase();   // 0x00F36C90
};
extern "C" void __fastcall MemberDtor_686df0_00af8bc0(void* self);
// Leaf: ??1CInputProcessManager@

class CInputProcessManager { public:
    void** vtbl;
};

extern const void* const g_vtbl_CInputProcessManager_0129f650[];

extern "C" void __fastcall DtorInputProcessManager(CInputProcessManager* self, int /*edx*/)
{
    self->vtbl = (void**)&g_vtbl_CInputProcessManager_0129f650;
    MemberDtor_686df0_00af8bc0(self);
    ((CDynamicObjectHeapBase*)(self))->CDynamicObjectHeapBase::~CDynamicObjectHeapBase();
}