#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <new>
#ifndef _MSC_VER
#define __cdecl
#define __fastcall
#define __forceinline inline
#endif
#define private public
#define protected public
// 00a60fb0 CChecksumCacheFile::GetChecksum
// Leaf: ?GetChecksum@CChecksumCacheFile@@QAEKABVCWideString@@@Z
// Flags:
#include <new>
void*__cdecl operator new[](::size_t);
void __cdecl operator delete[](void*);
namespace std{
typedef ::size_t size_t;typedef ::ptrdiff_t ptrdiff_t;
struct input_iterator_tag{};struct forward_iterator_tag:public input_iterator_tag{};struct bidirectional_iterator_tag:public forward_iterator_tag{};
template<class A,class B>struct pair{typedef A first_type;typedef B second_type;A first;B second;pair():first(A()),second(B()){}pair(const A&a,const B&b):first(a),second(b){}template<class X,class Y>pair(const pair<X,Y>&p):first(p.first),second(p.second){}pair(const pair&p):first(p.first),second(p.second){}};
template<class A,class B,class C>struct binary_function{typedef A first_argument_type;typedef B second_argument_type;typedef C result_type;};
template<class A,class B>struct unary_function{typedef A argument_type;typedef B result_type;};
template<class T>struct less:public binary_function<T,T,bool>{bool operator()(const T&a,const T&b)const{return a<b;}};
template<class P>struct _Select1st:public unary_function<P,typename P::first_type>{const typename P::first_type&operator()(const P&p)const{return p.first;}};
template<class T>class allocator{public:typedef T value_type;template<class U>struct rebind{typedef allocator<U>other;};allocator(){}allocator(const allocator&){}template<class U>allocator(const allocator<U>&){}};
template<class T,class A>struct _Alloc_traits{typedef typename A::template rebind<T>::other allocator_type;};
template<class V,class T,class A>class _STLP_alloc_proxy:public A{public:V _M_data;_STLP_alloc_proxy(const A&a,V p):A(a),_M_data(p){}};
typedef bool _Rb_tree_Color_type;
struct _Rb_tree_node_base{typedef _Rb_tree_node_base*_Base_ptr;_Rb_tree_Color_type _M_color;_Base_ptr _M_parent,_M_left,_M_right;};
template<class V>struct _Rb_tree_node:public _Rb_tree_node_base{V _M_value_field;};
template<class Dummy>class _Rb_global{public:typedef _Rb_tree_node_base*_Base_ptr;static _Base_ptr __cdecl _M_increment(_Base_ptr);static _Base_ptr __cdecl _M_decrement(_Base_ptr);};
typedef _Rb_global<bool>_Rb_global_inst;
struct _Rb_tree_base_iterator{typedef _Rb_tree_node_base*_Base_ptr;typedef bidirectional_iterator_tag iterator_category;typedef ptrdiff_t difference_type;_Base_ptr _M_node;bool operator==(const _Rb_tree_base_iterator&x)const{return _M_node==x._M_node;}bool operator!=(const _Rb_tree_base_iterator&x)const{return _M_node!=x._M_node;}};
template<class T>struct _Nonconst_traits{typedef T value_type;typedef T&reference;typedef T*pointer;};
template<class V,class Traits>struct _Rb_tree_iterator:public _Rb_tree_base_iterator{
 typedef V value_type;typedef typename Traits::reference reference;typedef typename Traits::pointer pointer;typedef _Rb_tree_iterator<V,Traits>_Self;typedef _Rb_tree_node<V>*_Link_type;
 _Rb_tree_iterator(){_M_node=0;}_Rb_tree_iterator(_Link_type p){_M_node=p;}_Rb_tree_iterator(const _Rb_tree_iterator<V,_Nonconst_traits<V> >&p){_M_node=p._M_node;}
 reference operator*()const{return _Link_type(_M_node)->_M_value_field;}
 _Self&operator++(){_M_node=_Rb_global_inst::_M_increment(_M_node);return*this;}_Self&operator--(){_M_node=_Rb_global_inst::_M_decrement(_M_node);return*this;}
};
template<class V,class A>struct _Rb_tree_base{
 typedef _Rb_tree_node<V>_Node;typedef typename _Alloc_traits<V,A>::allocator_type allocator_type;
 _Rb_tree_base(const allocator_type& = allocator_type());~_Rb_tree_base();
protected:typedef typename _Alloc_traits<_Node,A>::allocator_type _M_node_allocator_type;_STLP_alloc_proxy<_Node*,_Node,_M_node_allocator_type>_M_header;
};
template<class K,class V,class Key,class Compare,class A=allocator<V> >class _Rb_tree:public _Rb_tree_base<V,A>{
 typedef _Rb_tree_base<V,A>_Base;
protected:typedef _Rb_tree_node_base*_Base_ptr;typedef _Rb_tree_node<V>_Node;typedef _Rb_tree_Color_type _Color_type;
public:typedef K key_type;typedef V value_type;typedef V*pointer;typedef const V*const_pointer;typedef V&reference;typedef const V&const_reference;typedef _Rb_tree_node<V>*_Link_type;typedef size_t size_type;typedef ptrdiff_t difference_type;typedef bidirectional_iterator_tag _Iterator_category;typedef typename _Base::allocator_type allocator_type;
protected:size_type _M_node_count;Compare _M_key_compare;
 _Link_type&_M_rightmost()const{return(_Link_type&)this->_M_header._M_data->_M_right;}
 static _Link_type&__cdecl _S_right(_Link_type p){return(_Link_type&)p->_M_right;}
 static reference __cdecl _S_value(_Link_type p){return p->_M_value_field;}
 static const K&__cdecl _S_key(_Link_type p){return Key()(_S_value(p));}
 static _Link_type&__cdecl _S_right(_Base_ptr p){return(_Link_type&)p->_M_right;}
 static reference __cdecl _S_value(_Base_ptr p){return((_Link_type)p)->_M_value_field;}
 static const K&__cdecl _S_key(_Base_ptr p){return Key()(_S_value(_Link_type(p)));}
public:typedef _Rb_tree_iterator<V,_Nonconst_traits<V> >iterator;
private:template<class KT>_Link_type _M_find(const KT&)const;
public:iterator find(const K&x){return iterator(_M_find(x));}iterator end(){return iterator(this->_M_header._M_data);}
private:iterator _M_insert(_Base_ptr,_Base_ptr,const value_type&,_Base_ptr=0);
public:size_type size()const{return _M_node_count;}pair<iterator,bool>insert_unique(const value_type&);iterator insert_unique(iterator,const value_type&);
};

template<class T>struct _Identity:public unary_function<T,T>{const T&operator()(const T&v)const{return v;}};
template<class K,class V,class C=less<K>,class A=allocator<pair<const K,V> > >class map{
public:typedef pair<const K,V>value_type;
private:typedef _Rb_tree<K,value_type,_Select1st<value_type>,C,A>_Rep_type;_Rep_type _M_t;
public:typedef typename _Rep_type::iterator iterator;iterator find(const K&x){return _M_t.find(x);}iterator end(){return _M_t.end();}V&operator[](const K&);
};
template<class K,class C=less<K>,class A=allocator<K> >class set{
private:typedef _Rb_tree<K,K,_Identity<K>,C,A>_Rep_type;_Rep_type _M_t;
};
}
class CWideStringData;
class CWideString{protected:CWideStringData*PStringData;public:CWideString();~CWideString();};
class CDateAndTime{protected:long Year;unsigned long Month,Day,Hour,Minute,Second,Millisecond;public:CDateAndTime();bool operator==(const CDateAndTime&)const;};
class CBaseClass{public:virtual~CBaseClass();};
class CBaseClassNonCopyable:public CBaseClass{protected:CBaseClassNonCopyable();virtual~CBaseClassNonCopyable();};
class CASuspendableProcess;class CFileInformation;
enum FileAccessType{};
class CAFile:public CBaseClassNonCopyable{public:
 static bool __fastcall PathExists(const CWideString&);
 static CDateAndTime __fastcall GetFileModifiedTime(const CWideString&);
 virtual void Open(const CWideString&,FileAccessType,unsigned long)=0;
 virtual void Close()=0;
 virtual void Read(void*,long,CASuspendableProcess*)=0;
 virtual void Write(const void*,long,CASuspendableProcess*)=0;
 virtual void SetPosition(long)=0;virtual void MovePosition(long)=0;
 virtual long GetPosition()const=0;virtual void SetLength(long)=0;virtual long GetLength()const=0;
 virtual bool IsOpen()const=0;virtual CWideString GetPathname()const=0;
 virtual bool IsIOFinished()const;virtual long GetAlignment()const;
 virtual CAFile*Clone()const=0;virtual CFileInformation GetFileInformation();
};
#define CDiskFileWin32 CDiskFileWin32Fields
// Begin shared header engine/CDiskFileWin32.h
#ifndef FABLE_ENGINE_CDISKFILEWIN32_H
#define FABLE_ENGINE_CDISKFILEWIN32_H
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CDiskFileWin32 = 0x1c.
// Retail evidence: 1 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>
// Begin shared header rebuild_abi.h
#ifndef TEST_EMBED_3AC84EBD4026
#define TEST_EMBED_3AC84EBD4026


