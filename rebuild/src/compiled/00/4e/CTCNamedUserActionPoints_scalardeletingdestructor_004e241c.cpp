#pragma optimize("s", on)
extern void __fastcall DestructMember_00414ea0(void* member);
// Naming (NF423 gen, 2026-11-25): the callee 0x004DDF6F now binds its PDB spelling ??1CTCUserActionPointBase@@UAE@XZ (the T1 tracker target; the self row's
//   Leaf / PDB name sets), called qualified on a declaration of the class (no body, no layout); was the free stand-in ?CTCNamedUserActionPoints_Destruct@@YIXPAX@Z.
class CTCUserActionPointBase { public: virtual ~CTCUserActionPointBase(); };
void* __fastcall CTCNamedUserActionPoints_CompositeScalarDeletingDestructor(void* points, int, unsigned int flags)
{
    DestructMember_00414ea0(static_cast<unsigned char*>(points) + 52);
    reinterpret_cast<CTCUserActionPointBase*>(points)->CTCUserActionPointBase::~CTCUserActionPointBase();
    if (flags & 1)
        operator delete(points);
    return points;
}