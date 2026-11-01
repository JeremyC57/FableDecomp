// 00a60fb0 CChecksumCacheFile::GetChecksum
// Leaf: ?GetChecksum@CChecksumCacheFile@@QAEKABVCWideString@@@Z
// Flags:
#include <new>
void*__cdecl operator new[](unsigned int);
void __cdecl operator delete[](void*);
namespace std{
typedef unsigned int size_t;typedef int ptrdiff_t;
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
 _Rb_tree_base(const allocator_type&);~_Rb_tree_base();
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
#include "engine/CDiskFileWin32.h"
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
FABLE_STATIC_ASSERT(sizeof(CDiskFileWin32)==sizeof(CDiskFileWin32Fields));
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