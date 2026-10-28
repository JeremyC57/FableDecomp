#include <stdio.h>
#include <string.h>
#ifdef _MSC_VER

#else
#include <stdio.h>
#endif
class CString{public:char* m_pchData;void Append(const char*);void AppendPointer_00C0DE50(const void*);};void CString::AppendPointer_00C0DE50(const void* value){char buffer[64];sprintf(buffer,"%p",value);Append(buffer);}
static const char* textExpected;static CString* expected;static int calls,bad;void CString::Append(const char* text){if(this!=expected || strcmp(text,textExpected))bad=1;++calls;}int main(){CString string;expected=&string;string.m_pchData=(char*)"untouched";
#ifdef _MSC_VER
textExpected="00001234";
#else
textExpected="0x1234";
#endif
string.AppendPointer_00C0DE50((const void*)0x1234);
#ifdef _MSC_VER
textExpected="00000000";
#else
textExpected="(nil)";
#endif
string.AppendPointer_00C0DE50(0);

if(bad || calls!=2 || strcmp(string.m_pchData,"untouched")){puts("00c0de50_TEST FAIL");return 1;}puts("00c0de50_TEST PASS");return 0;}