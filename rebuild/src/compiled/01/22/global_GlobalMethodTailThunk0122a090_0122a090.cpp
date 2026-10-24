struct AutoTinyThunkTarget
// Leaf: CEntry>_const&>
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013bafa4;
void __fastcall GlobalMethodTailThunk_0122a090()
{
    g_AutoTinyThunkObject_013bafa4.Invoke();
}