// Self-contained C++03 fixture with actual selected row and native class headers pasted.
// Host long is pinned to32bits; host CRT path splitting is a checked Windows-semantics contract.
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <new>
#ifndef _MSC_VER
#define __declspec(x)
#define __fastcall
#define __cdecl
#define __forceinline inline
#define long int
#define FABLE_STATIC_ASSERT(x) typedef char fixture_assert_##__LINE__[1]
#define _MAX_DRIVE 3
#define _MAX_DIR 256
#define _MAX_FNAME 256
#define _MAX_EXT 256
extern "C" void _splitpath(const char*,char*,char*,char*,char*);
#else
#define FABLE_STATIC_ASSERT(x) typedef char fixture_assert_##__LINE__[(x)?1:-1]
#endif
#define FABLE_FASTCALL __fastcall
#define private public
#define protected public
// CAFile::GetRelativeFilePath(CCharString const &,CCharString const &) -- construct the file path relative to the supplied base path.
// Leaf: ?GetRelativeFilePath@CAFile@@SI?AVCCharString@@ABV2@0@Z
// Flags: /GS /Gr
#include <stdlib.h>
// Retail declarations (Codex pass 11): Ego_r TPI 0x27dc, class key V,
// sizeof 4, protected PStringData +0; native array construction advances by 4.
#pragma once

#include <stddef.h>



#pragma pack(push, 1)
// Retail opt-in (Codex pass11): original Ego_r TPI fields and native
// CCharString memory accounting confirm13byte data and17byte refcount owner.
// This branch reuses the existing packed scope; default declarations stay intact.
template<class T> class CBasicString {
protected:
    T* PData;
    unsigned long StringLength;
    unsigned long DataLength : 31;
    bool UseFastExtend : 1;
    static long __fastcall Compare(const T*,const T*);
public:
    const T* PeekData() const { return PData; }
    bool operator==(const CBasicString& other) const { if(StringLength!=other.StringLength)return false; return !Compare(PData,other.PData); }
};
class CCharStringData {
public:
    CBasicString<char> Data;
protected:
    long NoRefs;
};
#pragma pack(pop)

class CCharStringData;
class CCharString {
public:
    CCharString();
    CCharString(const char* text, long length = -1);
    CCharString(const CCharString& other);
    operator const char*() const;
    bool operator==(const CCharString& other) const {
        if(PStringData==other.PStringData)return true;
        if(!PStringData||!other.PStringData)return false;
        return PStringData->Data==other.PStringData->Data;
    }
    bool operator!=(const CCharString&) const;
    bool operator!=(const char*) const;
    const CCharString& operator=(const char*);
    const CCharString& operator=(const CCharString&);
    const CCharString& operator+=(const char*);
    const CCharString& operator+=(const CCharString&);
    ~CCharString();
protected:
    CCharStringData* PStringData;
};
// Retail layout (hand-written; no members). RTTI CBaseClassNonCopyable : CBaseClass; vtable 0x0129A7C4, slot 0 scalar
// deleting dtor 0x0099A470. Ego_r.pdb / FableWin.pdb: ??0CBaseClassNonCopyable@@IAE@XZ (protected),
// ??1CBaseClassNonCopyable@@MAE@XZ (protected virtual). The copy operations are private and undefined (non-copyable).
// Retail layout (hand-written; Ego_r.pdb has no members): the root polymorphic base. RTTI .?AVCBaseClass@@ (no bases);
// vtable 0x01231710, slot 0 scalar deleting dtor 0x004371F0, which calls ~CBaseClass 0x0099A2E0 (one vtable store).
// The default ctor is out of line (0x0099A2D0: `mov eax,ecx; mov [eax],0x01231710; ret`): Ego_r.pdb places
// ??0CBaseClass@@QAE@XZ / ??1CBaseClass@@UAE@XZ side by side (0x00B01F20 / 0x00B01F30, one .cpp) and FableWin.pdb does the
// same (0x03044700 / 0x03044720), and 1,225 landed rows call 0x0099A2D0. The copy ctor is the implicit one: FableWin.pdb
// emits ??0CBaseClass@@QAE@ABV0@@Z beside the compiler-generated ??_E (0x018CD7E0), and retail expands it inline (a bare
// vtable store in a derived copy ctor, landed chunk 577 0x00DCEDF0 / chunk 578 0x00F05F30: write `: CBaseClass(other)`).

