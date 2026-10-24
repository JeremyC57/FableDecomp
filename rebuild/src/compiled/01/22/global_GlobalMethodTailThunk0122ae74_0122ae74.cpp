struct AutoTinyThunkTarget
// Leaf: CEntry>_const&>
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_0143e91c;
void __fastcall GlobalMethodTailThunk_0122ae74()
{
    g_AutoTinyThunkObject_0143e91c.Invoke();
}