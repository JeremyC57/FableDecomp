struct AutoTinyThunkTarget
// Leaf: CEntry>_const&>
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013bacf0;
void __fastcall GlobalMethodTailThunk_01229950()
{
    g_AutoTinyThunkObject_013bacf0.Invoke();
}