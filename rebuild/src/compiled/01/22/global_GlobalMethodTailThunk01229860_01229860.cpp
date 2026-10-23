struct AutoTinyThunkTarget
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013bacb4;
void __fastcall GlobalMethodTailThunk_01229860()
{
    g_AutoTinyThunkObject_013bacb4.Invoke();
}