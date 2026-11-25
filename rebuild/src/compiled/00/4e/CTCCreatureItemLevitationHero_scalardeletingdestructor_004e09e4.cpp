#pragma optimize("s", on)
// Naming (NF423 gen, 2026-11-25): the callee 0x004DE6ED now binds its PDB spelling ??1CTCCreatureItemLevitationBase@@UAE@XZ (the T1 tracker target; the self row's
//   Leaf / PDB name sets), called qualified on a declaration of the class (no body, no layout); was the free stand-in ?CTCCreatureItemLevitationHero_Destruct@@YIXPAX@Z.
class CTCCreatureItemLevitationBase { public: virtual ~CTCCreatureItemLevitationBase(); };
void* __fastcall CTCCreatureItemLevitationHero_ScalarDeletingDestructor(void* hero, int, unsigned int flags)
{
    reinterpret_cast<CTCCreatureItemLevitationBase*>(hero)->CTCCreatureItemLevitationBase::~CTCCreatureItemLevitationBase();
    if (flags & 1)
        operator delete(hero);
    return hero;
}