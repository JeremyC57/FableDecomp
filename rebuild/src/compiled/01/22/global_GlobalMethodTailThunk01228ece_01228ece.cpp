struct AutoTinyThunkTarget
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013b8b3c;
void __fastcall GlobalMethodTailThunk_01228ece()
{
    g_AutoTinyThunkObject_013b8b3c.Invoke();
}