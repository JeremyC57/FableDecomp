struct AutoTinyThunkTarget
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013b8af0;
void __fastcall GlobalMethodTailThunk_01228e90()
{
    g_AutoTinyThunkObject_013b8af0.Invoke();
}