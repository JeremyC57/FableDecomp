#pragma optimize("t", on)
// Leaf: `vector_deleting_destructor'
class CTimer { public:
    ~CTimer();   // 0x00CD4470 (Ego_r.pdb ??1CTimer@@QAE@XZ)
};
extern void __fastcall AutoTinySecondMemberDestructor_009ac310(void* member);
extern void __fastcall AutoTinyCompositeDestructor_00cbd510(void* self);
void* __fastcall CThingSwitch_CompositeScalarDeletingDestructor(void* self, int, unsigned int flags)
{
    ((CTimer*)(static_cast<unsigned char*>(self) + 104))->~CTimer();
    AutoTinySecondMemberDestructor_009ac310(static_cast<unsigned char*>(self) + 76);
    AutoTinyCompositeDestructor_00cbd510(self);
    if (flags & 1)
        operator delete(self);
    return self;
}