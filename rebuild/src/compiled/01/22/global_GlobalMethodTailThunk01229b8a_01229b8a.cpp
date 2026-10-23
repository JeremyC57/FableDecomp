struct AutoTinyThunkTarget
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013badf4;
void __fastcall GlobalMethodTailThunk_01229b8a()
{
    g_AutoTinyThunkObject_013badf4.Invoke();
}