#include <cstdio>
#include <cstring>
struct CCharString {
    char text[32];
};
CCharString __fastcall operator+(const CCharString& left, const char* right)
{
    CCharString r;
    std::strcpy(r.text, left.text);
    std::strcat(r.text, right);
    return r;
}
struct CConsoleVariable {
    virtual void v00() { }
    CCharString Name;
    unsigned char Var[8];
    virtual CCharString GetFormattedName() const;
};
CCharString CConsoleVariable::GetFormattedName() const
{
    return Name + " ";
}
int main()
{
    CConsoleVariable v;
    std::strcpy(v.Name.text, "g_fov");
    CCharString f = v.GetFormattedName();
    if (std::strcmp(f.text, "g_fov ") != 0) { std::printf("009ea490_TEST FAIL\n"); return 1; }
    std::printf("009ea490_TEST PASS\n");
    return 0;
}
