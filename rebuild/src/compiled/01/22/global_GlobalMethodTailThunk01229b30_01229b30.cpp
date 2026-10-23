struct AutoTinyThunkTarget
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013baddc;
void __fastcall GlobalMethodTailThunk_01229b30()
{
    g_AutoTinyThunkObject_013baddc.Invoke();
}