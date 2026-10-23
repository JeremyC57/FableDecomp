extern void* g_AutoTinyOptionalObject_013d2d1c;
extern void __cdecl AutoTinyOptionalTarget(void* object);
void __fastcall DisplayProgress()
{
    if (g_AutoTinyOptionalObject_013d2d1c != 0)
        AutoTinyOptionalTarget(g_AutoTinyOptionalObject_013d2d1c);
}