class CBaseClass {
public:
    CBaseClass();                                                        // 0x0099A2D0
    virtual ~CBaseClass();                                               // slot 0  0x0099A2E0
};
FABLE_STATIC_ASSERT(sizeof(CBaseClass) == 0x4);


class CBaseClassNonCopyable : public CBaseClass {
protected:
    CBaseClassNonCopyable();
    virtual ~CBaseClassNonCopyable();                                    // slot 0
private:
    CBaseClassNonCopyable(const CBaseClassNonCopyable&);
    CBaseClassNonCopyable& operator=(const CBaseClassNonCopyable&);
};
FABLE_STATIC_ASSERT(sizeof(CBaseClassNonCopyable) == 0x4);

// Original PDB CWideString, class key V, one protected pointer at +0.
class CCharString;
class CWideStringData;
class CWideString {
public:
    ~CWideString();
protected:
    CWideStringData* PStringData;
};
class CCharString;
class CASuspendableProcess;
class CFileInformation;
enum FileAccessType { FILE_ACCESS_NULL=0, FILE_ACCESS_READ=1, FILE_ACCESS_READ_WRITE=2, FILE_ACCESS_READ_WRITE_CREATE=3, FILE_ACCESS_CREATE=4 };
// Original CAFile virtual order from Ego_r TPI and native CDiskFileWin32 vtable.
class CAFile : public CBaseClassNonCopyable {
public:
    static CCharString __fastcall GetRelativeFilePath(const CCharString&,const CCharString&);
    static void __fastcall SplitPath(const CCharString&,CCharString*,CCharString*,CCharString*,CCharString*);
    static bool __fastcall PathExists(const CWideString&);
    static bool __fastcall IsPathFile(const CWideString&);
    virtual void Open(const CWideString&,FileAccessType,unsigned long)=0;
    virtual void Close()=0;
    virtual void Read(void*,long,CASuspendableProcess*)=0;
    virtual void Write(const void*,long,CASuspendableProcess*)=0;
    virtual void SetPosition(long)=0;
    virtual void MovePosition(long)=0;
    virtual long GetPosition() const=0;
    virtual void SetLength(long)=0;
    virtual long GetLength() const=0;
    virtual bool IsOpen() const=0;
    virtual CWideString GetPathname() const=0;
    virtual bool IsIOFinished() const;
    virtual long GetAlignment() const;
    virtual CAFile* Clone() const=0;
    virtual CFileInformation GetFileInformation();
};
// Retail opt-in (Codex pass11): original TPI base and names; native offsets
// +4/+8/+c/+10/+14/+18/+1c and total40 after STLport list correction.


// STLport 4.6.2 excerpts: stl/_alloc.h, _vector.h, _list.h, _bitset.h,
// _pair.h and _function_base.h. Original inheritance, field names and keys.
// Only declarations and operations needed by these translation units appear.
#include <stddef.h>
#include <stdlib.h>
namespace std {
struct __true_type { };
template<class _Tp> class allocator {
public:
    _Tp* allocate(size_t n,const void* =0) { return n?(_Tp*)malloc(n*sizeof(_Tp)):0; }
    void deallocate(_Tp* p, size_t) {
        free(p);
    }
};
template<class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& a, _Value p)
        : _MaybeReboundAlloc(a), _M_data(p) { }
};
template<class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(const _Alloc& a)
        : _M_start(0), _M_finish(0), _M_end_of_storage(a,0) { }
    ~_Vector_base() {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data-_M_start);
    }
