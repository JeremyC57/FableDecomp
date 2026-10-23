struct AutoTinyThunkTarget
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013baf0c;
void __fastcall GlobalMethodTailThunk_01229ee0()
{
    g_AutoTinyThunkObject_013baf0c.Invoke();
}