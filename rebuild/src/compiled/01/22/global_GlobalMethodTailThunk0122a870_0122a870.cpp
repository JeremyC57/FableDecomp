struct AutoTinyThunkTarget
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013caa30;
void __fastcall GlobalMethodTailThunk_0122a870()
{
    g_AutoTinyThunkObject_013caa30.Invoke();
}