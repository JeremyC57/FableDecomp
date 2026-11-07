// Standalone fixture: original active declarations and exact compiled target body.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stddef.h>
#include <new>
#ifndef _MSC_VER
#define __declspec(x)
#define __fastcall
#define __cdecl
#define __forceinline inline
#define long int
#endif
#define FABLE_FASTCALL __fastcall
#define private public
#define protected public
// Original unused return-value types from FableWin TPI b8cb/d458.
class CDateAndTime {protected:long Year;unsigned long Month,Day,Hour,Minute,Second,Millisecond;};
class CFileInformation {public:CDateAndTime CreationTime,LastAccessTime,LastWriteTime;bool ReadOnly;};
#define FABLE_ENGINE_CCOUNTEDPOINTER_H










































class CCPPointerInfo {
public:
    unsigned long RefCount;                                              
    void (FABLE_FASTCALL* DeleteFunc)(void*);                             
    void* Data;                                                          
    CCPPointerInfo(unsigned long refCount, void (FABLE_FASTCALL* deleteFunc)(void*), void* data)
        : RefCount(refCount), DeleteFunc(deleteFunc), Data(data) { }     
};

template <class T> class CCountedPointer {
public:
    T* Data;                                                             
    CCPPointerInfo* Info;                                                

    CCountedPointer() : Data(0), Info(0) { }
    CCountedPointer(T* data) : Data(data)
    {
        if (data)
            Info = new CCPPointerInfo(1, DeleteData, (void*)Data);
        else
            Info = 0;
    }
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->RefCount;
    }
    template <class U> CCountedPointer(const CCountedPointer<U>& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->RefCount;
    }
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        ShareData(other.Data, other.Info);
        return *this;
    }
    template <class U> CCountedPointer& operator=(const CCountedPointer<U>& other)
    {
        ShareData(other.Data, other.Info);
        return *this;
    }
    T* operator->() const { return Data; }
    T& operator*() const { return *Data; }
    T* Get() const { return Data; }
    T* get() const { return Data; }
    bool IsNull() const { return Data == 0; }
    unsigned long GetReferenceCount() const { return Info ? Info->RefCount : 0; }   
    void Validate() const;                                               
    void ReleaseData()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Info = 0;
        Data = 0;
    }
    void ShareData(T* data, CCPPointerInfo* info)
    {
        if (Info != info) {
            ReleaseData();
            Data = data;
            Info = info;
            if (info)
                ++info->RefCount;
        }
    }
    void Reset(T* data)
    {
        ReleaseData();
        Data = data;
        if (data)
            Info = new CCPPointerInfo(1, DeleteData, (void*)Data);
    }
    template <class U> void SetStaticCast(const CCountedPointer<U>& other)   
    {
        ShareData(static_cast<T*>(other.Data), other.Info);              
    }
    void reset(T* data) { Reset(data); }                                 
    static void FABLE_FASTCALL DeleteData(void* data) { delete static_cast<T*>(data); }                    
};



#define FABLE_RETAIL_BANK_SYMBOLS
#define FABLE_RETAIL_CWIDESTRING_CTOR
#define FABLE_RETAIL_CWIDESTRING_COPY
#define FABLE_RETAIL_FILE_DATA_OUTPUT_STREAM
#define FABLE_ENGINE_CBANKFILE_H
#define FABLE_ENGINE_CBASECLASSNONCOPYABLE_H



#define FABLE_ENGINE_CBASECLASS_H








class CBaseClass {
public:
    CBaseClass();                                                        
    virtual ~CBaseClass();                                               
};


class CBaseClassNonCopyable : public CBaseClass {
protected:
    CBaseClassNonCopyable();
    virtual ~CBaseClassNonCopyable();                                    
private:
    CBaseClassNonCopyable(const CBaseClassNonCopyable&);
    CBaseClassNonCopyable& operator=(const CBaseClassNonCopyable&);
};

#define FABLE_ENGINE_CARRAY_H
#define FABLE_STLPORT_BANK_SYMBOLS_H
#include <stddef.h>
#include <stdlib.h>

#include <string.h>

