#pragma optimize("s", on)
// Naming (NF423 gen, 2026-11-25): the callee 0x004E2365 now binds its PDB spelling ??1CQuestCardDef@@UAE@XZ (the T1 tracker target; the self row's
//   Leaf / PDB name sets), called qualified on a declaration of the class (no body, no layout); was the free stand-in ?CQuestCardDef_Destruct@@YIXPAX@Z.
class CQuestCardDef { public: virtual ~CQuestCardDef(); };
void* __fastcall CQuestCardDef_ScalarDeletingDestructor(void* def, int, unsigned int flags)
{
    reinterpret_cast<CQuestCardDef*>(def)->CQuestCardDef::~CQuestCardDef();
    if (flags & 1)
        operator delete(def);
    return def;
}