struct AutoTinyThunkTarget
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013bad60;
void __fastcall GlobalMethodTailThunk_01229a50()
{
    g_AutoTinyThunkObject_013bad60.Invoke();
}