#pragma optimize("s", on)
// Naming (NF423 gen, 2026-11-25): the callee 0x004E1856 now binds its PDB spelling ??1CTCAISummon@@UAE@XZ (the T1 tracker target; the self row's
//   Leaf / PDB name sets), called qualified on a declaration of the class (no body, no layout); was the free stand-in ?CTCAISummon_Destruct@@YIXPAX@Z.
class CTCAISummon { public: virtual ~CTCAISummon(); };
void* __fastcall CTCAISummon_ScalarDeletingDestructor(void* summon, int, unsigned int flags)
{
    reinterpret_cast<CTCAISummon*>(summon)->CTCAISummon::~CTCAISummon();
    if (flags & 1)
        operator delete(summon);
    return summon;
}