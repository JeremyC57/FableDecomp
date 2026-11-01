#pragma optimize("t", on)
// Leaf: `scalar_deleting_destructor'
// Naming wave 28 (family esb): the base destructor is ??1CEntityScriptBase@@UAE@XZ 0x00F35B40 (chunk 696: stores vtable 0x012C3224, the
//   engine/CEntityScriptBase.h class; Ego_r.pdb + FableWin.pdb), called qualified. Was the free
//   __fastcall ?AutoTinyCompositeDestructor_00f35b40@@YIXPAX@Z stand-in.
class CTimer { public:
    ~CTimer();   // 0x00CD4470 (Ego_r.pdb ??1CTimer@@QAE@XZ)
};
class CEntityScriptBase { public: virtual ~CEntityScriptBase(); };   // 0x00F35B40 (vtable 0x012C3224)
void* __fastcall CEngineSubPrimitiveText_CompositeScalarDeletingDestructor(void* self, int, unsigned int flags)
{
    ((CTimer*)(static_cast<unsigned char*>(self) + 40))->~CTimer();
    ((CEntityScriptBase*)(self))->CEntityScriptBase::~CEntityScriptBase();
    if (flags & 1)
        operator delete(self);
    return self;
}