protected:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*,_Tp,_Alloc> _M_end_of_storage;
};
template<class _Tp, class _Alloc=allocator<_Tp> >
class vector : public _Vector_base<_Tp,_Alloc> {
public:
    typedef _Tp* iterator;
    typedef const _Tp* const_iterator;
    typedef _Tp value_type;
    typedef size_t size_type;
    explicit vector(const _Alloc& a=_Alloc()) : _Vector_base<_Tp,_Alloc>(a) { }
    iterator begin() { return this->_M_start; }
    const_iterator begin() const { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    const_iterator end() const { return this->_M_finish; }
    size_type size() const { return size_type(this->_M_finish-this->_M_start); }
    bool empty() const { return this->_M_start==this->_M_finish; }
    _Tp& operator[](size_type n) { return *(begin()+n); }
    const _Tp& operator[](size_type n) const { return *(begin()+n); }
    void push_back(const _Tp&);
};
template<class _T1,class _T2> struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;
    _T2 second;
};
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template<class _Tp> struct _List_node : public _List_node_base { _Tp _M_data; };
template<class _Tp> struct _Nonconst_traits { typedef _Tp* pointer; typedef _Tp& reference; };
template<class _Tp> struct _Const_traits { typedef const _Tp* pointer; typedef const _Tp& reference; };
struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* p) : _M_node(p) { }
    _List_iterator_base() { }
    void _M_incr() { _M_node=_M_node->_M_next; }
    bool operator==(const _List_iterator_base& y) const { return _M_node==y._M_node; }
    bool operator!=(const _List_iterator_base& y) const { return _M_node!=y._M_node; }
};
template<class _Tp,class _Traits> struct _List_iterator : public _List_iterator_base {
    typedef typename _Traits::reference reference;
    typedef typename _Traits::pointer pointer;
    typedef _List_iterator<_Tp,_Nonconst_traits<_Tp> > iterator;
    _List_iterator(_List_node_base* x) : _List_iterator_base(x) { }
    _List_iterator() { }
    reference operator*() const { return ((_List_node<_Tp>*)_M_node)->_M_data; }
    pointer operator->() const { return &operator*(); }
    _List_iterator& operator++() { this->_M_incr(); return *this; }
};
template<class _Tp,class _Alloc> class _List_base {
public:
    _List_base(const _Alloc&) : _M_node(allocator<_List_node<_Tp> >(),0) {
        _List_node<_Tp>* n=_M_node.allocate(1);n->_M_next=n;n->_M_prev=n;_M_node._M_data=n;
    }
    ~_List_base() { clear();_M_node.deallocate(_M_node._M_data,1); }
    _STLP_alloc_proxy<_List_node<_Tp>*,_List_node<_Tp>,allocator<_List_node<_Tp> > > _M_node;
    void clear();
};
template<class _Tp,class _Alloc=allocator<_Tp> > class list : public _List_base<_Tp,_Alloc> {
public:
    ~list();
    explicit list(const _Alloc& a=_Alloc()) : _List_base<_Tp,_Alloc>(a) { }
    typedef _List_iterator<_Tp,_Nonconst_traits<_Tp> > iterator;
    typedef _List_iterator<_Tp,_Const_traits<_Tp> > const_iterator;
    iterator begin() { return iterator(this->_M_node._M_data->_M_next); }
    const_iterator begin() const { return const_iterator(this->_M_node._M_data->_M_next); }
    iterator end() { return iterator(this->_M_node._M_data); }
    const_iterator end() const { return const_iterator(this->_M_node._M_data); }
    bool empty() const { return this->_M_node._M_data->_M_next==this->_M_node._M_data; }
};
template<size_t _Nw> struct _Base_bitset {
    typedef unsigned long _WordT;
    _WordT _M_w[_Nw];
    static size_t _S_whichword(size_t n) { return n/(8*sizeof(_WordT)); }
    static size_t _S_whichbit(size_t n) { return n%(8*sizeof(_WordT)); }
    static _WordT _S_maskbit(size_t n) { return _WordT(1)<<_S_whichbit(n); }
    _WordT _M_getword(size_t n) const { return _M_w[_S_whichword(n)]; }
};
void __stl_throw_out_of_range(const char*);
template<size_t _Nb> class bitset : public _Base_bitset<(_Nb+8*sizeof(unsigned long)-1)/(8*sizeof(unsigned long))> {
public:
    bool _Unchecked_test(size_t n) const { return (this->_M_getword(n)&this->_S_maskbit(n))!=0; }
    bool test(size_t n) const { if(n>=_Nb) __stl_throw_out_of_range("bitset"); return _Unchecked_test(n); }
};
template<class _Arg,class _Result> struct unary_function {
    typedef _Arg argument_type;
    typedef _Result result_type;
};
template<class _RandomAccessIter,class _RandomNumberGenerator>
void __fastcall random_shuffle(_RandomAccessIter,_RandomAccessIter,_RandomNumberGenerator&);
}
class CParsedItem;
// Layout: original pointer boundaries, size8, class keyV.
class CStringParserCommentBoundaries {public:const char* PStart;const char* PEnd;};
class CStringParser : public CBaseClass {
public:
    CStringParser(const CCharString&,void (__fastcall*)(const CCharString&)=0,const CCharString& filename=CCharString(""),long baseLineOffset=0);
    bool ReadAsStringUntilString(const CCharString&,CParsedItem&);
    bool SkipPastString(const CCharString&);
    bool SkipPastNextItemIfIs(const CCharString&);
    bool SkipUntilString(const CCharString&);
    CCharString ReadAsSymbol();
    CCharString ReadAsString();
    class CCursorPos {
    public:
        bool InQuotes;
        unsigned long StringPos,Line;
    };
protected:
    const char* String;
    unsigned long BaseLineOffset;
    CCharString Filename,TempCharString;
    std::list<CStringParserCommentBoundaries> Comments;
    void (__fastcall *ErrorCallback)(const CCharString&);
    CCursorPos CursorPos;
};
FABLE_STATIC_ASSERT(sizeof(CStringParser)==0x28);

