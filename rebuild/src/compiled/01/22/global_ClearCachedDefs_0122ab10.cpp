struct AutoTinyFieldMethodsTarget
{
    unsigned int value;
    void First();
    void Second_0099a300();
};
extern AutoTinyFieldMethodsTarget g_AutoTinyFieldMethodsObject;
void __fastcall ClearCachedDefs()
{
    g_AutoTinyFieldMethodsObject.value = 0x0129a064;
    g_AutoTinyFieldMethodsObject.First();
    g_AutoTinyFieldMethodsObject.Second_0099a300();
}