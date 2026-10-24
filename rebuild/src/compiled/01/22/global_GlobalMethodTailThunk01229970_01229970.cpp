struct AutoTinyThunkTarget
// Leaf: CEntry>_const&>
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013bacf8;
void __fastcall GlobalMethodTailThunk_01229970()
{
    g_AutoTinyThunkObject_013bacf8.Invoke();
}