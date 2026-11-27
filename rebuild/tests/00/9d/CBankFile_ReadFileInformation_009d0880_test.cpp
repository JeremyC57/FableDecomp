#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static char converted[160];
struct CCharString{
 char value[160];CCharString(const char* s=""){strcpy(value,s);}
 operator const char*()const{strcpy(converted,value);return value;}
};
static CCharString operator+(CCharString a,const char*b){strcat(a.value,b);return a;}
static CCharString operator+(CCharString a,const CCharString& b){strcat(a.value,b.value);return a;}
static CCharString GFIntToCharString(long n){char a[32];sprintf(a,"%ld",n);return a;}
struct CWideString{CCharString ToCharString()const{return CCharString("fixture.bank");}};
class CExceptionBase {public: CExceptionBase(){} CExceptionBase(const char*){}};
class CBankFileException : public CExceptionBase {
public:
    enum EBankFileExceptionType {
        BANK_IS_CORRUPT,
        BANK_IS_INCORRECT_VERSION,
        ENTRY_NOT_FOUND,
        ENTRY_IS_CORRUPT
    };

    CBankFileException(EBankFileExceptionType type) : Type(type) { }
    CBankFileException(CCharString message, EBankFileExceptionType type)
        : CExceptionBase(message), Type(type) { }

protected:
    EBankFileExceptionType Type;
};

class ExceptionTypeAccess : public CBankFileException {
public: static EBankFileExceptionType Read(const CBankFileException& e){ EBankFileExceptionType CBankFileException::*p=&ExceptionTypeAccess::Type;return e.*p;}
};
typedef char ExceptionHasFourBytes[(sizeof(CBankFileException)==4)?1:-1];
struct CFile{unsigned char data[80];unsigned long len;long GetLength()const{return len;}};
struct CFileDataInputStream{CFile& f;unsigned long pos;CFileDataInputStream(CFile& x,unsigned long n):f(x),pos(0){if(n!=0x4000)abort();}void SetPosition(unsigned long n){pos=n;}unsigned long GetPosition()const{return pos;}bool IsEOF()const{return pos==f.len;}void Read(void* p,long n){if(pos+n>f.len)abort();memcpy(p,f.data+pos,n);pos+=n;}unsigned long ReadULONG(){unsigned long r;Read(&r,4);return r;}};
class CBankFile{public:CWideString Filename;CFile* BankFile;unsigned long BankFormatVersion,Alignment,HeaderSize,FileSize;bool ReadOnly;int recover,entries;bool rOK,eOK;const char* log;CCharString saved;
 unsigned long GetDataFormatVersion()const{return 19;}
 void WriteBankLog(const CCharString& msg){saved=msg;log=saved.value;}
 bool ReadRecoveryFile(unsigned long x){if(x!=2)abort();++recover;return rOK;}
 bool ReadEntryList(CFileDataInputStream& s,unsigned long mx,unsigned long cnt){if(mx!=14||cnt!=4||s.GetPosition()!=48)abort();++entries;return eOK;}
 bool ReadFileInformation(bool);
};
bool CBankFile::ReadFileInformation(bool throwOnError)
{
    CCharString name(Filename.ToCharString());
    CFileDataInputStream stream(*BankFile, 0x4000);
    stream.SetPosition(0);
    bool valid_header = false;
    if (!stream.IsEOF()) {
        char correct_header[] = "BBBB";
        char header[4];
        stream.Read(header, 4);
        if (memcmp(header, correct_header, 4) == 0)
            valid_header = true;
    }
    if (!valid_header) {
        if (throwOnError)
            throw CBankFileException(name + " is not a valid resource file!", CBankFileException::BANK_IS_CORRUPT);
        return false;
    }
    unsigned long format = stream.ReadULONG();
    if (format != BankFormatVersion) {
        if (ReadOnly) {
            if (throwOnError) {
                throw CBankFileException(name + " has the wrong bank format! (should be " + GFIntToCharString(BankFormatVersion) +
                                     " not " + GFIntToCharString(format) + ")", CBankFileException::BANK_IS_INCORRECT_VERSION);
            }
            return false;
        }
        WriteBankLog("Bank file version has changed, requiring rebuild");
        return true;
    }
    unsigned long version = stream.ReadULONG();
    if (version != GetDataFormatVersion()) {
        if (ReadOnly) {
            if (throwOnError) {
                throw CBankFileException(name + " has the wrong version number! (should be " + GFIntToCharString(GetDataFormatVersion()) +
                                     " not " + GFIntToCharString(version) + ")", CBankFileException::BANK_IS_INCORRECT_VERSION);
            }
            return false;
        }
        WriteBankLog("Bank data version has changed, requiring rebuild");
        return true;
    }
    unsigned long updateState = stream.ReadULONG();
    unsigned long alignment = stream.ReadULONG();
    if (alignment != Alignment) {
        if (ReadOnly) {
            Alignment = alignment;
        } else {
            WriteBankLog("Data alignment has changed, requiring rebuild");
            return true;
        }
    }
    unsigned long maxIndex = stream.ReadULONG();
    unsigned long entryCount = stream.ReadULONG();
    unsigned long entryListStart = stream.ReadULONG();
    HeaderSize = stream.GetPosition();
    FileSize = BankFile->GetLength();
    if (updateState != 1) {
        if (ReadOnly) {
            if (throwOnError) {
                throw CBankFileException(CBankFileException::BANK_IS_CORRUPT);
            }
            return false;
        }
        if (!ReadRecoveryFile(updateState))
            WriteBankLog("Previous bank update was interrupted, requiring rebuild");
        return true;
    }
    stream.SetPosition(entryListStart);
    return ReadEntryList(stream, maxIndex, entryCount);
}

