struct AutoTinyThunkTarget
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013bad3c;
void __fastcall GlobalMethodTailThunk_01229a00()
{
    g_AutoTinyThunkObject_013bad3c.Invoke();
}