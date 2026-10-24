struct AutoTinyThunkTarget
// Leaf: CEntry>_const&>
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013bab28;
void __fastcall GlobalMethodTailThunk_01229460()
{
    g_AutoTinyThunkObject_013bab28.Invoke();
}