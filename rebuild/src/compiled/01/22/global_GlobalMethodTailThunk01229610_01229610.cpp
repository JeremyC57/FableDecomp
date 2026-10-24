struct AutoTinyThunkTarget
// Leaf: CEntry>_const&>
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013bac18;
void __fastcall GlobalMethodTailThunk_01229610()
{
    g_AutoTinyThunkObject_013bac18.Invoke();
}