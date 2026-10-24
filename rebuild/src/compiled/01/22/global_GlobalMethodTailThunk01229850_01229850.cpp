struct AutoTinyThunkTarget
// Leaf: CEntry>_const&>
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013bacb0;
void __fastcall GlobalMethodTailThunk_01229850()
{
    g_AutoTinyThunkObject_013bacb0.Invoke();
}