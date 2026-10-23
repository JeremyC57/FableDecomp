struct AutoTinyThunkTarget
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013babc8;
void __fastcall GlobalMethodTailThunk_012295e0()
{
    g_AutoTinyThunkObject_013babc8.Invoke();
}