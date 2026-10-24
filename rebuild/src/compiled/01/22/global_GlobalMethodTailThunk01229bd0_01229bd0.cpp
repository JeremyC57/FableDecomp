struct AutoTinyThunkTarget
// Leaf: CEntry>_const&>
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013bae0c;
void __fastcall GlobalMethodTailThunk_01229bd0()
{
    g_AutoTinyThunkObject_013bae0c.Invoke();
}