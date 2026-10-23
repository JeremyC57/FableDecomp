struct AutoTinyThunkTarget
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013bade4;
void __fastcall GlobalMethodTailThunk_01229b50()
{
    g_AutoTinyThunkObject_013bade4.Invoke();
}