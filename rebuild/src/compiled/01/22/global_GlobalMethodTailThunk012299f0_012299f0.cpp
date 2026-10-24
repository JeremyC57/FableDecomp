struct AutoTinyThunkTarget
// Leaf: CEntry>_const&>
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013bad38;
void __fastcall GlobalMethodTailThunk_012299f0()
{
    g_AutoTinyThunkObject_013bad38.Invoke();
}