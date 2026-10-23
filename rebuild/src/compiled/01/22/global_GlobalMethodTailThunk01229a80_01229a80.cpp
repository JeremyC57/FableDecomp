struct AutoTinyThunkTarget
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013bad8c;
void __fastcall GlobalMethodTailThunk_01229a80()
{
    g_AutoTinyThunkObject_013bad8c.Invoke();
}