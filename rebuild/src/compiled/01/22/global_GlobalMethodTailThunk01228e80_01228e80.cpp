struct AutoTinyThunkTarget
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013b8aec;
void __fastcall GlobalMethodTailThunk_01228e80()
{
    g_AutoTinyThunkObject_013b8aec.Invoke();
}