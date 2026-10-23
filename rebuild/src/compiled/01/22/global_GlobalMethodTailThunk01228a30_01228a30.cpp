struct AutoTinyThunkTarget
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013b8704;
void __fastcall GlobalMethodTailThunk_01228a30()
{
    g_AutoTinyThunkObject_013b8704.Invoke();
}