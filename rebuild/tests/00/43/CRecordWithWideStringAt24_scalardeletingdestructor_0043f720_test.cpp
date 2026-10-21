// behaviour test for 0x0043F720
#include <stdio.h>
static int g_dtors = 0; static void* g_last = 0;
class CWideString {
public:
    void* PData;
    ~CWideString() { ++g_dtors; g_last = this; }  //                                        // 0x0099B510
};
class CRecordWithWideStringAt24 {
public:
    unsigned char _pad00[0x24];
    CWideString Text;                                         // +0x24
    ~CRecordWithWideStringAt24() {}
};
void DeleteCRecordWithWideStringAt24(CRecordWithWideStringAt24* p)                 // emission helper, not a retail function
{
    delete p;
}

int main()
{
    CRecordWithWideStringAt24* p = new CRecordWithWideStringAt24; char* text = (char*)&p->Text;
    int ok = text - (char*)p == sizeof(p->_pad00);
    DeleteCRecordWithWideStringAt24(p);
    ok &= g_dtors == 1 && g_last == text;
    printf(ok ? "0043f720_TEST PASS\n" : "0043f720_TEST FAIL\n");
    return ok ? 0 : 1;
}