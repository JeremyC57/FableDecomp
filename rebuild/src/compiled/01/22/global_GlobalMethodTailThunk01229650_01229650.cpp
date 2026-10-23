struct AutoTinyThunkTarget
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013bac30;
void __fastcall GlobalMethodTailThunk_01229650()
{
    g_AutoTinyThunkObject_013bac30.Invoke();
}