struct AutoTinyThunkTarget
// Leaf: CEntry>_const&>
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013bab2c;
void __fastcall GlobalMethodTailThunk_01229470()
{
    g_AutoTinyThunkObject_013bab2c.Invoke();
}