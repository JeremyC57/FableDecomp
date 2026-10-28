class CCharString { public:
    void* p;
    CCharString(const char* s, long n);
};
class CTCScriptedControl { public:
    CCharString GetClassTypeName();
};
CCharString CTCScriptedControl::GetClassTypeName(){
    return CCharString((const char*)0x123a514, -1);
}