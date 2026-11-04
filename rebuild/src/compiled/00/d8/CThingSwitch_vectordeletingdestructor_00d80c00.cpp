#pragma optimize("t", on)
// Leaf: ?CThingSwitch_CompositeScalarDeletingDestructor@@YIPAXPAXHI@Z
extern void __fastcall AutoTinyFirstMemberDestructor_00cdb960(void* member);
class CTimer { public:
    ~CTimer();   // 0x00CD4470 (Ego_r.pdb ??1CTimer@@QAE@XZ)
};
extern void __fastcall AutoTinyCompositeDestructor_00cbd510(void* self);
void* __fastcall CThingSwitch_CompositeScalarDeletingDestructor(void* self, int, unsigned int flags)
{
    AutoTinyFirstMemberDestructor_00cdb960(static_cast<unsigned char*>(self) + 92);
    ((CTimer*)(static_cast<unsigned char*>(self) + 88))->~CTimer();
    AutoTinyCompositeDestructor_00cbd510(self);
    if (flags & 1)
        operator delete(self);
    return self;
}