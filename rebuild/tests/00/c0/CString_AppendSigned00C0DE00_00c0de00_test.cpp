#include <stdio.h>
#include <string.h>
#ifdef _MSC_VER

#else
#include <stdio.h>
#endif
class CString{public:char* m_pchData;void Append(const char*);void AppendSigned_00C0DE00(int);};void CString::AppendSigned_00C0DE00(int value){char buffer[64];sprintf(buffer,"%d",value);Append(buffer);}
static const char* textExpected;static CString* expected;static int calls,bad;void CString::Append(const char* text){if(this!=expected || strcmp(text,textExpected))bad=1;++calls;}int main(){CString string;expected=&string;string.m_pchData=(char*)"untouched";
textExpected="-123";string.AppendSigned_00C0DE00(-123);textExpected="2147483647";string.AppendSigned_00C0DE00(2147483647);textExpected="0";string.AppendSigned_00C0DE00(0);
if(bad || calls!=3 || strcmp(string.m_pchData,"untouched")){puts("00c0de00_TEST FAIL");return 1;}puts("00c0de00_TEST PASS");return 0;}