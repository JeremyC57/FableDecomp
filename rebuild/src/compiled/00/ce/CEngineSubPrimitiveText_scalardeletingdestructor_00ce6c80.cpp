#pragma optimize("t", on)
// Leaf: `scalar_deleting_destructor'
class CCharString { public:
    ~CCharString();   // 0x0099EAE0 (Ego_r.pdb ??1CCharString@@QAE@XZ)
};
extern void __fastcall AutoTinyCompositeDestructor_00cbd510(void* self);
void* __fastcall CEngineSubPrimitiveText_CompositeScalarDeletingDestructor(void* self, int, unsigned int flags)
{
    ((CCharString*)(static_cast<unsigned char*>(self) + 72))->~CCharString();
    AutoTinyCompositeDestructor_00cbd510(self);
    if (flags & 1)
        operator delete(self);
    return self;
}