namespace std {
struct __true_type{};
template<class T>inline const T&(max)(const T&a,const T&b){return a<b?b:a;}
inline void*__copy_trivial(const void*first,const void*last,void*result){return first==last?result:(char*)memmove(result,first,(const char*)last-(const char*)first)+((const char*)last-(const char*)first);}
template<class I,class N,class T>inline I fill_n(I first,N n,const T&value){for(;n>0;--n,++first)*first=value;return first;}
template<class T>class allocator {public:T*allocate(size_t n){return n?(T*)malloc(n*sizeof(T)):0;}void deallocate(T*p,size_t){free(p);}};
template<class V,class T,class A>class _STLP_alloc_proxy:public A {public:V _M_data;_STLP_alloc_proxy(const A&a,V p):A(a),_M_data(p){}};
template<class T,class U>inline void _Construct(T*p,const U&x){new(p)T(x);}
template<class T,class A>class _Vector_base {public:_Vector_base(const A&a):_M_start(0),_M_finish(0),_M_end_of_storage(a,0){}~_Vector_base(){if(_M_start)_M_end_of_storage.deallocate(_M_start,_M_end_of_storage._M_data-_M_start);}protected:T*_M_start;T*_M_finish;_STLP_alloc_proxy<T*,T,A>_M_end_of_storage;};
template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A>{public:typedef T*iterator;typedef const T*const_iterator;typedef size_t size_type;explicit vector(const A&a=A()):_Vector_base<T,A>(a){}~vector(){for(iterator p=this->_M_start;p!=this->_M_finish;++p)p->~T();}iterator begin(){return this->_M_start;}const_iterator begin()const{return this->_M_start;}iterator end(){return this->_M_finish;}const_iterator end()const{return this->_M_finish;}size_type size()const{return size_type(this->_M_finish-this->_M_start);}bool empty()const{return this->_M_start==this->_M_finish;}T&operator[](size_type n){return *(begin()+n);}const T&operator[](size_type n)const{return *(begin()+n);}void push_back(const T&x){if(this->_M_finish!=this->_M_end_of_storage._M_data){_Construct(this->_M_finish,x);++this->_M_finish;}else _M_insert_overflow(this->_M_finish,x,__true_type(),1,true);}protected:__declspec(noinline) void _M_insert_overflow(iterator __position,const T&__x,const __true_type&,size_type __fill_len,bool __atend){const size_type __old_size=size();const size_type __len=__old_size+(max)(__old_size,__fill_len);iterator __new_start=this->_M_end_of_storage.allocate(__len);iterator __new_finish=(iterator)__copy_trivial(this->_M_start,__position,__new_start);__new_finish=fill_n(__new_finish,__fill_len,__x);if(!__atend)__new_finish=(iterator)__copy_trivial(__position,this->_M_finish,__new_finish);for(iterator p=this->_M_start;p!=this->_M_finish;++p)p->~T();this->_M_end_of_storage.deallocate(this->_M_start,this->_M_end_of_storage._M_data-this->_M_start);this->_M_start=__new_start;this->_M_finish=__new_finish;this->_M_end_of_storage._M_data=__new_start+__len;}};
template<class A,class B>struct pair{typedef A first_type;typedef B second_type;A first;B second;pair():first(),second(){}pair(const A&a,const B&b):first(a),second(b){}};
template<class A,class B,class R>struct binary_function{};template<class A,class R>struct unary_function{};
template<class T>struct less:public binary_function<T,T,bool>{bool operator()(const T&a,const T&b)const{return a<b;}};
template<class P>struct _Select1st:public unary_function<P,typename P::first_type>{const typename P::first_type&operator()(const P&p)const{return p.first;}};
template<class T>struct _Nonconst_traits{typedef T*pointer;typedef T&reference;};template<class T>struct _Const_traits{typedef const T*pointer;typedef const T&reference;};
struct _Rb_tree_node_base{bool _M_color;_Rb_tree_node_base*_M_parent,*_M_left,*_M_right;};
template<class V>struct _Rb_tree_node:public _Rb_tree_node_base{V _M_value_field;};
template<class Dummy>class _Rb_global{public:static _Rb_tree_node_base*__cdecl _M_increment(_Rb_tree_node_base*node){if(node->_M_right){node=node->_M_right;while(node->_M_left)node=node->_M_left;}else{_Rb_tree_node_base*parent=node->_M_parent;while(node==parent->_M_right){node=parent;parent=parent->_M_parent;}if(node->_M_right!=parent)node=parent;}return node;}};
struct _Rb_tree_base_iterator{_Rb_tree_node_base*_M_node;bool operator!=(const _Rb_tree_base_iterator&x)const{return _M_node!=x._M_node;}};
template<class V,class Traits>struct _Rb_tree_iterator:public _Rb_tree_base_iterator{typedef typename Traits::reference reference;typedef typename Traits::pointer pointer;_Rb_tree_iterator(){} _Rb_tree_iterator(_Rb_tree_node<V>*p){_M_node=p;}reference operator*()const{return ((_Rb_tree_node<V>*)_M_node)->_M_value_field;}pointer operator->()const{return &operator*();}_Rb_tree_iterator&operator++(){_M_node=_Rb_global<bool>::_M_increment(_M_node);return *this;}};
template<class V,class A>struct _Rb_tree_base{_Rb_tree_base(const A&):_M_header(allocator<_Rb_tree_node<V> >(),0){_M_header._M_data=_M_header.allocate(1);}~_Rb_tree_base(){if(_M_header._M_data)_M_header.deallocate(_M_header._M_data,1);}protected:_STLP_alloc_proxy<_Rb_tree_node<V>*,_Rb_tree_node<V>,allocator<_Rb_tree_node<V> > >_M_header;};
template<class K,class V,class E,class C,class A>class _Rb_tree:public _Rb_tree_base<V,A>{protected:size_t _M_node_count;C _M_key_compare;private:void _M_erase(_Rb_tree_node<V>*);public:typedef _Rb_tree_iterator<V,_Nonconst_traits<V> >iterator;_Rb_tree(const C&c,const A&a):_Rb_tree_base<V,A>(a),_M_node_count(0),_M_key_compare(c){this->_M_header._M_data->_M_color=false;this->_M_header._M_data->_M_parent=0;this->_M_header._M_data->_M_left=this->_M_header._M_data;this->_M_header._M_data->_M_right=this->_M_header._M_data;}~_Rb_tree(){clear();}void clear(){if(_M_node_count){_M_erase((_Rb_tree_node<V>*)this->_M_header._M_data->_M_parent);this->_M_header._M_data->_M_left=this->_M_header._M_data;this->_M_header._M_data->_M_parent=0;this->_M_header._M_data->_M_right=this->_M_header._M_data;_M_node_count=0;}}iterator begin(){return iterator((_Rb_tree_node<V>*)this->_M_header._M_data->_M_left);}iterator end(){return iterator(this->_M_header._M_data);}};
template<class K,class V,class C=less<K>,class A=allocator<pair<const K,V> > >class map{public:typedef pair<const K,V>value_type;private:typedef _Rb_tree<K,value_type,_Select1st<value_type>,C,A>_Rep_type;_Rep_type _M_t;public:typedef typename _Rep_type::iterator iterator;map():_M_t(C(),A()){}iterator begin(){return _M_t.begin();}iterator end(){return _M_t.end();}V&operator[](const K&);};
}
template<class T>class CArray:public std::vector<T>{public:T&operator[](unsigned int n){return std::vector<T>::operator[](n);}const T&operator[](unsigned int n)const{return std::vector<T>::operator[](n);}};
#define FABLE_RETAIL_HEADER_CWIDESTRING

