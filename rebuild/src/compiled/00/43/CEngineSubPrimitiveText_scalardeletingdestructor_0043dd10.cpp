class CCharString { public:
    ~CCharString();   // 0x0099EAE0 (Ego_r.pdb ??1CCharString@@QAE@XZ)
};
// Leaf: ?CEngineSubPrimitiveText_CompositeScalarDeletingDestructor@@YIPAXPAXHI@Z
class CWideString { public:
    ~CWideString();   // 0x0099B510 (Ego_r.pdb ??1CWideString@@QAE@XZ)
};
void* __fastcall CEngineSubPrimitiveText_CompositeScalarDeletingDestructor(void* self, int, unsigned int flags)
{
    ((CCharString*)(static_cast<unsigned char*>(self) + 16))->~CCharString();
    ((CWideString*)self)->~CWideString();
    if (flags & 1)
        operator delete(self);
    return self;
}