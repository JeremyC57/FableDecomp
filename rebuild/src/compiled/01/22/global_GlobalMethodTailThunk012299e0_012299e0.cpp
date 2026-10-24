struct AutoTinyThunkTarget
// Leaf: CEntry>_const&>
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013bad34;
void __fastcall GlobalMethodTailThunk_012299e0()
{
    g_AutoTinyThunkObject_013bad34.Invoke();
}