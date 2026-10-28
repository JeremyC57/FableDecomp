#include <stdio.h>
#include <string.h>
// CBankCreatorException::GetText() -- Return a copy of the owned value.
// Leaf: ?GetText@CBankCreatorException@@QBE?AVCCharString@@XZ
// Identity: PDB GetText const; caller009cf328 formats caught bank exception; EntrySymbol+0/ErrorText+4.
class CCharStringData;
class CCharString {public:CCharStringData* PStringData;CCharString():PStringData(0){};CCharString(const CCharString&);~CCharString();};
// Layout: no active exception header; original EntrySymbol+0, ErrorText+4.
class CBankCreatorException {public:CCharString EntrySymbol,ErrorText;CCharString GetText() const;};
CCharString CBankCreatorException::GetText() const {return ErrorText;}

static int copies,destroys;static const CCharString* copied;
CCharString::CCharString(const CCharString& o):PStringData(o.PStringData){++copies;copied=&o;}
CCharString::~CCharString(){++destroys;}
int main(){int token=1;CBankCreatorException object;object.ErrorText.PStringData=(CCharStringData*)&token;object.EntrySymbol.PStringData=0;unsigned char before[sizeof(object)];memcpy(before,&object,sizeof(object));int ok=1;{const CCharString& value=object.GetText();ok=ok && value.PStringData==(CCharStringData*)&token && copies==1 && copied==&object.ErrorText && destroys==0;}ok=ok && destroys==1 && memcmp(before,&object,sizeof(object))==0;{const CCharString& value=object.GetText();ok=ok && value.PStringData==(CCharStringData*)&token && copies==2;}ok=ok && destroys==2;puts(ok?"009cb930_TEST PASS":"009cb930_TEST FAIL");return ok?0:1;}