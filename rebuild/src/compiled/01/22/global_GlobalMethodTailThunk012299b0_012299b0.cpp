struct AutoTinyThunkTarget
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013bad28;
void __fastcall GlobalMethodTailThunk_012299b0()
{
    g_AutoTinyThunkObject_013bad28.Invoke();
}