// 009f83d0 CGraphicBankManager::OpenGraphicBank
// Leaf: ?OpenGraphicBank@CGraphicBankManager@@QAE?AV?$CCountedPointer@VCGraphicDataBank@@@@ABVCCharString@@ABVCGraphicDataBankInit@@_N2@Z
// Flags:
#include <stddef.h>
#include <new>
#include <stdlib.h>
#include "rebuild_abi.h"
#ifndef _MSC_VER
#define __forceinline inline
#endif
#define CCharString CCharStringFields
#define CCharStringData CCharStringDataFields
#include "fable_string.h"
#undef CCharStringData
#undef CCharString
// Layout: original narrow-string packed data below restore PDB members; shared header is imported only for physical comparison.
#pragma pack(push,1)
template<class T>class CBasicString{public:T*PData;unsigned long StringLength;unsigned long DataLength:31;bool UseFastExtend:1;
 bool operator==(const CBasicString&o)const{if(StringLength!=o.StringLength)return false;return Compare(PData,o.PData)?false:true;}
protected:static long __fastcall Compare(const T*,const T*);
};
#pragma auto_inline(off)
template<>long __fastcall CBasicString<char>::Compare(const char*a,const char*b){for(;;){char ca=*a;if(ca==0&&*b==0)return 0;char cb=*b;if(ca<cb)return -1;if(ca>cb)return 1;++a;++b;}}
#pragma auto_inline(on)
class CCharStringData{public:CBasicString<char>Data;long NoRefs;};
#pragma pack(pop)
FABLE_STATIC_ASSERT(sizeof(CBasicString<char>)==13);
FABLE_STATIC_ASSERT(sizeof(CCharStringData)==17);
FABLE_STATIC_ASSERT(offsetof(CCharStringData,NoRefs)==13);
FABLE_STATIC_ASSERT(offsetof(CBasicString<char>,PData)==0);
FABLE_STATIC_ASSERT(offsetof(CBasicString<char>,StringLength)==4);
FABLE_STATIC_ASSERT(sizeof(CCharStringData)==sizeof(CCharStringDataFields));
class CCharString{public:CCharStringData*PStringData;CCharString();CCharString(const char*,long=-1);CCharString(const CCharString&);~CCharString();const CCharString&operator=(const CCharString&);
 bool operator==(const CCharString&o)const{if(PStringData==o.PStringData)return true;if(PStringData&&o.PStringData)return PStringData->Data==o.PStringData->Data;return false;}
};
class CCPPointerInfo{public:unsigned long RefCount;void(__fastcall*DeleteFunc)(void*);void*Data;CCPPointerInfo(unsigned long n,void(__fastcall*f)(void*),void*p):RefCount(n),DeleteFunc(f),Data(p){}};
template<class T>class CCountedPointer{public:T*Data;CCPPointerInfo*Info;
 CCountedPointer():Data(0),Info(0){}CCountedPointer(T*p):Data(p),Info(p?new CCPPointerInfo(1,DeleteData,p):0){}
 CCountedPointer(const CCountedPointer&o):Data(o.Data),Info(o.Info){if(Info)++Info->RefCount;}
 void ReleaseData(){if(Info){--Info->RefCount;if(Info->RefCount==0){Info->DeleteFunc(Info->Data);delete Info;}}Data=0;Info=0;}
 ~CCountedPointer(){ReleaseData();}
 CCountedPointer&operator=(const CCountedPointer&o){T*data=o.Data;CCPPointerInfo*info=o.Info;if(Info!=info){ReleaseData();Data=data;Info=info;if(info)++info->RefCount;}return *this;}
 T*operator->()const{return Data;}static void __fastcall DeleteData(void*);
};
namespace std{
 template<class T>class allocator{};
 template<class P,class T,class A>class _STLP_alloc_proxy:public A{public:P _M_data;};
 struct _List_node_base{_List_node_base*_M_next,*_M_prev;};
 template<class T>struct _List_node:public _List_node_base{T _M_data;};
 struct _List_iterator_base{_List_node_base*_M_node;_List_iterator_base(_List_node_base*n):_M_node(n){}void _M_incr(){_M_node=_M_node->_M_next;}bool operator!=(const _List_iterator_base&o)const{return _M_node!=o._M_node;}bool operator==(const _List_iterator_base&o)const{return _M_node==o._M_node;}};
 template<class T>struct _Nonconst_traits{};
 template<class T,class Traits>struct _List_iterator:public _List_iterator_base{_List_iterator(_List_node<T>*n):_List_iterator_base(n){} _List_iterator(const _List_iterator&o):_List_iterator_base(o._M_node){}T*operator->()const{return &static_cast<_List_node<T>*>(this->_M_node)->_M_data;} _List_iterator&operator++(){this->_M_incr();return *this;}};
 template<class T,class A>class _List_base{protected:_STLP_alloc_proxy<_List_node<T>*,_List_node<T>,allocator<_List_node<T> > >_M_node;};
 template<class T,class A=allocator<T> >class list:public _List_base<T,A>{protected:_List_node<T>*_M_create_node(const T&x){_List_node<T>*n=(_List_node<T>*)malloc(sizeof(_List_node<T>));new(&n->_M_data)T(x);return n;}
 public:typedef _List_iterator<T,_Nonconst_traits<T> >iterator;iterator begin(){return iterator(static_cast<_List_node<T>*>(this->_M_node._M_data->_M_next));}iterator end(){return iterator(this->_M_node._M_data);}
 iterator insert(iterator pos,const T&x){_List_node<T>*n=_M_create_node(x);_List_node_base*prev=pos._M_node->_M_prev;n->_M_next=pos._M_node;n->_M_prev=prev;prev->_M_next=n;pos._M_node->_M_prev=n;return iterator(n);}void push_back(const T&x){insert(end(),x);}
 };
}
class CBaseClass{public:virtual~CBaseClass();};class CBaseClassNonCopyable:public CBaseClass{protected:virtual~CBaseClassNonCopyable();};
#define CBankFile CBankFileFields
#include "engine/CBankFile.h"
#undef CBankFile
class CBankFile:public CBaseClassNonCopyable{unsigned char unread[0x10c];public:virtual bool OpenReadOnly(const CCharString&,unsigned long);};
class CBankFileAsync:public CBankFile{unsigned char unread[0x54];public:virtual bool OpenReadOnly(const CCharString&,unsigned long);};
extern bool CUnprovenGlobal_013ca7b0;
#define CPixelFormat CPixelFormatFields
#include "engine/CPixelFormat.h"
#undef CPixelFormat
#define CGraphicDataBank CGraphicDataBankFields
#include "engine/_quarantine/CGraphicDataBank.h"
#undef CGraphicDataBank
class CPixelFormat:public CPixelFormatFields{public:CPixelFormat(){FormatIndex=-1;}};
class CGraphicDataBankInit{public:CPixelFormat NonAlphaFormat,AlphaFormat,InterpolatedAlphaFormat,BooleanAlphaFormat,UncompressedNonAlphaFormat,UncompressedAlphaFormat,SignedFormat;bool GenerateMipmaps;unsigned long MaxGraphicWidth,MaxGraphicHeight;bool AllowDither,AllowCompressedTextures,AllowDXT1ForBooleanAlpha;
 CGraphicDataBankInit():GenerateMipmaps(false),MaxGraphicWidth((unsigned long)-1),MaxGraphicHeight((unsigned long)-1),AllowDither(true),AllowCompressedTextures(false),AllowDXT1ForBooleanAlpha(true){}
};
class CResourceBank:public CBaseClassNonCopyable{unsigned char unread[0x78];};
class CGraphicDataBank:public CBankFileAsync,public CResourceBank{unsigned char unread[0x12c];public:CGraphicDataBank();void Initialise(const CGraphicDataBankInit&);};
namespace NProgressDisplay{void __fastcall DisplayProgress(const CCharString&,float,bool,bool);}
class CGraphicBankManager:public CBaseClassNonCopyable{public:
 class CGraphicDataBankInfo{public:CGraphicDataBankInit Init;CCharString BankHandle;CCountedPointer<CGraphicDataBank>GraphicDataBank;};
 std::list<CGraphicDataBankInfo>GraphicDataBanks;
 CCountedPointer<CGraphicDataBank>OpenGraphicBank(const CCharString&,const CGraphicDataBankInit&,bool,bool);
};
CCountedPointer<CGraphicDataBank>CGraphicBankManager::OpenGraphicBank(const CCharString&name,const CGraphicDataBankInit&init,bool a,bool b){
 for(std::list<CGraphicDataBankInfo>::iterator i=GraphicDataBanks.begin();i!=GraphicDataBanks.end();++i){if(i->BankHandle==name)return i->GraphicDataBank;}
 NProgressDisplay::DisplayProgress("Init Graphic Data Bank",-1.0f,false,false);
 CCountedPointer<CGraphicDataBank>pointer(new CGraphicDataBank);pointer->Initialise(init);
 unsigned long flags=0;if(!CUnprovenGlobal_013ca7b0)flags=0xd8;
 NProgressDisplay::DisplayProgress("Opening Graphic Data Bank",-1.0f,false,false);pointer->OpenReadOnly(name,flags);
 CGraphicDataBankInfo entry;entry.BankHandle=name;entry.Init=init;entry.GraphicDataBank=pointer;GraphicDataBanks.push_back(entry);return pointer;
}
FABLE_STATIC_ASSERT(sizeof(CGraphicDataBank)==0x30c);
FABLE_STATIC_ASSERT(sizeof(CGraphicBankManager::CGraphicDataBankInfo)==56);

FABLE_STATIC_ASSERT(sizeof(CGraphicDataBank)==sizeof(CGraphicDataBankFields));