class CDynamicObjectHeapBase {
public:
    virtual ~CDynamicObjectHeapBase();   // 0x00F36C90
};
class CInputProcessManager { public:
// Leaf: ?DtorIPM_B@@YIXPAVCInputProcessManager@@@Z
    void* vptr;
};

extern void __fastcall SubCleanup(CInputProcessManager* self);  // 0xaf8860

void __fastcall DtorIPM_B(CInputProcessManager* self)
{
    self->vptr = (void*)0x0129f644;
    SubCleanup(self);
    ((CDynamicObjectHeapBase*)(self))->CDynamicObjectHeapBase::~CDynamicObjectHeapBase();
}