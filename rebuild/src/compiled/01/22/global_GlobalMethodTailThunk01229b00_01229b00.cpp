struct AutoTinyThunkTarget
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013bada8;
void __fastcall GlobalMethodTailThunk_01229b00()
{
    g_AutoTinyThunkObject_013bada8.Invoke();
}