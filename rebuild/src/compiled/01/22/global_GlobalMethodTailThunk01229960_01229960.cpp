struct AutoTinyThunkTarget
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013bacf4;
void __fastcall GlobalMethodTailThunk_01229960()
{
    g_AutoTinyThunkObject_013bacf4.Invoke();
}