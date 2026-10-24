extern void* g_AutoTinyOptionalObject;
// Leaf: DisplayProgress
extern void __cdecl AutoTinyOptionalTarget(void* object);
void __fastcall DisplayProgress()
{
    if (g_AutoTinyOptionalObject != 0)
        AutoTinyOptionalTarget(g_AutoTinyOptionalObject);
}