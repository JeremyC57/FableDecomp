struct AutoTinyThunkTarget
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013b8a7c;
void __fastcall GlobalMethodTailThunk_01228de0()
{
    g_AutoTinyThunkObject_013b8a7c.Invoke();
}