int main(){
 try { throw CBankFileException(CBankFileException::BANK_IS_CORRUPT); } catch(const CExceptionBase&) { }
 for(int m=0;m<3;++m)for(int flags=0;flags<256;++flags){
  bool format=(flags&1)!=0,version=(flags&2)!=0,align=(flags&4)!=0,valid=(flags&8)!=0,ro=(flags&16)!=0,ex=(flags&32)!=0,rek=(flags&64)!=0,ent=(flags&128)!=0;
  CFile f;memset(&f,0,sizeof(f));f.len=m==0?0:80;memcpy(f.data,m==2?"BBBB":"TEST",4);unsigned long fields[]={format?7u:6u,version?19u:18u,valid?1u:2u,align?16u:8u,14u,4u,48u};memcpy(f.data+4,fields,sizeof(fields));
  CBankFile b;b.BankFile=&f;b.BankFormatVersion=7;b.Alignment=16;b.HeaderSize=b.FileSize=999;b.ReadOnly=ro;b.recover=b.entries=0;b.rOK=rek;b.eOK=ent;b.log=0;
  converted[0]=0;int r=0;const char* msg=0;CCharString errmsg;
  try {r=b.ReadFileInformation(ex)?1:0;} catch(const CBankFileException& e) {r=2+(int)ExceptionTypeAccess::Read(e);errmsg=CCharString(converted);}
  int want,read=0,rec=0;unsigned long ea=16,eh=999,es=999;const char* em=0;
  if(m!=2){want=ex?2:0;if(ex)em="fixture.bank is not a valid resource file!";}
  else if(!format){want=ro?(ex?3:0):1;if(!ro)msg="Bank file version has changed, requiring rebuild";else if(ex)em="fixture.bank has the wrong bank format! (should be 7 not 6)";}
  else if(!version){want=ro?(ex?3:0):1;if(!ro)msg="Bank data version has changed, requiring rebuild";else if(ex)em="fixture.bank has the wrong version number! (should be 19 not 18)";}
  else if(!align&&!ro){want=1;msg="Data alignment has changed, requiring rebuild";}
  else {ea=align?16:8;eh=32;es=80;if(valid){want=ent?1:0;read=1;}else if(ro)want=ex?2:0;else {want=1;rec=1;if(!rek)msg="Previous bank update was interrupted, requiring rebuild";}}
  if(r!=want||b.entries!=read||b.recover!=rec||b.Alignment!=ea||b.HeaderSize!=eh||b.FileSize!=es||((b.log==0)!=(msg==0))||(msg&&strcmp(b.log,msg))||(em&&strcmp(errmsg.value,em)))return 1;
 }
 puts("009d0880_TEST PASS");return 0;
}