class CCharString;
class CWideStringData;
class CWideString {
public:
    CWideString();
    CWideString(const CWideString&);
    long GetLength()const;
    const CWideString& operator=(const CWideString&);
    ~CWideString();
protected:
    CWideStringData* PStringData;
};
#define FABLE_ENGINE_CCHARSTRING_H


class CCharStringData;
class CCharString {
public:
    long FindCharPos(char)const;
    CCharString();
    CCharString(const char* text, long length = -1);
    CCharString(const CCharString& other);
    ~CCharString();
protected:
    CCharStringData* PStringData;
};

CCharString __fastcall operator+(const CCharString&,const CCharString&);
CCharString __fastcall operator+(const CCharString&,const char*);
CCharString __fastcall GFIntToCharString(long);
#define FABLE_ENGINE_CPACKEDUINTARRAY_H
class CPackedUIntArray{public:unsigned long operator[](unsigned long)const;unsigned long GetSize()const{return Size;}private:unsigned long*PackedInts;unsigned long Size;unsigned char Bits;unsigned long Bias;};
#define FABLE_ENGINE_CBANKFILEENTRYUPDATEDATA_H
class CBankFileEntryUpdateData{friend class CBankFile;private:unsigned long StateBlockCRC,InfoSize;char*InfoBuffer;unsigned char Filenames[8];bool RequiresUpdate,Exists;};
class CCharString;class CWideString;class CDateAndTime;class CBankStateBlock;class CBankFileEntryStats;
class CBankFileDataTypeCount;class CDataOutputStream;
template<class T>class CArray;template<class T>class CCountedPointer;


