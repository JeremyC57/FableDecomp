struct AutoTinyThunkTarget
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013bacc0;
void __fastcall GlobalMethodTailThunk_01229890()
{
    g_AutoTinyThunkObject_013bacc0.Invoke();
}