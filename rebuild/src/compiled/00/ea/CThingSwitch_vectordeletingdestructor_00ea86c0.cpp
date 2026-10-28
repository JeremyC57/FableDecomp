#pragma optimize("t", on)
// Leaf: `vector_deleting_destructor'
extern void __fastcall AutoTinyFirstMemberDestructor_007add50(void* member);
class CScriptThing { public: virtual ~CScriptThing(); };   // 0x004AA840 (Ego_r.pdb ??1CScriptThing@@UAE@XZ)
extern void __fastcall AutoTinyCompositeDestructor_00cbd510(void* self);
void* __fastcall CThingSwitch_CompositeScalarDeletingDestructor(void* self, int, unsigned int flags)
{
    AutoTinyFirstMemberDestructor_007add50(static_cast<unsigned char*>(self) + 88);
    reinterpret_cast<CScriptThing*>(static_cast<unsigned char*>(self) + 76)->CScriptThing::~CScriptThing();
    AutoTinyCompositeDestructor_00cbd510(self);
    if (flags & 1)
        operator delete(self);
    return self;
}