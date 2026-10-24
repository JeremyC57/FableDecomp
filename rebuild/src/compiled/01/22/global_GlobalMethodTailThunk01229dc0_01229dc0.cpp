struct AutoTinyThunkTarget
// Leaf: CEntry>_const&>
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013bae6c;
void __fastcall GlobalMethodTailThunk_01229dc0()
{
    g_AutoTinyThunkObject_013bae6c.Invoke();
}