class CBankFile:public CBaseClassNonCopyable {
public:
 class CProgressCallback {public:
  virtual void NotifyBankUpdateProgress(unsigned long,unsigned long)=0;
  virtual void NotifyBankUpdateStatus(const CCharString&)=0;
  virtual void NotifyBankLogMessage(const CCharString&)=0;
  virtual void NotifyBankWarning(const CCharString&)=0;
  virtual void NotifyBankStats(const class CBankFileEntryStats&)=0;
 };
 void WriteSymbolHeaderFile();
protected:
 void SetBankUpdateStatus(const CCharString& text){if(ProgressCallback)ProgressCallback->NotifyBankUpdateStatus(text);}
public:
 unsigned long GetEntryDataType(unsigned long i)const{return PackedDataType.GetSize()?PackedDataType[i-1]:RuntimeData[i].DataType;}
 CCharString GetEntrySymbol(unsigned long i)const{if(Symbols.empty())return CCharString("(Unknown)");return Symbols[i];}
private:
 class CRuntimeData {public:unsigned long DataOffset,DataSize;unsigned char DataType;bool Valid;};
public: 
public: CBankFile();virtual ~CBankFile();
public:
 virtual bool OpenReadOnly(const CCharString &,unsigned long);
public:
 virtual void OpenReadWrite(const CCharString &,unsigned long,unsigned long);
public:
 virtual bool OpenFileReadOnly(const CWideString &,unsigned long);
public:
 virtual void OpenFileReadWrite(const CWideString &,unsigned long,unsigned long);
public:
 virtual void Close();
public:
 virtual void AddEntryMultipleFiles(const CCharString &,unsigned long,const CArray<CCharString> &,const CCountedPointer<CBankStateBlock>);
public:
 virtual void Update();
protected:
 virtual unsigned long GetDataFormatVersion();
protected:
 virtual unsigned long AdjustUpdateEntries();
protected:
 virtual void NotifyBankSize(unsigned long,const CArray<CBankFileDataTypeCount> &);
protected:
 virtual void NotifyBankOpened();
protected:
 virtual void NotifyEntryAdded(unsigned long,unsigned long,const CArray<char> &);
protected:
 virtual void NotifyEntryUpdated(unsigned long,unsigned long,const CArray<char> &);
protected:
 virtual void NotifyEntryRemoved(unsigned long);
protected:
 virtual void NotifyPostEntriesLoaded();
protected:
 virtual void PrepareDataForInclusion(unsigned long,const CArray<CCharString> &,unsigned long,const CCountedPointer<CBankStateBlock>,CDataOutputStream &,CDataOutputStream &);
protected:
 virtual bool IsDiskFile(unsigned long,unsigned long);
protected:
 virtual CDateAndTime GetLastModifiedTime(const CCharString &);
protected:
 virtual CCharString GetEnumerationType(unsigned long) const;


private:
 unsigned long Size;
 CArray<CCharString> Symbols;CArray<unsigned long> Checksums;CArray<CRuntimeData> RuntimeData;CArray<CBankFileEntryUpdateData*> UpdateData;
 CPackedUIntArray PackedDataOffset,PackedDataSize,PackedDataType,PackedValid;
 unsigned long OpenFlags;
 unsigned char BankFile[8];CWideString Filename;CCharString BankHandle;
 bool RetailMode,ReadOnly,WrittenToFile,Solid;
 unsigned long ChangeCount;
 bool FileValid,UpdatingFlag;
 unsigned long Alignment,BankFormatVersion,HeaderSize,FileInformationSize,FileSize;
 unsigned char SymbolIndices[12],OldSymbolIndices[12],FilenameIndices[12],SymbolCRCIndices[16],BlockOrder[12];
 CProgressCallback* ProgressCallback;
 unsigned char HeaderEnumerationType[4];
 bool WarningFlag;
 unsigned char _pad_0xf5[3];
 unsigned char ChecksumCache[24];
};

#define FABLE_ENGINE_CBANKFILEMANAGER_H


class CCharString;
class CBankFileManager:public CBaseClass{public:virtual ~CBankFileManager();CWideString GetDevHeaderFilePath(const CCharString&)const;CWideString GetRetailHeaderFilePath(const CCharString&)const;private:unsigned char Banks[12],RetailBanks[4];bool UsingRetailBanks;unsigned char _pad_0x15[3];unsigned char BankAliasMap[12];CWideString BasePath;unsigned char CreatedRetailBanks[12];unsigned long FileVersion;};
namespace NBankFileManager{extern CBankFileManager GBankFileManager;}
#define FABLE_ENGINE_CMEMORYFILE_H
#define FABLE_RETAIL_HEADER_CAFILE


class CCharString;
class CASuspendableProcess;
class CFileInformation;
enum FileAccessType { FILE_ACCESS_NULL=0, FILE_ACCESS_READ=1, FILE_ACCESS_READ_WRITE=2, FILE_ACCESS_READ_WRITE_CREATE=3, FILE_ACCESS_CREATE=4 };

