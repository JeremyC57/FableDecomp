struct AutoTinyThunkTarget
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013b8b68;
void __fastcall GlobalMethodTailThunk_01228f39()
{
    g_AutoTinyThunkObject_013b8b68.Invoke();
}