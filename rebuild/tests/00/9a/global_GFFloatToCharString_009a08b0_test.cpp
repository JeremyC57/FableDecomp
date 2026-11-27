#include <stdio.h>
#include <stdarg.h>
#include <float.h>
#include <string.h>
#include <assert.h>
#define FABLE_FASTCALL __fastcall
template<class T> class CArray {
    T* data;
public:
    explicit CArray(unsigned n):data(new T[n]) { memset(data,0,n*sizeof(T)); }
    ~CArray() { delete[] data; }
    T& operator[](unsigned i) { return data[i]; }
};
class Basic {
    char* p;
    unsigned long length;
public:
    Basic(const char* s):p(0),length(strlen(s)){p=new char[length+1];memcpy(p,s,length+1);}
    Basic(const char* s,long n):p(0),length(n){p=new char[n+1];memcpy(p,s,n);p[n]=0;}
    ~Basic(){delete[] p;}
    const char* PeekData()const{return p;}
    unsigned long GetLength()const{return length;}
    void Truncate(unsigned long n){assert(n<length);length=n;p[n]=0;}
    void Append(char c,unsigned long n){char* q=new char[length+n+1];memcpy(q,p,length);memset(q+length,c,n);length+=n;q[length]=0;delete[]p;p=q;}
};
class CCharStringData{
    long refs;
public:
    Basic Data;CCharStringData(const char* s):refs(1),Data(s){}CCharStringData(const char* s,long n):refs(1),Data(s,n){}
    void IncRef(){++refs;}void DecRef(){--refs;}long GetRefs()const{return refs;}
};
class CCharString{
    CCharStringData* PStringData;
    void UnassignString();void SetString(const CCharString&);void SetString(const char*,long);
    void MakeOwnCopyOfData();CCharStringData* AllocStringData(const char*,long);static void FABLE_FASTCALL InitMemory();
public:
    static long NoStrings;CCharString();CCharString(const CCharString&);~CCharString();
    long GetLength()const{return PStringData?PStringData->Data.GetLength():0;}
    const char* GetAsCharArray()const{return PStringData?PStringData->Data.PeekData():"";}
    void Sprintf(const char*,...);const CCharString& operator=(const char*);const CCharString& operator+=(char);void Truncate(unsigned long);
};
long CCharString::NoStrings=0;
inline void FABLE_FASTCALL CCharString::InitMemory() { ++NoStrings; }
inline CCharString::CCharString() : PStringData(0) { InitMemory(); }
inline void CCharString::UnassignString()
{
    CCharStringData* data=PStringData;
    if (data) {
        data->DecRef();
        if (data->GetRefs() <= 0)
            delete PStringData;
        PStringData = 0;
    }
}
inline CCharString::~CCharString() { UnassignString(); --NoStrings; }
inline void CCharString::SetString(const CCharString& other)
{
    if (&other != this) {
        #pragma inline_depth(0)
        if (PStringData) UnassignString();
#pragma inline_depth()
        if (other.PStringData) {
            PStringData = other.PStringData;
            PStringData->IncRef();
        }
    }
}
inline CCharString::CCharString(const CCharString& other) : PStringData(0) { InitMemory(); SetString(other); }
inline CCharStringData* CCharString::AllocStringData(const char* text, long length)
{
    if (length == -1) return new CCharStringData(text);
    return new CCharStringData(text, length);
}
inline void CCharString::MakeOwnCopyOfData()
{
    if (PStringData && PStringData->GetRefs() > 1) {
        CCharStringData* copy = AllocStringData(PStringData->Data.PeekData(), -1);
#pragma inline_depth(0)
        UnassignString();
#pragma inline_depth()
        PStringData = copy;
    }
}
inline void CCharString::SetString(const char* text,long length) {
    if(PStringData) UnassignString();
    if(text && *text) PStringData=AllocStringData(text,length);
}
inline const CCharString& CCharString::operator=(const char* text) {
    SetString(text,-1);
    return *this;
}
inline const CCharString& CCharString::operator+=(char ch)
{
    if (PStringData) {
        MakeOwnCopyOfData();
        PStringData->Data.Append(ch,1);
    } else {
        char text[2] = {ch, 0};
        SetString(text, -1);
    }
    return *this;
}
inline void CCharString::Truncate(unsigned long length)
{
    if (static_cast<unsigned long>(GetLength()) > length) {
        if (length == 0) *this = "";
        else {
            MakeOwnCopyOfData();
            PStringData->Data.Truncate(length);
        }
    }
}

#include <stdarg.h>
#include <stdio.h>
inline void __cdecl CCharString::Sprintf(const char* format, ...) {
    long size = strlen(format) + 1024;
    CArray<char> buffer(size);
    va_list args;
    va_start(args,format);
    int count=_vsnprintf(&buffer[0],size,format,args);
    va_end(args);
    buffer[size-1]=0;
    if(count<=size)SetString(&buffer[0],-1);
}
CCharString FABLE_FASTCALL GFFloatToCharString(double n, long precision)
{
    CCharString temp;
    bool nip_trailing_zeroes = false;
    if (_isnan(n))
        temp.Sprintf("%.*f", precision, 0.0);
    else
        temp.Sprintf("%.*f", precision, n);

    for (long c1 = 0; c1 < temp.GetLength(); ++c1) {
        if (temp.GetAsCharArray()[c1] == '.') {
            nip_trailing_zeroes = true;
            break;
        }
    }
    if (nip_trailing_zeroes) {
        while (temp.GetLength() > 0) {
            char ch = temp.GetAsCharArray()[temp.GetLength() - 1];
            if (ch == '.') {
                temp += '0';
                break;
            } else if (ch == '0') {
                temp.Truncate(temp.GetLength() - 1);
            } else {
                break;
            }
        }
    }
    return temp;
}

static void check(double value,int precision){
    char expected[1030]={0};
    _snprintf(expected,1028,"%.*f",precision,_isnan(value)?0.0:value);expected[1027]=0;
    char* dot=strchr(expected,'.');
    if(dot){char* end=expected+strlen(expected);while(end>dot+1 && end[-1]=='0')--end;if(end==dot+1)*end++='0';*end=0;}
    CCharString result=GFFloatToCharString(value,precision);assert(!strcmp(result.GetAsCharArray(),expected));
}
int main(){
    double zero=0.0;double numbers[]={0.0,-0.0,1.0,-1.0,2.5,-2.5,0.0025,781.1200,DBL_MIN,DBL_MAX};
    int ps[]={-1,0,1,2,5,9,20,100,1025};
    for(unsigned i=0;i<sizeof(numbers)/sizeof(numbers[0]);++i)
        for(unsigned j=0;j<sizeof(ps)/sizeof(ps[0]);++j)check(numbers[i],ps[j]);
    for(int v=-225;v<=225;++v)for(int j=0;j<=8;++j)check(v/20.0,j);
    check(zero/zero,2);check(1.0/zero,2);check(-1.0/zero,3);
    {CCharString a;a="123.00";CCharString b=a;b.Truncate(3);assert(!strcmp(a.GetAsCharArray(),"123.00") && !strcmp(b.GetAsCharArray(),"123"));b.Truncate(0);b+='x';assert(!strcmp(b.GetAsCharArray(),"x"));}
    assert(CCharString::NoStrings==0);puts("009a08b0_TEST PASS");return 0;
}