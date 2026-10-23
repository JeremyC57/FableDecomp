struct AutoTinyThunkTarget
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_01436ec4;
void __fastcall GlobalMethodTailThunk_0122acd0()
{
    g_AutoTinyThunkObject_01436ec4.Invoke();
}