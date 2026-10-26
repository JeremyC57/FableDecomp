#pragma optimize("t", on)
// Leaf: `scalar_deleting_destructor'
class CScriptThing {
public:
    virtual ~CScriptThing();   // 0x004AA840
};
extern void __fastcall AutoTinyCompositeDestructor(void* self);
void* __fastcall CEngineSubPrimitiveText_CompositeScalarDeletingDestructor(void* self, int, unsigned int flags)
{
    ((CScriptThing*)(static_cast<unsigned char*>(self) + 60))->CScriptThing::~CScriptThing();
    AutoTinyCompositeDestructor(self);
    if (flags & 1)
        operator delete(self);
    return self;
}