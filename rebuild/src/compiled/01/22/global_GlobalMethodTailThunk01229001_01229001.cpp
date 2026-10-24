struct AutoTinyThunkTarget
// Leaf: CEntry>_const&>
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013b8bb8;
void __fastcall GlobalMethodTailThunk_01229001()
{
    g_AutoTinyThunkObject_013b8bb8.Invoke();
}