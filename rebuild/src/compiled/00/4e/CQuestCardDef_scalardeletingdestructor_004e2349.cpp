#pragma optimize("s", on)
extern void __fastcall CQuestCardDef_Destruct(void* def);
void* __fastcall CQuestCardDef_ScalarDeletingDestructor(void* def, int, unsigned int flags)
{
    CQuestCardDef_Destruct(def);
    if (flags & 1)
        operator delete(def);
    return def;
}