enum EParsedItemType {PARSED_ITEM_NULL=0,PARSED_ITEM_SYMBOL=1,PARSED_ITEM_STRING=2,PARSED_ITEM_INTEGER=3,PARSED_ITEM_FLOAT=4};
// Layout: original Type+0, Integer/Float union+4, actual String+8, full size12.
class CParsedItem {
private:EParsedItemType Type;union{long Integer;float Float;};CCharString String;
public:CParsedItem(){} const CCharString& GetString() const;
};
FABLE_STATIC_ASSERT(sizeof(CParsedItem)==12);
CCharString __fastcall operator+(const CCharString&,const CCharString&);
CCharString __fastcall operator+(const CCharString&,const char*);
void __fastcall CAFile::SplitPath(const CCharString& path,CCharString* drive,CCharString* directory,CCharString* filename,CCharString* extension)
{
 char drive_buffer[_MAX_DRIVE],directory_buffer[_MAX_DIR],filename_buffer[_MAX_FNAME],extension_buffer[_MAX_EXT];
 _splitpath(path,drive_buffer,directory_buffer,filename_buffer,extension_buffer);
 if(drive)*drive=drive_buffer;
 if(directory)*directory=directory_buffer;
 if(filename)*filename=filename_buffer;
 if(extension)*extension=extension_buffer;
}
CCharString __fastcall CAFile::GetRelativeFilePath(const CCharString& path1_absolute,const CCharString& path2_absolute)
{
 if(path1_absolute==path2_absolute)return CCharString("");
 CCharString path1_drive,path1_directory;
 SplitPath(path1_absolute,&path1_drive,&path1_directory,0,0);
 CCharString path2_drive,path2_directory,path2_filename,path2_extension;
 SplitPath(path2_absolute,&path2_drive,&path2_directory,&path2_filename,&path2_extension);
 CCharString path_relative;
 bool path_completed=false;
 CStringParser parser1(path1_directory),parser2(path2_directory);
 while(!path_completed) {
  CParsedItem parsed_item1,parsed_item2;
  bool directory_name_found_1=parser1.ReadAsStringUntilString("\\",parsed_item1);
  bool directory_name_found_2=parser2.ReadAsStringUntilString("\\",parsed_item2);
  if(!directory_name_found_1&&!directory_name_found_2) {
   path_relative=path2_filename+path2_extension;
   path_completed=true;
  } else {
   parser1.SkipPastString("\\");
   parser2.SkipPastString("\\");
   if(!directory_name_found_1||!directory_name_found_2||parsed_item1.GetString()!=parsed_item2.GetString()) {
    if(directory_name_found_1) {
     do {
      parser1.SkipPastNextItemIfIs("\\");
      path_relative+="..\\";
      directory_name_found_1=parser1.SkipUntilString("\\");
     } while(directory_name_found_1);
    }
    if(directory_name_found_2) {
     do {
      parser2.SkipPastNextItemIfIs("\\");
      if(parsed_item2.GetString()!="")path_relative+=parsed_item2.GetString()+"\\";
      directory_name_found_2=parser2.ReadAsStringUntilString("\\",parsed_item2);
     } while(directory_name_found_2);
    }
    path_relative+=path2_filename+path2_extension;
    path_completed=true;
   }
  }
 }
 return path_relative;
}

