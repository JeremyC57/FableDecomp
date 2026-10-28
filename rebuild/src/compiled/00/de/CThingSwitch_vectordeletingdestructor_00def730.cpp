#pragma optimize("t", on)
// Leaf: `vector_deleting_destructor'
class CScriptThing { public: virtual ~CScriptThing(); };   // 0x004AA840 (Ego_r.pdb ??1CScriptThing@@UAE@XZ)
extern void __fastcall AutoTinySecondMemberDestructor_008ac970(void* member);
extern void __fastcall AutoTinyCompositeDestructor_00f35b40(void* self);
void* __fastcall CThingSwitch_CompositeScalarDeletingDestructor(void* self, int, unsigned int flags)
{
    reinterpret_cast<CScriptThing*>(static_cast<unsigned char*>(self) + 40)->CScriptThing::~CScriptThing();
    AutoTinySecondMemberDestructor_008ac970(static_cast<unsigned char*>(self) + 28);
    AutoTinyCompositeDestructor_00f35b40(self);
    if (flags & 1)
        operator delete(self);
    return self;
}