struct AutoTinyThunkTarget
// Leaf: CEntry>_const&>
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013bad94;
void __fastcall GlobalMethodTailThunk_01229aa0()
{
    g_AutoTinyThunkObject_013bad94.Invoke();
}