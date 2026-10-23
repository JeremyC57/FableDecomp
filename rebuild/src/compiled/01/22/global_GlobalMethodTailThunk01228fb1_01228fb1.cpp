struct AutoTinyThunkTarget
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013b8b98;
void __fastcall GlobalMethodTailThunk_01228fb1()
{
    g_AutoTinyThunkObject_013b8b98.Invoke();
}