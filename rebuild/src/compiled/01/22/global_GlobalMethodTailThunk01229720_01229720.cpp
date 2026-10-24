struct AutoTinyThunkTarget
// Leaf: CEntry>_const&>
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013bac64;
void __fastcall GlobalMethodTailThunk_01229720()
{
    g_AutoTinyThunkObject_013bac64.Invoke();
}