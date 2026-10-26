class CDynamicObjectHeapBase {
public:
    virtual ~CDynamicObjectHeapBase();   // 0x00F36C90
};
class CInputProcessManager { public:
// Leaf: ??1CInputProcessManager@
    void* vptr;
};

extern void __fastcall Helper_af8780(CInputProcessManager* self);

void __fastcall DtorIPM_A(CInputProcessManager* self)
{
    self->vptr = (void*)0x0129F638;
    Helper_af8780(self);
    ((CDynamicObjectHeapBase*)(self))->CDynamicObjectHeapBase::~CDynamicObjectHeapBase();
}