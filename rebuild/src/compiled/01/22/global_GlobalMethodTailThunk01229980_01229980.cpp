struct AutoTinyThunkTarget
// Leaf: CEntry>_const&>
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013bacfc;
void __fastcall GlobalMethodTailThunk_01229980()
{
    g_AutoTinyThunkObject_013bacfc.Invoke();
}