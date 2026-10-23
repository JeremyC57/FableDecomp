extern void* g_AutoTinyOptionalObject_013baba4;
extern void __cdecl AutoTinyOptionalTarget(void* object);
void __fastcall DisplayProgress()
{
    if (g_AutoTinyOptionalObject_013baba4 != 0)
        AutoTinyOptionalTarget(g_AutoTinyOptionalObject_013baba4);
}