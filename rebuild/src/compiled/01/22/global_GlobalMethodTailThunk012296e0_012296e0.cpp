struct AutoTinyThunkTarget
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013bac54;
void __fastcall GlobalMethodTailThunk_012296e0()
{
    g_AutoTinyThunkObject_013bac54.Invoke();
}