// ABI primitives for reconstructed FableTLC modules. Keep this header valid
// under VC7.1; modern-only language/library features belong outside Tier 0.
#if defined(_MSC_VER)
#  define FABLE_CDECL __cdecl
#  define FABLE_STDCALL __stdcall
#  define FABLE_FASTCALL __fastcall
#  define FABLE_THISCALL __fastcall
#else
#  define FABLE_CDECL
#  define FABLE_STDCALL
#  define FABLE_FASTCALL
#  define FABLE_THISCALL
#endif

typedef signed char fable_i8;
typedef unsigned char fable_u8;
typedef signed short fable_i16;
typedef unsigned short fable_u16;
typedef signed int fable_i32;
typedef unsigned int fable_u32;

typedef char fable_require_i8[sizeof(fable_i8) == 1 ? 1 : -1];
typedef char fable_require_i16[sizeof(fable_i16) == 2 ? 1 : -1];
typedef char fable_require_i32[sizeof(fable_i32) == 4 ? 1 : -1];
#ifdef _MSC_VER
typedef char fable_require_ptr32[sizeof(void*) == 4 ? 1 : -1];
#endif

#define FABLE_JOIN_INNER(a, b) a##b
#define FABLE_JOIN(a, b) FABLE_JOIN_INNER(a, b)
#define FABLE_STATIC_ASSERT(expression) typedef char FABLE_JOIN(fable_sassert_, __LINE__)[(expression) ? 1 : -1]

