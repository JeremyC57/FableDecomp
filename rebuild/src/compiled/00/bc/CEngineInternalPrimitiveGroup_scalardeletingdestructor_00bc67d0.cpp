#pragma optimize("t", on)
extern void __fastcall DestructMember_00af7210(void* member);
// Naming (NF423 gen, 2026-11-25): the callee 0x00B9C660 now binds its PDB spelling ??1CEngineInternalPrimitiveOcclusionBase@@UAE@XZ (the T1 tracker target; the self row's
//   Leaf / PDB name sets), called qualified on a declaration of the class (no body, no layout); was the free stand-in ?CEngineInternalPrimitiveGroup_Destruct@@YIXPAX@Z.
class CEngineInternalPrimitiveOcclusionBase { public: virtual ~CEngineInternalPrimitiveOcclusionBase(); };
void* __fastcall CEngineInternalPrimitiveGroup_CompositeScalarDeletingDestructor(void* group, int, unsigned int flags)
{
    DestructMember_00af7210(static_cast<unsigned char*>(group) + 80);
    reinterpret_cast<CEngineInternalPrimitiveOcclusionBase*>(group)->CEngineInternalPrimitiveOcclusionBase::~CEngineInternalPrimitiveOcclusionBase();
    if (flags & 1)
        operator delete(group);
    return group;
}