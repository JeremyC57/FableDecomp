struct AutoTinyThunkTarget
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013b8ae4;
void __fastcall GlobalMethodTailThunk_01228e60()
{
    g_AutoTinyThunkObject_013b8ae4.Invoke();
}