struct CCharString {
    void* p;
    CCharString(const char* s, int n);
};
struct CTCScriptedControl {
    CCharString GetClassTypeName();
};
CCharString CTCScriptedControl::GetClassTypeName(){
    return CCharString((const char*)0x123a514, -1);
}