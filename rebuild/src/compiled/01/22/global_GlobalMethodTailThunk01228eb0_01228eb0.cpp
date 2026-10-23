struct AutoTinyThunkTarget
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013b8af8;
void __fastcall GlobalMethodTailThunk_01228eb0()
{
    g_AutoTinyThunkObject_013b8af8.Invoke();
}