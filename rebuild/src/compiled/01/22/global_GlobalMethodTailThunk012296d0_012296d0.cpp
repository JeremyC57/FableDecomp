struct AutoTinyThunkTarget
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013bac50;
void __fastcall GlobalMethodTailThunk_012296d0()
{
    g_AutoTinyThunkObject_013bac50.Invoke();
}