struct AutoTinyThunkTarget
// Leaf: ?GlobalMethodTailThunk_01229800@@YIXXZ
{
    void Invoke();
};
extern AutoTinyThunkTarget g_AutoTinyThunkObject_013bac9c;
void __fastcall GlobalMethodTailThunk_01229800()
{
    g_AutoTinyThunkObject_013bac9c.Invoke();
}