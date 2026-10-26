class CDynamicObjectHeapBase {
public:
    virtual ~CDynamicObjectHeapBase();   // 0x00F36C90
};
class CInputProcessManager { public: void* vtbl; };
// Leaf: ??1CInputProcessManager@

extern "C" void __fastcall inner_dtor_00b48e10(CInputProcessManager* self);

void __fastcall DtorIPM_F(CInputProcessManager* self)
{
    self->vtbl = (void*)0x12a20fc;
    inner_dtor_00b48e10(self);
    ((CDynamicObjectHeapBase*)(self))->CDynamicObjectHeapBase::~CDynamicObjectHeapBase();
}