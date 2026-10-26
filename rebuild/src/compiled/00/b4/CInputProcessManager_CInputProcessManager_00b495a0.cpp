class CDynamicObjectHeapBase {
public:
    virtual ~CDynamicObjectHeapBase();   // 0x00F36C90
};
class CInputProcessManager { public:
// Leaf: ??1CInputProcessManager@
    void* vtbl;
};

extern int __fastcall sub_b48c70(CInputProcessManager* self);

void __fastcall DtorIPM_D(CInputProcessManager* self)
{
    self->vtbl = (void*)0x012a20e4;
    sub_b48c70(self);
    ((CDynamicObjectHeapBase*)(self))->CDynamicObjectHeapBase::~CDynamicObjectHeapBase();
}