#endif

// End shared header rebuild_abi.h

class CWideStringData;

#pragma pack(push, 1)
class CDiskFileWin32 {
public:
    void*            __vftable;                              // +0x00 vptr, or first dword of a flattened base subobject
    unsigned char    AccessType[0x4];                        // +0x04 FileAccessType
    long             Position;                               // +0x08
    long             Length;                                 // +0x0c
    void*            WinFileHandle;                          // +0x10
    bool             Opened;                                 // +0x14
    unsigned char    _pad_0x15[0x3];                         // +0x15
    CWideStringData* Pathname_PStringData;                   // +0x18 CWideString::PStringData
};
#pragma pack(pop)

#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CDiskFileWin32) == 0x1c);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CDiskFileWin32, __vftable) == 0x0);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CDiskFileWin32, AccessType) == 0x4);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CDiskFileWin32, Position) == 0x8);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CDiskFileWin32, Length) == 0xc);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CDiskFileWin32, WinFileHandle) == 0x10);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CDiskFileWin32, Opened) == 0x14);
#endif
#endif
#ifdef _MSC_VER
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CDiskFileWin32, Pathname_PStringData) == 0x18);
#endif
#endif

#endif // FABLE_ENGINE_CDISKFILEWIN32_H

// End shared header engine/CDiskFileWin32.h
#undef CDiskFileWin32
// Layout: the shared header flattens the real CAFile polymorphic base. Restore its
// original virtual prefix and typed members, keeping Length0c, Opened14, Pathname18.
class CDiskFileWin32:public CAFile{private:FileAccessType AccessType;long Position,Length;void*WinFileHandle;bool Opened;CWideString Pathname;
public:CDiskFileWin32(){Opened=false;}virtual~CDiskFileWin32(){Close();}
 virtual void Open(const CWideString&,FileAccessType,unsigned long);
 virtual void Close();virtual void Read(void*,long,CASuspendableProcess*);
 virtual void Write(const void*,long,CASuspendableProcess*);
 virtual void SetPosition(long);virtual void MovePosition(long);virtual long GetPosition()const;
 virtual void SetLength(long);virtual long GetLength()const{return Length;}
 virtual bool IsOpen()const;virtual CWideString GetPathname()const;
 virtual CAFile*Clone()const;virtual CFileInformation GetFileInformation();
};
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CDiskFileWin32)==sizeof(CDiskFileWin32Fields));
#endif
class CCRC{public:static unsigned long __fastcall Calc(unsigned long,const void*,unsigned long);};
// Layout: original map occupies12 bytes in retail versus16 in the donor. Filename0,
// Entries4, BufferSize10, NeedsSaving14. Tree node pair begins10; TimeStamp14, Checksum30.
class CChecksumCacheFile{public:struct CEntry{CDateAndTime TimeStamp;unsigned long Checksum;};private:CWideString Filename;std::map<CWideString,CEntry>Entries;long BufferSize;bool NeedsSaving;
public:unsigned long GetChecksum(const CWideString&);
};
unsigned long CChecksumCacheFile::GetChecksum(const CWideString& path)
{
    if (!CAFile::PathExists(path))
        return 0;
    std::map<CWideString,CEntry>::iterator found = Entries.find(path);
    CDateAndTime modified;
    if (found != Entries.end()) {
        modified = CAFile::GetFileModifiedTime(path);
        if (modified == (*found).second.TimeStamp)
            return (*found).second.Checksum;
    } else {
        modified = CAFile::GetFileModifiedTime(path);
    }
    char* buffer = new char[BufferSize];
    unsigned long checksum = 0xFFFFFFFF;
    CDiskFileWin32 file;
    file.Open(path, static_cast<FileAccessType>(1), 2);
    long length = file.GetLength();
    for (long position = 0; position < length; position += BufferSize) {
        long size = BufferSize >= length-position ? length-position : BufferSize;
        file.Read(buffer, size, 0);
        checksum = CCRC::Calc(checksum, buffer, size);
    }
    checksum = ~checksum;
    delete[] buffer;
    CEntry entry;
    entry.Checksum = checksum;
    entry.TimeStamp = modified;
    Entries[path] = entry;
    NeedsSaving = true;
    return checksum;
}

