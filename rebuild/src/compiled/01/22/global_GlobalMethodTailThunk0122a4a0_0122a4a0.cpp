struct AutoTinyThunkTarget
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013bc440;
void __fastcall GlobalMethodTailThunk_0122a4a0()
{
    g_AutoTinyThunkObject_013bc440.Invoke();
}