static int liveStrings=0,liveLists=0,bases=0,parsers=0,splitCalls=0,comparisons=0;
static void drop(CCharStringData* p){if(p&&!--p->NoRefs){delete[]p->Data.PData;delete p;--liveStrings;}}
static CCharStringData* textData(const char* p,long n){if(n<0)n=(long)strlen(p);CCharStringData* d=new CCharStringData;d->Data.PData=new char[n+1];memcpy(d->Data.PData,p,n);d->Data.PData[n]=0;d->Data.StringLength=(unsigned long)n;d->Data.DataLength=n+1;d->Data.UseFastExtend=false;d->NoRefs=1;++liveStrings;return d;}
CCharString::CCharString():PStringData(0){}
CCharString::CCharString(const char* p,long n):PStringData(textData(p,n)){}
CCharString::CCharString(const CCharString& other):PStringData(other.PStringData){if(PStringData)++PStringData->NoRefs;}
CCharString::~CCharString(){drop(PStringData);}
CCharString::operator const char*()const{return PStringData?PStringData->Data.PData:"";}
const CCharString& CCharString::operator=(const CCharString& other){if(other.PStringData)++other.PStringData->NoRefs;drop(PStringData);PStringData=other.PStringData;return *this;}
const CCharString& CCharString::operator=(const char* p){CCharString replacement(p);return *this=replacement;}
const CCharString& CCharString::operator+=(const char* p){unsigned long a=(unsigned long)strlen(*this),b=(unsigned long)strlen(p);char* out=new char[a+b+1];memcpy(out,(const char*)*this,a);memcpy(out+a,p,b+1);*this=out;delete[]out;return *this;}
const CCharString& CCharString::operator+=(const CCharString& other){return *this+=(const char*)other;}
bool CCharString::operator!=(const CCharString& other)const{return strcmp(*this,other)!=0;}
bool CCharString::operator!=(const char* p)const{return strcmp(*this,p)!=0;}
template<class T>long __fastcall CBasicString<T>::Compare(const T* a,const T* b){++comparisons;return strcmp(a,b);}
CCharString __fastcall operator+(const CCharString& a,const CCharString& b){CCharString r(a);r+=b;return r;}
CCharString __fastcall operator+(const CCharString& a,const char* b){CCharString r(a);r+=b;return r;}
CBaseClass::CBaseClass(){++bases;}CBaseClass::~CBaseClass(){--bases;}
namespace std {
template<class T,class A>void _List_base<T,A>::clear(){assert(this->_M_node._M_data->_M_next==this->_M_node._M_data&&this->_M_node._M_data->_M_prev==this->_M_node._M_data);}
template<class T,class A>list<T,A>::~list(){--liveLists;}
}
CStringParser::CStringParser(const CCharString& text,void (__fastcall* callback)(const CCharString&),const CCharString& filename,long baseline):String(text),BaseLineOffset(baseline),Filename(filename),ErrorCallback(callback){CursorPos.InQuotes=false;CursorPos.StringPos=0;CursorPos.Line=0;++liveLists;++parsers;}
const CCharString& CParsedItem::GetString()const{assert(Type==PARSED_ITEM_STRING);return String;}
bool CStringParser::SkipUntilString(const CCharString& delimiter){const char* found=strstr(String+CursorPos.StringPos,delimiter);if(found){CursorPos.StringPos=(unsigned long)(found-String);return true;}CursorPos.StringPos=(unsigned long)strlen(String);return false;}
bool CStringParser::SkipPastString(const CCharString& delimiter){if(!SkipUntilString(delimiter))return false;CursorPos.StringPos+=(unsigned long)strlen(delimiter);return true;}
bool CStringParser::SkipPastNextItemIfIs(const CCharString& delimiter){unsigned long n=(unsigned long)strlen(delimiter);if(strncmp(String+CursorPos.StringPos,delimiter,n))return false;CursorPos.StringPos+=n;return true;}
bool CStringParser::ReadAsStringUntilString(const CCharString& delimiter,CParsedItem& item){unsigned long begin=CursorPos.StringPos;if(!SkipUntilString(delimiter))return false;item.String=CCharString(String+begin,(long)(CursorPos.StringPos-begin));item.Type=PARSED_ITEM_STRING;return true;}
#ifndef _MSC_VER
extern "C" void _splitpath(const char* path,char* drive,char* directory,char* filename,char* extension){
 ++splitCalls;const char* p=path;if(p[0]&&p[1]==':'){drive[0]=p[0];drive[1]=':';drive[2]=0;p+=2;}else drive[0]=0;
 const char* last=0;for(const char* a=p;*a;++a)if(*a=='\\'||*a=='/')last=a;
 const char* file=last?last+1:p;unsigned int n=(unsigned int)(file-p);memcpy(directory,p,n);directory[n]=0;
 const char* dot=0;for(const char* a=file;*a;++a)if(*a=='.')dot=a;
 if(dot){n=(unsigned int)(dot-file);memcpy(filename,file,n);filename[n]=0;strcpy(extension,dot);}else{strcpy(filename,file);extension[0]=0;}
}
#endif
struct Case{const char* first;const char* second;const char* expected;};
static const Case cases[]={
 {"","",""},{"a.txt","a.txt",""},{"old.txt","new.txt","new.txt"},{"","new.txt","new.txt"},{"old.txt","",""},
 {"C:\\root\\old.txt","C:\\root\\new.txt","new.txt"},
 {"C:\\root\\child\\old.txt","C:\\root\\peer\\new.txt","..\\peer\\new.txt"},
 {"C:\\root\\child\\deep\\old.txt","C:\\root\\new.txt","..\\..\\new.txt"},
 {"C:\\root\\old.txt","C:\\root\\child\\deep\\new.txt","child\\deep\\new.txt"},
 {"C:\\root\\a.txt","D:\\root\\b.txt","b.txt"},
 {"root\\a.txt","other\\b.txt","..\\other\\b.txt"},
 {"C:\\ROOT\\a.txt","C:\\root\\b.txt","..\\root\\b.txt"},
 {"C:\\a.txt","C:\\b.txt","b.txt"},
 {"C:\\a\\b.txt","C:\\c.txt","..\\c.txt"},
 {"C:\\a.txt","C:\\b\\c.txt","b\\c.txt"},
 {"C:\\a\\a\\old","C:\\a\\b\\new.ext","..\\b\\new.ext"},
 {"folder\\old","folder\\.cfg",".cfg"},
 {"folder\\old.tar.gz","folder\\new.tar.gz","new.tar.gz"},
 {"C:\\first\\second\\third\\old","C:\\other\\more\\new","..\\..\\..\\other\\more\\new"},
 {"C:\\root\\a","C:\\root\\directory\\","directory\\"}
};
int main(){unsigned int invocations=0;
 for(unsigned int i=0;i<sizeof(cases)/sizeof(cases[0]);++i)for(unsigned int mode=0;mode<4;++mode){
  int before=liveStrings;{
   CCharString first(cases[i].first),second(cases[i].second);if(mode&1){CCharString copy(first);first=copy;}if(mode&2){CCharString copy(second);second=copy;}
   parsers=splitCalls=comparisons=0;CCharString result=CAFile::GetRelativeFilePath(first,second);++invocations;
   if(strcmp(result,cases[i].expected)){fprintf(stderr,"case%u expected[%s] actual[%s]\n",i,cases[i].expected,(const char*)result);return 1;}
   bool same=!strcmp(cases[i].first,cases[i].second);assert(parsers==(same?0:2));assert(!liveLists&&!bases);
#ifndef _MSC_VER
   assert(splitCalls==(same?0:2));
#endif
   assert(!strcmp(first,cases[i].first)&&!strcmp(second,cases[i].second));
  }assert(liveStrings==before);
 }
 for(unsigned int mode=0;mode<3;++mode){CCharString a=mode==0?CCharString():CCharString(mode==1?"same":"C:\\some\\path.txt");CCharString b(a);CCharString r=CAFile::GetRelativeFilePath(a,b);++invocations;assert(!strcmp(r,"")&&!liveLists&&!bases);}
 for(unsigned int direction=0;direction<2;++direction){CCharString empty;CCharString full("file.txt");CCharString result=direction?CAFile::GetRelativeFilePath(full,empty):CAFile::GetRelativeFilePath(empty,full);++invocations;assert(!strcmp(result,direction?"":"file.txt")&&!liveLists&&!bases);}
 assert(invocations==85&&!liveStrings&&!liveLists&&!bases);
#ifdef _MSC_VER
 assert(sizeof(CBasicString<char>)==13&&sizeof(CCharStringData)==17&&sizeof(CStringParser)==40&&sizeof(CParsedItem)==12&&sizeof(CAFile)==4);
#endif
 puts("00998570_TEST PASS");return 0;}
