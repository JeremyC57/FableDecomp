#pragma optimize("t", on)
// Leaf: `scalar_deleting_destructor'
class CTimer { public:
    ~CTimer();   // 0x00CD4470 (Ego_r.pdb ??1CTimer@@QAE@XZ)
};
extern void __fastcall AutoTinyCompositeDestructor_00cbd510(void* self);
void* __fastcall CEngineSubPrimitiveText_CompositeScalarDeletingDestructor(void* self, int, unsigned int flags)
{
    ((CTimer*)(static_cast<unsigned char*>(self) + 84))->~CTimer();
    AutoTinyCompositeDestructor_00cbd510(self);
    if (flags & 1)
        operator delete(self);
    return self;
}