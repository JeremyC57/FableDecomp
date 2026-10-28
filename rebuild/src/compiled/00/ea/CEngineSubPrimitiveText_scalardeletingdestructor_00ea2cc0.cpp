#pragma optimize("t", on)
// Leaf: `scalar_deleting_destructor'
class CScriptThing { public: virtual ~CScriptThing(); };   // 0x004AA840 (Ego_r.pdb ??1CScriptThing@@UAE@XZ)
extern void __fastcall AutoTinyCompositeDestructor_00cbd510(void* self);
void* __fastcall CEngineSubPrimitiveText_CompositeScalarDeletingDestructor(void* self, int, unsigned int flags)
{
    reinterpret_cast<CScriptThing*>(static_cast<unsigned char*>(self) + 92)->CScriptThing::~CScriptThing();
    AutoTinyCompositeDestructor_00cbd510(self);
    if (flags & 1)
        operator delete(self);
    return self;
}