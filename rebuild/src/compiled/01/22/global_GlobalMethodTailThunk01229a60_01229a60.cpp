struct AutoTinyThunkTarget
// Leaf: CEntry>_const&>
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013bad64;
void __fastcall GlobalMethodTailThunk_01229a60()
{
    g_AutoTinyThunkObject_013bad64.Invoke();
}