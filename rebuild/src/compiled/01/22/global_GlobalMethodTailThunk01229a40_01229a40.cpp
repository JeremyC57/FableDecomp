struct AutoTinyThunkTarget
// Leaf: CEntry>_const&>
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013bad5c;
void __fastcall GlobalMethodTailThunk_01229a40()
{
    g_AutoTinyThunkObject_013bad5c.Invoke();
}