#include <cstdio>
#include <cwchar>
#include <cstdarg>
#include <string>
#include <vector>
#include <cstdlib>
static int resize_calls,format_calls,alloc_calls,free_calls,data_deletes,mode;
static size_t requested_size,requested_count;
static const wchar_t* seen_format;
static int FormatBoundary(wchar_t* out,size_t count,const wchar_t* format,va_list args){
 ++format_calls;requested_count=count;seen_format=format;
 if(mode==0){out[0]=L'O';out[1]=L'K';out[2]=0;return 2;}
 if(mode==1){for(size_t i=0;i<count;++i)out[i]=L'X';return -1;}
 if(mode==2){out[0]=L'N';out[1]=0;return (int)count+2;}
 if(mode==3){out[0]=0;return 0;}
#ifdef _MSC_VER
 return _vsnwprintf(out,count,format,args);
#else
 return vswprintf(out,count,format,args);
#endif
}
#define _vsnwprintf FormatBoundary
template<class T>class CArray{std::vector<T> data;public:void resize(size_t n){++resize_calls;requested_size=n;data.resize(n);}T&operator[](size_t n){return data[n];}};
class CWideStringData{public:int NoRefs;std::wstring Text;CWideStringData(const wchar_t*p):NoRefs(1),Text(p){}~CWideStringData(){++free_calls;++data_deletes;}};
class CWideString{public:CWideStringData*PStringData;CWideString():PStringData(0){}~CWideString(){UnassignString();}void Sprintf(const wchar_t* string,...);
 CWideStringData*AllocStringData(const wchar_t* p,long length){++alloc_calls;return new CWideStringData(p);}
 void UnassignString(){if(PStringData){CWideStringData*data=PStringData;--data->NoRefs;if(data->NoRefs<=0)delete PStringData;PStringData=0;}}
 void SetString(const wchar_t* string,long length=-1){if(PStringData)UnassignString();if(!PStringData&&string&&*string)PStringData=AllocStringData(string,length);}
 const std::wstring Value()const{return PStringData?PStringData->Text:std::wstring();}
};
void CWideString::Sprintf(const wchar_t* string,...)
{
 const long HALF_STATIC_BUFFER_SIZE=4000;
 wchar_t sbuffer[HALF_STATIC_BUFFER_SIZE*2];
 CArray<wchar_t> dbuffer;
 wchar_t* temp_str=0;
 const long fmtlen=wcslen(string);
 if(fmtlen>HALF_STATIC_BUFFER_SIZE){
  dbuffer.resize(fmtlen+HALF_STATIC_BUFFER_SIZE);
  temp_str=&dbuffer[0];
 }else temp_str=sbuffer;
 long buffer_char_length=fmtlen+HALF_STATIC_BUFFER_SIZE;
 va_list arg_ptr;
 va_start(arg_ptr,string);
 long no_chars=_vsnwprintf(temp_str,buffer_char_length-1,string,arg_ptr);
 temp_str[buffer_char_length-1]=0;
 va_end(arg_ptr);
 if(no_chars<=buffer_char_length)SetString(temp_str);
}

int main(){bool ok=true;int cases=0;const int lengths[]={0,1,10,3999,4000,4001,8001};
 for(int n=0;n<7;++n)for(int old=0;old<3;++old)for(mode=0;mode<4;++mode){
  CWideString s;CWideStringData*shared=0;
  if(old){s.PStringData=new CWideStringData(L"before");if(old==2){shared=s.PStringData;shared->NoRefs=2;}}
  std::wstring format(lengths[n],L'a');resize_calls=format_calls=alloc_calls=free_calls=data_deletes=0;requested_size=requested_count=0;seen_format=0;
  s.Sprintf(format.c_str());size_t capacity=lengths[n]+4000;
  ok &= format_calls==1&&seen_format==format.c_str()&&requested_count==capacity-1;
  ok &= resize_calls==(lengths[n]>4000?1:0)&&requested_size==(lengths[n]>4000?capacity:0);
  if(mode==2){ok &= s.Value()==(old?L"before":L"")&&alloc_calls==0&&free_calls==0;if(shared)ok &= shared->NoRefs==2;}
  else {std::wstring expected=mode==0?L"OK":mode==1?std::wstring(capacity-1,L'X'):L"";ok &= s.Value()==expected&&alloc_calls==(mode==3?0:1)&&free_calls==(old==1?1:0)&&data_deletes==free_calls;if(shared)ok &= shared->NoRefs==1;}
  if(shared){--shared->NoRefs;if(!shared->NoRefs)delete shared;}
  ++cases;
 }
 mode=4;{CWideString s;s.Sprintf(L"value %ld / %lc",42L,(wint_t)L'Z');ok &= s.Value()==L"value 42 / Z";++cases;s.Sprintf(L"%ls",L"");ok &= s.Value().empty();++cases;}
 ok &= cases==86;std::printf(ok?"0099ba70_TEST PASS\n":"0099ba70_TEST FAIL\n");return ok?0:1;
}