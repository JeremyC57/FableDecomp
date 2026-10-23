extern void* g_AutoTinyOptionalObject_013d2d48;
extern void __cdecl AutoTinyOptionalTarget(void* object);
void __fastcall DisplayProgress()
{
    if (g_AutoTinyOptionalObject_013d2d48 != 0)
        AutoTinyOptionalTarget(g_AutoTinyOptionalObject_013d2d48);
}