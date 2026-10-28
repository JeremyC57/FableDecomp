#pragma optimize("t", on)
// Leaf: `vector_deleting_destructor'
class CScriptThing { public: virtual ~CScriptThing(); };   // 0x004AA840 (Ego_r.pdb ??1CScriptThing@@UAE@XZ)
extern void __fastcall AutoTinySecondMemberDestructor(void* member);
extern void __fastcall AutoTinyCompositeDestructor(void* self);
void* __fastcall CThingSwitch_CompositeScalarDeletingDestructor(void* self, int, unsigned int flags)
{
    reinterpret_cast<CScriptThing*>(static_cast<unsigned char*>(self) + 72)->CScriptThing::~CScriptThing();
    AutoTinySecondMemberDestructor(static_cast<unsigned char*>(self) + 52);
    AutoTinyCompositeDestructor(self);
    if (flags & 1)
        operator delete(self);
    return self;
}