// behaviour test for 0x00414F70
#include <stdio.h>
#include <string.h>
class CCharStringData;
class CCharString {                                     // 4-byte handle (Ego_r.pdb); user copy ctor/dtor: returned through a hidden pointer
public:
    CCharStringData* PStringData;
    const char* text; long len;
    CCharString(const char* t, long length = -1) : text(t), len(length) {}
    CCharString(const CCharString& o) : text(o.text), len(o.len) {}
    ~CCharString() {}
};
class CConsoleCommandBase {
public:
    virtual CCharString GetTypeAsString() const;
};

CCharString CConsoleCommandBase::GetTypeAsString() const
{
    return CCharString("command");
}
int main()
{
    CConsoleCommandBase c; CCharString s = c.GetTypeAsString();
    int ok = strcmp(s.text, "command") == 0 && s.len == -1;
    printf(ok ? "00414f70_TEST PASS\n" : "00414f70_TEST FAIL\n");
    return ok ? 0 : 1;
}