struct AutoTinyThunkTarget
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013baca4;
void __fastcall GlobalMethodTailThunk_01229820()
{
    g_AutoTinyThunkObject_013baca4.Invoke();
}