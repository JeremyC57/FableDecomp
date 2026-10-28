#include <stdio.h>
#include <string.h>
#ifndef _MSC_VER
#define __fastcall
#endif
class CCharStringData;class CCharString{public:CCharStringData* PStringData;CCharString(const char*,long=-1);};
CCharString __fastcall CUnprovenCombatNameFactory_008b0440(){return CCharString("COMBAT_SEQUENCE_WHISPER_SORT_YOURSELF_OUT");}
static int calls;static const char* seen;static int length;CCharString::CCharString(const char* text,long n){++calls;seen=text;length=n;PStringData=(CCharStringData*)text;}
int main(){CCharString result=CUnprovenCombatNameFactory_008b0440();if(calls!=1 || strcmp(seen,"COMBAT_SEQUENCE_WHISPER_SORT_YOURSELF_OUT") || length!=-1 || (const char*)result.PStringData!=seen){puts("008b0440_TEST FAIL");return 1;}puts("008b0440_TEST PASS");return 0;}