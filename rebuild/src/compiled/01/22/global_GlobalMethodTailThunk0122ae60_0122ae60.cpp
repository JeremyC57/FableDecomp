struct AutoTinyThunkTarget
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_0143e8ec;
void __fastcall GlobalMethodTailThunk_0122ae60()
{
    g_AutoTinyThunkObject_0143e8ec.Invoke();
}