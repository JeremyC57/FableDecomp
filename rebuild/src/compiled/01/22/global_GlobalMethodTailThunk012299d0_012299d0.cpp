struct AutoTinyThunkTarget
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013bad30;
void __fastcall GlobalMethodTailThunk_012299d0()
{
    g_AutoTinyThunkObject_013bad30.Invoke();
}