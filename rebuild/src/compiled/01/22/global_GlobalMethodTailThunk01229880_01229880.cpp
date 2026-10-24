struct AutoTinyThunkTarget
// Leaf: CEntry>_const&>
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013bacbc;
void __fastcall GlobalMethodTailThunk_01229880()
{
    g_AutoTinyThunkObject_013bacbc.Invoke();
}