#undef private
#undef protected
static unsigned cases,pathCalls,timeCalls,findCalls,lookupCalls,opens,closes,reads,crcs,baseCtors,baseDtors,arrayAllocs,arrayFrees;
static bool existsNow;static long fileLength;static unsigned char contents[256];static const CWideString*expectedPath;
static unsigned long currentStamp,readOffset,rolling;static bool fileOpen;
#define CHECK(x) do{if(!(x)){printf("failure line %d case %u\n",__LINE__,cases);exit(1);}}while(0)
void*__cdecl operator new[](::size_t n){++arrayAllocs;void*p=malloc(n?n:1);CHECK(p);return p;}
void __cdecl operator delete[](void*p){++arrayFrees;free(p);}
class CWideStringData{public:unsigned key;};
CWideString::CWideString():PStringData(0){}CWideString::~CWideString(){}
CDateAndTime::CDateAndTime():Year(0),Month(0),Day(0),Hour(0),Minute(0),Second(0),Millisecond(0){}
bool CDateAndTime::operator==(const CDateAndTime&x)const{return Year==x.Year&&Month==x.Month&&Day==x.Day&&Hour==x.Hour&&Minute==x.Minute&&Second==x.Second&&Millisecond==x.Millisecond;}
static CDateAndTime makeStamp(unsigned long v){CDateAndTime d;d.Year=2006;d.Month=10;d.Day=31;d.Hour=7;d.Minute=11;d.Second=13;d.Millisecond=v;return d;}
CBaseClass::~CBaseClass(){}CBaseClassNonCopyable::CBaseClassNonCopyable(){++baseCtors;}CBaseClassNonCopyable::~CBaseClassNonCopyable(){++baseDtors;}
bool __fastcall CAFile::PathExists(const CWideString&p){CHECK(&p==expectedPath);++pathCalls;return existsNow;}
CDateAndTime __fastcall CAFile::GetFileModifiedTime(const CWideString&p){CHECK(&p==expectedPath&&existsNow);++timeCalls;return makeStamp(currentStamp);}
bool CAFile::IsIOFinished()const{return true;}long CAFile::GetAlignment()const{return 1;}
class CFileInformation{};CFileInformation CAFile::GetFileInformation(){CHECK(false);return CFileInformation();}
void CDiskFileWin32::Open(const CWideString&p,FileAccessType access,unsigned long flags){CHECK(&p==expectedPath&&access==1&&flags==2&&!Opened&&!fileOpen);++opens;Opened=true;fileOpen=true;Position=0;Length=fileLength;AccessType=access;}
void CDiskFileWin32::Close(){CHECK(Opened&&fileOpen&&Position==Length);++closes;Opened=false;fileOpen=false;}
void CDiskFileWin32::Read(void*p,long n,CASuspendableProcess*process){CHECK(Opened&&fileOpen&&!process&&n>0&&Position+n<=Length&&Position==(long)readOffset);memcpy(p,contents+Position,(size_t)n);Position+=n;readOffset+=n;++reads;}
void CDiskFileWin32::Write(const void*,long,CASuspendableProcess*){CHECK(false);}void CDiskFileWin32::SetPosition(long){CHECK(false);}void CDiskFileWin32::MovePosition(long){CHECK(false);}long CDiskFileWin32::GetPosition()const{return Position;}void CDiskFileWin32::SetLength(long){CHECK(false);}bool CDiskFileWin32::IsOpen()const{return Opened;}CWideString CDiskFileWin32::GetPathname()const{CHECK(false);CWideString x;return x;}CAFile*CDiskFileWin32::Clone()const{CHECK(false);return 0;}CFileInformation CDiskFileWin32::GetFileInformation(){CHECK(false);return CFileInformation();}
static unsigned long crcByte(unsigned long v,unsigned char b){v^=b;for(unsigned i=0;i<8;++i)v=(v>>1)^((v&1)?0xedb88320UL:0);return v&0xffffffffUL;}
unsigned long __fastcall CCRC::Calc(unsigned long seed,const void*p,unsigned long n){CHECK(fileOpen&&reads==crcs+1&&n>0&&n<=readOffset&&seed==rolling);const unsigned char*q=(const unsigned char*)p;for(unsigned long i=0;i<n;++i){CHECK(q[i]==contents[readOffset-n+i]);seed=crcByte(seed,q[i]);}rolling=seed;++crcs;return seed;}
namespace std{
template<class V,class A>_Rb_tree_base<V,A>::_Rb_tree_base(const allocator_type&a):_M_header(_M_node_allocator_type(a),new _Node){_Node*h=_M_header._M_data;h->_M_color=false;h->_M_parent=0;h->_M_left=h->_M_right=h;}
template<class V,class A>_Rb_tree_base<V,A>::~_Rb_tree_base(){delete _M_header._M_data;}
template<class K,class V,class Key,class Compare,class A>template<class KT>typename _Rb_tree<K,V,Key,Compare,A>::_Link_type _Rb_tree<K,V,Key,Compare,A>::_M_find(const KT&k)const{
 ++findCalls;CHECK(&k==expectedPath);_Link_type x=(_Link_type)this->_M_header._M_data->_M_parent;while(x){unsigned a=Key()(x->_M_value_field).PStringData->key,b=k.PStringData->key;if(a==b)return x;x=(_Link_type)(b<a?x->_M_left:x->_M_right);}return this->_M_header._M_data;
}
template<class K,class V,class C,class A>V&map<K,V,C,A>::operator[](const K&k){++lookupCalls;CHECK(&k==expectedPath);typedef typename _Rep_type::_Node Node;Node*h=_M_t._M_header._M_data;Node*x=(Node*)h->_M_parent;Node*parent=h;while(x){unsigned a=x->_M_value_field.first.PStringData->key,b=k.PStringData->key;if(a==b)return x->_M_value_field.second;parent=x;x=(Node*)(b<a?x->_M_left:x->_M_right);}Node*n=new Node;const_cast<K&>(n->_M_value_field.first).PStringData=k.PStringData;n->_M_parent=parent;n->_M_left=n->_M_right=0;if(parent==h)h->_M_parent=n;else if(k.PStringData->key<parent->_M_value_field.first.PStringData->key)parent->_M_left=n;else parent->_M_right=n;++_M_t._M_node_count;return n->_M_value_field.second;}
}
static void freeNodes(std::_Rb_tree_node_base*p){if(!p)return;freeNodes(p->_M_left);freeNodes(p->_M_right);delete static_cast<std::_Rb_tree_node<std::pair<const CWideString,CChecksumCacheFile::CEntry> >*>(p);}
static void resetCounts(){pathCalls=timeCalls=findCalls=lookupCalls=opens=closes=reads=crcs=baseCtors=baseDtors=arrayAllocs=arrayFrees=0;readOffset=0;rolling=0xffffffffUL;fileOpen=false;}
static unsigned long expectedCRC(){unsigned long v=0xffffffffUL;for(long i=0;i<fileLength;++i)v=crcByte(v,contents[i]);return (~v)&0xffffffffUL;}
int main(){const long lengths[]={0,1,2,7,8,15,32,65};const long buffers[]={1,2,3,4,7,8,16,128};
 for(unsigned l=0;l<8;++l)for(unsigned b=0;b<8;++b)for(unsigned cached=0;cached<3;++cached)for(unsigned exists=0;exists<2;++exists)for(unsigned pattern=0;pattern<4;++pattern)for(unsigned dirty=0;dirty<2;++dirty){
 ++cases;CChecksumCacheFile cache;CWideString path,left,right;CWideStringData names[3];names[0].key=2;names[1].key=1;names[2].key=3;path.PStringData=&names[0];left.PStringData=&names[1];right.PStringData=&names[2];cache.BufferSize=buffers[b];cache.NeedsSaving=dirty!=0;cache.Entries._M_t._M_node_count=0;
 expectedPath=&left;CChecksumCacheFile::CEntry&lv=cache.Entries[left];lv.TimeStamp=makeStamp(41);lv.Checksum=0x1234;expectedPath=&right;CChecksumCacheFile::CEntry&rv=cache.Entries[right];rv.TimeStamp=makeStamp(42);rv.Checksum=0xabcd;
 expectedPath=&path;currentStamp=100+pattern;CChecksumCacheFile::CEntry*old=0;if(cached){old=&cache.Entries[path];old->TimeStamp=makeStamp(cached==2?currentStamp:currentStamp-1);old->Checksum=0x81234567UL;}
 fileLength=lengths[l];for(long i=0;i<fileLength;++i)contents[i]=(unsigned char)(i*(pattern*23+1)+pattern*19);existsNow=exists!=0;resetCounts();unsigned long got=cache.GetChecksum(path);
 // The target's unsigned-long complement has 32-bit width in retail. GNU64 checks low32 bits.
 unsigned long expect=!exists?0:(cached==2?0x81234567UL:expectedCRC());CHECK((got&0xffffffffUL)==expect);CHECK(pathCalls==1&&findCalls==(exists?1u:0u));bool recalc=exists&&cached!=2;unsigned expectedReads=fileLength?(unsigned)((fileLength+buffers[b]-1)/buffers[b]):0;
 CHECK(timeCalls==(exists?1u:0u)&&lookupCalls==(recalc?1u:0u));CHECK(opens==(recalc?1u:0u)&&closes==opens&&baseCtors==opens&&baseDtors==opens&&arrayAllocs==opens&&arrayFrees==opens);CHECK(reads==(recalc?expectedReads:0)&&crcs==reads&&!fileOpen);CHECK(cache.NeedsSaving==(dirty||recalc));CHECK(cache.Entries._M_t._M_node_count==2+(cached?1:0)+(recalc&&!cached?1:0));CHECK(lv.Checksum==0x1234&&lv.TimeStamp.Millisecond==41&&rv.Checksum==0xabcd&&rv.TimeStamp.Millisecond==42);
 if(exists){expectedPath=&path;CChecksumCacheFile::CEntry&entry=cache.Entries[path];CHECK((entry.Checksum&0xffffffffUL)==expect&&entry.TimeStamp.Millisecond==currentStamp);resetCounts();unsigned long again=cache.GetChecksum(path);CHECK((again&0xffffffffUL)==expect&&pathCalls==1&&findCalls==1&&timeCalls==1&&opens==0&&reads==0&&lookupCalls==0&&arrayAllocs==0&&arrayFrees==0);}
 else if(cached)CHECK(old->Checksum==0x81234567UL&&old->TimeStamp.Millisecond==(cached==2?currentStamp:currentStamp-1));
 freeNodes(cache.Entries._M_t._M_header._M_data->_M_parent);cache.Entries._M_t._M_header._M_data->_M_parent=0;
 }
 CHECK(cases==3072);puts("00a60fb0_TEST PASS");return 0;
}