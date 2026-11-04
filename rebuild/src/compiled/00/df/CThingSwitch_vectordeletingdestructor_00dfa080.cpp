#pragma optimize("t", on)
// Leaf: ?CThingSwitch_CompositeScalarDeletingDestructor@@YIPAXPAXHI@Z
class CTimer { public:
    ~CTimer();   // 0x00CD4470 (Ego_r.pdb ??1CTimer@@QAE@XZ)
};
extern void __fastcall AutoTinySecondMemberDestructor_008ac970(void* member);
extern void __fastcall AutoTinyCompositeDestructor_00cbd510(void* self);
void* __fastcall CThingSwitch_CompositeScalarDeletingDestructor(void* self, int, unsigned int flags)
{
    ((CTimer*)(static_cast<unsigned char*>(self) + 108))->~CTimer();
    AutoTinySecondMemberDestructor_008ac970(static_cast<unsigned char*>(self) + 72);
    AutoTinyCompositeDestructor_00cbd510(self);
    if (flags & 1)
        operator delete(self);
    return self;
}