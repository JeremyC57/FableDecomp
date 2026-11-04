struct AutoTinyThunkTarget
// Leaf: ?GlobalMethodTailThunk_01229b70@@YIXXZ
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013badec;
void __fastcall GlobalMethodTailThunk_01229b70()
{
    g_AutoTinyThunkObject_013badec.Invoke();
}