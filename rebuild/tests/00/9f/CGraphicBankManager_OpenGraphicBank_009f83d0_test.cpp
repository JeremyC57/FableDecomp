#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#ifndef _MSC_VER
#define __cdecl
#define __fastcall
#define __stdcall
#define __declspec(x)
#define __forceinline inline
#define __int64 __attribute__((__mode__(__DI__))) long
#endif
#define PASS9_TEST

#ifndef _MSC_VER
// Layout: GNU-only MS bitfield ABI adaptation; all original fields and operations remain unchanged.
#define PASS9_MS_STRUCT __attribute__((ms_struct))
#else
#define PASS9_MS_STRUCT
#endif
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
template<class T>class PASS9_MS_STRUCT CBasicString{public:T*PData;unsigned long StringLength;unsigned long DataLength:31;bool UseFastExtend:1;
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

#undef PASS9_MS_STRUCT

static int testError,testOpenCalls,testConstructors,testInitialises,testProgress,testDeletes,testPhase;static unsigned long testFlags;static bool testOpenResult;static const CCharString*testName;static void*testOpened;
bool CUnprovenGlobal_013ca7b0;
static const char*testChars(const CCharString&s){return s.PStringData?s.PStringData->Data.PData:"";}
static void testStringRelease(CCharStringData*&data){if(data&&--data->NoRefs==0){delete[]data->Data.PData;delete data;}data=0;}
CCharString::CCharString():PStringData(0){}
CCharString::CCharString(const char*text,long length):PStringData(0){if(length<0)length=strlen(text);if(length){PStringData=new CCharStringData;PStringData->Data.PData=new char[length+1];memcpy(PStringData->Data.PData,text,length);PStringData->Data.PData[length]=0;PStringData->Data.StringLength=length;PStringData->Data.DataLength=length;PStringData->Data.UseFastExtend=false;PStringData->NoRefs=1;}}
CCharString::CCharString(const CCharString&o):PStringData(o.PStringData){if(PStringData)++PStringData->NoRefs;}
CCharString::~CCharString(){testStringRelease(PStringData);}
const CCharString&CCharString::operator=(const CCharString&o){if(PStringData!=o.PStringData){testStringRelease(PStringData);PStringData=o.PStringData;if(PStringData)++PStringData->NoRefs;}return *this;}
CBaseClass::~CBaseClass(){}CBaseClassNonCopyable::~CBaseClassNonCopyable(){}
template<class T>void __fastcall CCountedPointer<T>::DeleteData(void*p){++testDeletes;delete static_cast<T*>(p);}
static bool testRecordOpen(void*self,const CCharString&name,unsigned long flags){if(&name!=testName||flags!=testFlags)testError=1;testOpened=self;++testOpenCalls;return testOpenResult;}
bool CBankFile::OpenReadOnly(const CCharString&name,unsigned long flags){return testRecordOpen(this,name,flags);}
bool CBankFileAsync::OpenReadOnly(const CCharString&name,unsigned long flags){return testRecordOpen(this,name,flags);}

CGraphicDataBank::CGraphicDataBank(){++testConstructors;}

static CGraphicDataBankInit testInit;
static bool testSameInit(const CGraphicDataBankInit&a,const CGraphicDataBankInit&b){return a.NonAlphaFormat.FormatIndex==b.NonAlphaFormat.FormatIndex&&a.AlphaFormat.FormatIndex==b.AlphaFormat.FormatIndex&&a.InterpolatedAlphaFormat.FormatIndex==b.InterpolatedAlphaFormat.FormatIndex&&a.BooleanAlphaFormat.FormatIndex==b.BooleanAlphaFormat.FormatIndex&&a.UncompressedNonAlphaFormat.FormatIndex==b.UncompressedNonAlphaFormat.FormatIndex&&a.UncompressedAlphaFormat.FormatIndex==b.UncompressedAlphaFormat.FormatIndex&&a.SignedFormat.FormatIndex==b.SignedFormat.FormatIndex&&a.GenerateMipmaps==b.GenerateMipmaps&&a.MaxGraphicWidth==b.MaxGraphicWidth&&a.MaxGraphicHeight==b.MaxGraphicHeight&&a.AllowDither==b.AllowDither&&a.AllowCompressedTextures==b.AllowCompressedTextures&&a.AllowDXT1ForBooleanAlpha==b.AllowDXT1ForBooleanAlpha;}
void CGraphicDataBank::Initialise(const CGraphicDataBankInit&i){if(testPhase!=1||!testSameInit(i,testInit))testError=2;++testInitialises;testPhase=2;}
void __fastcall NProgressDisplay::DisplayProgress(const CCharString&message,float value,bool first,bool second){if(value!=-1.0f||first||second)testError=3;const char*text=testChars(message);if(testProgress==0){if(strcmp(text,"Init Graphic Data Bank")||testPhase!=0)testError=4;testPhase=1;}else{if(strcmp(text,"Opening Graphic Data Bank")||testPhase!=2)testError=5;testPhase=3;}++testProgress;}

int main(){const char*names[]={"bank","CGraphicDataBank","stone",""};int cases=0;
 for(int nameIndex=0;nameIndex<4;++nameIndex)for(int shared=0;shared<2;++shared)for(int matched=0;matched<3;++matched){
  CGraphicBankManager manager;typedef CGraphicBankManager::CGraphicDataBankInfo Entry;std::_List_node<Entry>head,nodes[3];head._M_next=&nodes[0];head._M_prev=&nodes[2];for(int i=0;i<3;++i){nodes[i]._M_prev=i?static_cast<std::_List_node_base*>(&nodes[i-1]):&head;nodes[i]._M_next=i<2?static_cast<std::_List_node_base*>(&nodes[i+1]):&head;nodes[i]._M_data.BankHandle=CCharString("mismatch");}
  std::_List_node<Entry>*headPointer=&head;memcpy(&manager.GraphicDataBanks,&headPointer,sizeof headPointer);CCharString name(names[nameIndex]);testName=&name;
  {CCountedPointer<CGraphicDataBank>owner(new CGraphicDataBank);if(shared)nodes[matched]._M_data.BankHandle=name;else nodes[matched]._M_data.BankHandle=CCharString(names[nameIndex]);nodes[matched]._M_data.GraphicDataBank=owner;
   CGraphicDataBankInit init;memset(&init,0,sizeof init);init.NonAlphaFormat.FormatIndex=17;init.AlphaFormat.FormatIndex=3;init.MaxGraphicWidth=256;init.MaxGraphicHeight=512;init.GenerateMipmaps=true;init.AllowDither=true;testInit=init;
   testError=testOpenCalls=testConstructors=testInitialises=testProgress=testDeletes=testPhase=0;
   {CCountedPointer<CGraphicDataBank>result=manager.OpenGraphicBank(name,init,false,true);if(result.Data!=owner.Data||result.Info!=owner.Info||owner.Info->RefCount!=3||testOpenCalls||testConstructors||testInitialises||testProgress||testError)return 1;}
   if(owner.Info->RefCount!=2)return 2;nodes[matched]._M_data.GraphicDataBank.ReleaseData();if(owner.Info->RefCount!=1)return 3;
  }
  if(testDeletes!=1||head._M_next!=&nodes[0]||head._M_prev!=&nodes[2])return 4;++cases;
 }
 for(int nameIndex=0;nameIndex<4;++nameIndex)for(int mode=0;mode<2;++mode)for(int open=0;open<2;++open){
  CGraphicBankManager manager;typedef CGraphicBankManager::CGraphicDataBankInfo Entry;std::_List_node<Entry>head;head._M_next=&head;head._M_prev=&head;std::_List_node<Entry>*headPointer=&head;memcpy(&manager.GraphicDataBanks,&headPointer,sizeof headPointer);CCharString name(names[nameIndex]);testName=&name;CGraphicDataBankInit init;memset(&init,0,sizeof init);init.NonAlphaFormat.FormatIndex=17;init.AlphaFormat.FormatIndex=3;init.MaxGraphicWidth=256;init.MaxGraphicHeight=512;init.GenerateMipmaps=true;init.AllowDither=true;testInit=init;
  CUnprovenGlobal_013ca7b0=mode!=0;testFlags=mode?0:216;testOpenResult=open!=0;testError=testOpenCalls=testConstructors=testInitialises=testProgress=testDeletes=testPhase=0;testOpened=0;
  {CCountedPointer<CGraphicDataBank>result=manager.OpenGraphicBank(name,init,false,true);std::_List_node<Entry>*created=static_cast<std::_List_node<Entry>*>(head._M_next);
   if(!result.Data||!result.Info||result.Info->RefCount!=2||created==&head||created!=head._M_prev||created->_M_next!=&head||created->_M_prev!=&head||created->_M_data.GraphicDataBank.Data!=result.Data||created->_M_data.GraphicDataBank.Info!=result.Info||strcmp(testChars(created->_M_data.BankHandle),testChars(name)))return 5;
   if(testError||testConstructors!=1||testOpenCalls!=1||testOpened!=result.Data||testDeletes||testInitialises!=1||testProgress!=2)return 6;
   if(!testSameInit(created->_M_data.Init,init))return 10;
  }
  std::_List_node<Entry>*created=static_cast<std::_List_node<Entry>*>(head._M_next);if(created->_M_data.GraphicDataBank.Info->RefCount!=1)return 7;created->_M_data.~Entry();free(created);head._M_next=&head;head._M_prev=&head;if(testDeletes!=1)return 8;++cases;
 }
 if(cases!=40)return 9;puts("009f83d0_TEST PASS");return 0;}