#include <string>
#include <wchar.h>
#include <stdlib.h>
#include <stdio.h>
static void check(bool passed) { if (!passed) abort(); }
struct CWideStringData {
    std::wstring text;
    int NoRefs;
    CWideStringData(const wchar_t* s, size_t n):text(s,n),NoRefs(1) {}
    void Append(const wchar_t* s,long n) { text.append(s, n<0?wcslen(s):(size_t)n); }
};
class CWideString {
    CWideStringData* PStringData;
    static long NoStrings;
    void UnassignString() { if(PStringData && --PStringData->NoRefs==0)delete PStringData; PStringData=0; }
    void MakeOwnCopyOfData() {
        if(PStringData && PStringData->NoRefs>1) {
            CWideStringData* prior=PStringData;
            PStringData=new CWideStringData(prior->text.c_str(),prior->text.size());
            --prior->NoRefs;
        }
    }
public:
    CWideString(); ~CWideString();
    explicit CWideString(const wchar_t* s):PStringData(0){++NoStrings; SetAsSubString(s,-1);}
    CWideString(const CWideString& other):PStringData(other.PStringData){++NoStrings;if(PStringData)++PStringData->NoRefs;}
    const wchar_t* GetAsCharArray()const{return PStringData?PStringData->text.c_str():L"";}
    void SetAsSubString(const wchar_t* s,long length){UnassignString();if(s&&*s)PStringData=new CWideStringData(s,length<0?wcslen(s):(size_t)length);}
    static long Live(){return NoStrings;}
    const CWideString& operator=(const CWideString&);
    const CWideString& operator+=(const CWideString&);
    void Replace(const CWideString&,const CWideString&);
};
long CWideString::NoStrings=0;
CWideString::CWideString() : PStringData(0) { ++NoStrings; }
CWideString::~CWideString() { UnassignString(); --NoStrings; }

const CWideString& CWideString::operator=(const CWideString& other)
{
    if (this != &other && PStringData != other.PStringData) {
        if (PStringData) UnassignString();
        if (other.PStringData) {
            PStringData = other.PStringData;
            ++PStringData->NoRefs;
        }
    }
    return *this;
}

const CWideString& CWideString::operator+=(const CWideString& other)
{
    if (PStringData) {
        MakeOwnCopyOfData();
        PStringData->Append(other.GetAsCharArray(), -1);
    } else if (&other != this && other.PStringData) {
        PStringData = other.PStringData;
        ++PStringData->NoRefs;
    }
    return *this;
}

void CWideString::Replace(const CWideString& What, const CWideString& With)
{
    CWideString ret_str;
    wchar_t* data = const_cast<wchar_t*>(GetAsCharArray());
    wchar_t* what = const_cast<wchar_t*>(What.GetAsCharArray());
    wchar_t* prev = data;
    wchar_t* res = wcsstr(data,what);
    while (res) {
        { CWideString temp;
        temp.SetAsSubString(prev, res - prev);
        res += wcslen(what);
        ret_str += temp; }
        ret_str += With;
        prev = res;
        res = wcsstr(res,what);
    }
    ret_str += CWideString(prev);
    *this = ret_str;
}

static void run(const wchar_t* input,const wchar_t* needle,const wchar_t* replacement,const wchar_t* expected){
    CWideString value(input), original(value), what(needle), with(replacement);
    value.Replace(what,with);
    check(wcscmp(value.GetAsCharArray(),expected)==0 && wcscmp(original.GetAsCharArray(),input)==0);
    check(wcscmp(what.GetAsCharArray(),needle)==0 && wcscmp(with.GetAsCharArray(),replacement)==0 && CWideString::Live()==4);
}
int main(){
    run(L"",L"a",L"z",L""); run(L"ab",L"c",L"x",L"ab");
    run(L"aba",L"a",L"x",L"xbx");run(L"aaaaa",L"aa",L"b",L"bba");
    run(L"xxaxx",L"x",L"",L"a");run(L"xyxy",L"xy",L"",L"");
    run(L"a",L"a",L"aa",L"aa");run(L"prefix-middle-tail",L"middle",L"new",L"prefix-new-tail");
    run(L"xy",L"xy",L"xy",L"xy");
    {CWideString v(L"abc"),with(L"z");v.Replace(v,with);check(!wcscmp(v.GetAsCharArray(),L"z"));}
    {CWideString v(L"aba"),what(L"b");v.Replace(what,v);check(!wcscmp(v.GetAsCharArray(),L"aabaa"));}
    {CWideString v(L"abc");v.Replace(v,v);check(!wcscmp(v.GetAsCharArray(),L"abc"));}
    check(CWideString::Live()==0);puts("0099c1e0_TEST PASS");return 0;
}
