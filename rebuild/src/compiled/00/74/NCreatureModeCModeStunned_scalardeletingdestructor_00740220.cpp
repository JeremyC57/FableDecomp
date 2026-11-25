#pragma optimize("t", on)
extern void __fastcall DestructMember_00739db0(void* member);
// Naming (NF423 gen, 2026-11-25): the callee 0x008D7580 now binds its PDB spelling ??1CModeOutOfActionBase@NCreatureMode@@UAE@XZ (the T1 tracker target; the self row's
//   Leaf / PDB name sets), called qualified on a declaration of the class (no body, no layout); was the free stand-in ?CModeStunned_Destruct@@YIXPAX@Z.
namespace NCreatureMode { class CModeOutOfActionBase { public: virtual ~CModeOutOfActionBase(); }; }
void* __fastcall CModeStunned_CompositeScalarDeletingDestructor(void* mode, int, unsigned int flags)
{
    DestructMember_00739db0(static_cast<unsigned char*>(mode) + 120);
    reinterpret_cast<NCreatureMode::CModeOutOfActionBase*>(mode)->NCreatureMode::CModeOutOfActionBase::~CModeOutOfActionBase();
    if (flags & 1)
        operator delete(mode);
    return mode;
}