class CAFile : public CBaseClassNonCopyable {
public:
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

class CMemoryFile:public CAFile{public:CMemoryFile():Opened(false){}virtual ~CMemoryFile(){Close();}virtual void Open(const CWideString&,FileAccessType,unsigned long);virtual void Close();virtual void Read(void*,long,CASuspendableProcess*);virtual void Write(const void*,long,CASuspendableProcess*);virtual void SetPosition(long);virtual void MovePosition(long);virtual long GetPosition()const;virtual void SetLength(long);virtual long GetLength()const;virtual bool IsOpen()const;virtual CWideString GetPathname()const;virtual CAFile*Clone()const;void DumpToFile();private:FileAccessType AccessType;long Position,Length;CArray<unsigned char>DataBuffer;bool Opened;CWideString Pathname;};
#define FABLE_ENGINE_CDATAOUTPUTSTREAM_H

#define FABLE_RETAIL_HEADER_CDATASTREAM

class CDataStream : public CBaseClassNonCopyable {
public:
    virtual ~CDataStream();
    virtual void SetPosition(unsigned long)=0;
    virtual unsigned long GetPosition() const=0;
    virtual void Close()=0;
};

CCharString __fastcall operator+(const CCharString&,const CCharString&);
CCharString __fastcall operator+(const CCharString&,const char*);
CCharString __fastcall GFIntToCharString(long);
class CWideString;
class CDataOutputStream : public CDataStream {
public:
    virtual ~CDataOutputStream();
    virtual void Write(const void*,long)=0;
    void WriteStringNewLine(const CCharString&);
    void WriteSLONG(long);
    void WriteULONG(unsigned long);
private:
    CCharString NewLine;
};
#define FABLE_ENGINE_CFILEDATAOUTPUTSTREAM_H










class CAFile;
class CFileDataOutputStream : public CDataOutputStream {
public:
    CFileDataOutputStream(CAFile& file);                                 
    virtual ~CFileDataOutputStream();                                    
    virtual void SetPosition(unsigned long position);                    
    virtual unsigned long GetPosition() const;                           
    virtual void Close();                                                
    virtual void Write(const void* data, long length);                   
    CAFile* File;                                                        
};

void CBankFile::WriteSymbolHeaderFile()
{
 CWideString header_filename;
 if(RetailMode)header_filename=NBankFileManager::GBankFileManager.GetRetailHeaderFilePath(BankHandle);
 else header_filename=NBankFileManager::GBankFileManager.GetDevHeaderFilePath(BankHandle);
 if(header_filename.GetLength()!=0){
  SetBankUpdateStatus("Writing symbol file...");
  CMemoryFile header_file;
  header_file.Open(header_filename,FILE_ACCESS_CREATE,2);
  CFileDataOutputStream outstream(header_file);
  CCharString included_define=BankHandle+"_HEADER_INCLUDED";
  outstream.WriteStringNewLine(CCharString("/* This file was auto-generated by BankCreator */"));
  outstream.WriteStringNewLine("");
  std::map<CCharString,CArray<unsigned long> > enum_lists;
  for(long c_entry=0;c_entry<(long)Size;++c_entry){
   if(RuntimeData[c_entry].Valid&&RuntimeData[c_entry].DataOffset!=0&&!UpdateData[c_entry]->RequiresUpdate){
    CCharString enum_type=GetEnumerationType(GetEntryDataType(c_entry));
    enum_lists[enum_type].push_back(c_entry);
   }
  }
  {
   for(std::map<CCharString,CArray<unsigned long> >::iterator itr=enum_lists.begin();itr!=enum_lists.end();++itr){
    outstream.WriteStringNewLine(CCharString("enum ")+itr->first);
    outstream.WriteStringNewLine("{");
    const CArray<unsigned long>& enum_list=itr->second;
    for(unsigned long i=0;i<enum_list.size();++i){
     unsigned long entry_index=enum_list[i];
     CCharString symbol=GetEntrySymbol(entry_index);
     if(symbol.FindCharPos('[')==-1)outstream.WriteStringNewLine(CCharString("\t")+symbol+" = "+GFIntToCharString(entry_index)+",");
    }
    outstream.WriteStringNewLine("};");
    outstream.WriteStringNewLine("");
   }
  }
  header_file.DumpToFile();
  outstream.WriteStringNewLine("");
  outstream.WriteStringNewLine("/*");
  outstream.WriteStringNewLine("So we can see all the graphic indices (for debugging) the remaining symbols are output here.");
  outstream.WriteStringNewLine("Warning: Do not uncomment!");
  {
   for(std::map<CCharString,CArray<unsigned long> >::iterator itr=enum_lists.begin();itr!=enum_lists.end();++itr){
    outstream.WriteStringNewLine(CCharString("enum ")+itr->first+"_1");
    outstream.WriteStringNewLine("{");
    const CArray<unsigned long>& enum_list=itr->second;
    for(unsigned long i=0;i<enum_list.size();++i){
     unsigned long entry_index=enum_list[i];
     CCharString symbol=GetEntrySymbol(entry_index);
     if(symbol.FindCharPos('[')!=-1)outstream.WriteStringNewLine(CCharString("\t")+symbol+" = "+GFIntToCharString(entry_index)+",");
    }
    outstream.WriteStringNewLine("};");
    outstream.WriteStringNewLine("");
   }
  }
  outstream.WriteStringNewLine("*/");
  header_file.DumpToFile();
  header_file.Close();
 }
}
static unsigned string_blocks,wide_blocks,node_blocks,case_count;
static unsigned dev_calls,retail_calls,open_calls,dump_calls,close_calls,status_calls,enum_calls;
static bool empty_path;
static char output_text[16384],first_dump[16384],last_dump[16384];
static const char* chars(const CCharString&s){return s.PStringData?(const char*)s.PStringData:"";}
static char* duplicate_text(const char*s,long n){if(n<0)n=(long)strlen(s);char*p=(char*)malloc((size_t)n+1);assert(p);memcpy(p,s,n);p[n]=0;return p;}
CCharString::CCharString():PStringData(0){}
CCharString::CCharString(const char*s,long n):PStringData((CCharStringData*)duplicate_text(s,n)){++string_blocks;}
CCharString::CCharString(const CCharString&s):PStringData((CCharStringData*)duplicate_text(chars(s),-1)){++string_blocks;}
CCharString::~CCharString(){if(PStringData){free(PStringData);--string_blocks;}}
long CCharString::FindCharPos(char c)const{const char*p=strchr(chars(*this),c);return p?(long)(p-chars(*this)):-1;}
CCharString __fastcall operator+(const CCharString&a,const CCharString&b){char text[4096];assert(strlen(chars(a))+strlen(chars(b))<sizeof(text));strcpy(text,chars(a));strcat(text,chars(b));return CCharString(text);}
CCharString __fastcall operator+(const CCharString&a,const char*b){return a+CCharString(b);}
CCharString __fastcall GFIntToCharString(long value){char text[32];sprintf(text,"%d",(int)value);return CCharString(text);}
CWideString::CWideString():PStringData(0){}
CWideString::CWideString(const CWideString&s):PStringData((CWideStringData*)duplicate_text(s.PStringData?(const char*)s.PStringData:"",-1)){++wide_blocks;}
CWideString::~CWideString(){if(PStringData){free(PStringData);--wide_blocks;}}
const CWideString&CWideString::operator=(const CWideString&s){if(this!=&s){if(PStringData){free(PStringData);--wide_blocks;}PStringData=(CWideStringData*)duplicate_text(s.PStringData?(const char*)s.PStringData:"",-1);++wide_blocks;}return *this;}
long CWideString::GetLength()const{return PStringData?(long)strlen((const char*)PStringData):0;}
CBaseClass::CBaseClass(){}CBaseClass::~CBaseClass(){}
CBaseClassNonCopyable::CBaseClassNonCopyable(){}CBaseClassNonCopyable::~CBaseClassNonCopyable(){}
CBankFileManager::~CBankFileManager(){}
namespace NBankFileManager {CBankFileManager GBankFileManager;}
static CWideString path_result(bool retail){CWideString s;s.PStringData=(CWideStringData*)duplicate_text(empty_path?"":retail?"retail.h":"development.h",-1);++wide_blocks;return s;}
CWideString CBankFileManager::GetDevHeaderFilePath(const CCharString&key)const{assert(strcmp(chars(key),"TEST_BANK")==0);++dev_calls;return path_result(false);}
CWideString CBankFileManager::GetRetailHeaderFilePath(const CCharString&key)const{assert(strcmp(chars(key),"TEST_BANK")==0);++retail_calls;return path_result(true);}
void CMemoryFile::Open(const CWideString&p,FileAccessType a,unsigned long flags){assert(a==FILE_ACCESS_CREATE&&flags==2);assert(p.GetLength()>0);++open_calls;Pathname=p;Opened=true;Position=Length=0;AccessType=a;}
void CMemoryFile::Close(){++close_calls;Opened=false;}
void CMemoryFile::DumpToFile(){assert(Opened);if(!dump_calls)strcpy(first_dump,output_text);strcpy(last_dump,output_text);++dump_calls;}
void CMemoryFile::Read(void*,long,CASuspendableProcess*){assert(false);}
void CMemoryFile::Write(const void*,long,CASuspendableProcess*){assert(false);}
void CMemoryFile::SetPosition(long n){Position=n;}
void CMemoryFile::MovePosition(long n){Position+=n;}
long CMemoryFile::GetPosition()const{return Position;}
void CMemoryFile::SetLength(long n){Length=n;}
long CMemoryFile::GetLength()const{return Length;}
bool CMemoryFile::IsOpen()const{return Opened;}
CWideString CMemoryFile::GetPathname()const{return Pathname;}
CAFile*CMemoryFile::Clone()const{assert(false);return 0;}
bool CAFile::IsIOFinished()const{return true;}long CAFile::GetAlignment()const{return 1;}
CFileInformation CAFile::GetFileInformation(){throw 1;}
CDataStream::~CDataStream(){}CDataOutputStream::~CDataOutputStream(){}
CFileDataOutputStream::CFileDataOutputStream(CAFile&f):File(&f){}
CFileDataOutputStream::~CFileDataOutputStream(){}
void CFileDataOutputStream::SetPosition(unsigned long n){File->SetPosition(n);}
unsigned long CFileDataOutputStream::GetPosition()const{return File->GetPosition();}
void CFileDataOutputStream::Close(){}
void CFileDataOutputStream::Write(const void*,long){assert(false);}
void CDataOutputStream::WriteStringNewLine(const CCharString&s){assert(strlen(output_text)+strlen(chars(s))+2<sizeof(output_text));strcat(output_text,chars(s));strcat(output_text,"\n");}
unsigned long CPackedUIntArray::operator[](unsigned long n)const{assert(n<Size&&Bits==32&&Bias==0);return PackedInts[n];}
CBankFile::CBankFile():Size(0),RetailMode(false),ProgressCallback(0){PackedDataOffset.PackedInts=PackedDataSize.PackedInts=PackedDataType.PackedInts=PackedValid.PackedInts=0;PackedDataOffset.Size=PackedDataSize.Size=PackedDataType.Size=PackedValid.Size=0;}
CBankFile::~CBankFile(){}
bool CBankFile::OpenReadOnly(const CCharString&,unsigned long){throw 1;}
void CBankFile::OpenReadWrite(const CCharString&,unsigned long,unsigned long){throw 1;}
bool CBankFile::OpenFileReadOnly(const CWideString&,unsigned long){throw 1;}
void CBankFile::OpenFileReadWrite(const CWideString&,unsigned long,unsigned long){throw 1;}
void CBankFile::Close(){throw 1;}
void CBankFile::AddEntryMultipleFiles(const CCharString&,unsigned long,const CArray<CCharString>&,const CCountedPointer<CBankStateBlock>){throw 1;}
void CBankFile::Update(){throw 1;}
unsigned long CBankFile::GetDataFormatVersion(){throw 1;}
unsigned long CBankFile::AdjustUpdateEntries(){throw 1;}
void CBankFile::NotifyBankSize(unsigned long,const CArray<CBankFileDataTypeCount>&){throw 1;}
void CBankFile::NotifyBankOpened(){throw 1;}
void CBankFile::NotifyEntryAdded(unsigned long,unsigned long,const CArray<char>&){throw 1;}
void CBankFile::NotifyEntryUpdated(unsigned long,unsigned long,const CArray<char>&){throw 1;}
void CBankFile::NotifyEntryRemoved(unsigned long){throw 1;}
void CBankFile::NotifyPostEntriesLoaded(){throw 1;}
void CBankFile::PrepareDataForInclusion(unsigned long,const CArray<CCharString>&,unsigned long,const CCountedPointer<CBankStateBlock>,CDataOutputStream&,CDataOutputStream&){throw 1;}
bool CBankFile::IsDiskFile(unsigned long,unsigned long){throw 1;}
CDateAndTime CBankFile::GetLastModifiedTime(const CCharString&){throw 1;}
CCharString CBankFile::GetEnumerationType(unsigned long t)const{assert(t<2);++enum_calls;return CCharString(t?"BETA":"ALPHA");}
namespace std {
template<class K,class V,class E,class C,class A>void _Rb_tree<K,V,E,C,A>::_M_erase(_Rb_tree_node<V>*p){if(!p)return;_M_erase((_Rb_tree_node<V>*)p->_M_left);_M_erase((_Rb_tree_node<V>*)p->_M_right);p->_M_value_field.~V();free(p);--node_blocks;}
template<class K,class V,class C,class A>V&map<K,V,C,A>::operator[](const K&key){typedef _Rb_tree_node<value_type>Node;Node*header=_M_t._M_header._M_data;Node*parent=header;Node*node=(Node*)header->_M_parent;int comparison=0;while(node){comparison=strcmp(chars(key),chars(node->_M_value_field.first));if(!comparison)return node->_M_value_field.second;parent=node;node=(Node*)(comparison<0?node->_M_left:node->_M_right);}node=(Node*)malloc(sizeof(Node));assert(node);++node_blocks;new(&node->_M_value_field)value_type(key,V());node->_M_color=false;node->_M_parent=parent;node->_M_left=node->_M_right=0;if(parent==header)header->_M_parent=node;else if(comparison<0)parent->_M_left=node;else parent->_M_right=node;if(header->_M_left==header||strcmp(chars(key),chars(((Node*)header->_M_left)->_M_value_field.first))<0)header->_M_left=node;if(header->_M_right==header||strcmp(chars(key),chars(((Node*)header->_M_right)->_M_value_field.first))>0)header->_M_right=node;++_M_t._M_node_count;return node->_M_value_field.second;}
}
class BankProgressRecorder:public CBankFile::CProgressCallback {public:void NotifyBankUpdateProgress(unsigned long,unsigned long){assert(false);}void NotifyBankUpdateStatus(const CCharString&s){assert(strcmp(chars(s),"Writing symbol file...")==0);++status_calls;}void NotifyBankLogMessage(const CCharString&){assert(false);}void NotifyBankWarning(const CCharString&){assert(false);}void NotifyBankStats(const CBankFileEntryStats&){assert(false);}};
template<class T>static void allocate_array(CArray<T>&a,unsigned n){assert(a.empty());T*p=(T*)malloc(n*sizeof(T));assert(p||!n);a._M_start=a._M_finish=p;a._M_end_of_storage._M_data=p+n;for(unsigned i=0;i<n;++i){new(p+i)T();++a._M_finish;}}
static void append_line(char*out,const char*s){strcat(out,s);strcat(out,"\n");}
static void run(bool retail,bool blank,bool callback,bool packed,bool unknown,unsigned filter,unsigned n){
 unsigned old_strings=string_blocks,old_wide=wide_blocks;
 dev_calls=retail_calls=open_calls=dump_calls=close_calls=status_calls=enum_calls=0;output_text[0]=first_dump[0]=last_dump[0]=0;empty_path=blank;
 {
 CBankFile b;BankProgressRecorder progress;b.BankHandle.~CCharString();new(&b.BankHandle)CCharString("TEST_BANK");b.RetailMode=retail;b.ProgressCallback=callback?&progress:0;b.Size=n;
 CBankFileEntryUpdateData update[8];memset(update,0,sizeof(update));unsigned long types[8];
 allocate_array(b.RuntimeData,n);allocate_array(b.UpdateData,n);if(!unknown)allocate_array(b.Symbols,n);
 const char*symbols[8]={"SKIP_ZERO","B_PUBLIC","A[0]","A_PUBLIC","B[1]","LAST","C[3]","FINAL"};
 bool allowed[8];unsigned groups[8];
 for(unsigned i=0;i<n;++i){bool valid=i>0;bool offset=true;bool refresh=false;if(filter==1&&i==2)valid=false;if(filter==2&&i==3)offset=false;if(filter==3&&i==4)refresh=true;
 b.RuntimeData[i].Valid=valid;b.RuntimeData[i].DataOffset=offset?16:0;b.RuntimeData[i].DataType=(unsigned char)(i%2);b.UpdateData[i]=&update[i];update[i].RequiresUpdate=refresh;allowed[i]=valid&&offset&&!refresh;groups[i]=packed?(i+1)%2:i%2;if(i)types[i-1]=groups[i];
 if(!unknown){b.Symbols[i].~CCharString();new(&b.Symbols[i])CCharString(symbols[i]);}}
 if(packed){b.PackedDataType.PackedInts=types;b.PackedDataType.Size=n?n-1:0;b.PackedDataType.Bits=32;b.PackedDataType.Bias=0;}
 char expected[16384]="",expected_first[16384]="";unsigned qualifying=0;
 if(!blank){append_line(expected,"/* This file was auto-generated by BankCreator */");append_line(expected,"");for(unsigned i=0;i<n;++i)if(allowed[i])++qualifying;
 for(unsigned phase=0;phase<2;++phase){if(phase){strcpy(expected_first,expected);append_line(expected,"");append_line(expected,"/*");append_line(expected,"So we can see all the graphic indices (for debugging) the remaining symbols are output here.");append_line(expected,"Warning: Do not uncomment!");}
 for(unsigned group=0;group<2;++group){bool exists=false;for(unsigned i=0;i<n;++i)if(allowed[i]&&groups[i]==group)exists=true;if(!exists)continue;char line[256];sprintf(line,"enum %s%s",group?"BETA":"ALPHA",phase?"_1":"");append_line(expected,line);append_line(expected,"{");for(unsigned i=0;i<n;++i){const char*name=unknown?"(Unknown)":symbols[i];bool private_symbol=strchr(name,'[')!=0;if(allowed[i]&&groups[i]==group&&private_symbol==(phase!=0)){sprintf(line,"\t%s = %u,",name,i);append_line(expected,line);}}append_line(expected,"};");append_line(expected,"");}}
 append_line(expected,"*/");}
 b.WriteSymbolHeaderFile();assert(dev_calls==(retail?0u:1u)&&retail_calls==(retail?1u:0u));assert(open_calls==(blank?0u:1u));assert(dump_calls==(blank?0u:2u)&&close_calls==(blank?0u:2u));assert(status_calls==(!blank&&callback?1u:0u));assert(enum_calls==qualifying);assert(strcmp(output_text,expected)==0);assert(strcmp(first_dump,expected_first)==0);assert(strcmp(last_dump,expected)==0);assert(node_blocks==0);
 }
 assert(string_blocks==old_strings&&wide_blocks==old_wide);++case_count;
}
int main(){for(unsigned flags=0;flags<32;++flags)for(unsigned filter=0;filter<4;++filter)for(unsigned n=0;n<=8;++n)run((flags&1)!=0,(flags&2)!=0,(flags&4)!=0,(flags&8)!=0,(flags&16)!=0,filter,n);assert(case_count==1152);puts("009cf3a0_TEST PASS");return 0;}