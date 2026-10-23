struct AutoTinyThunkTarget
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013b8a50;
void __fastcall GlobalMethodTailThunk_01228dc0()
{
    g_AutoTinyThunkObject_013b8a50.Invoke();
}