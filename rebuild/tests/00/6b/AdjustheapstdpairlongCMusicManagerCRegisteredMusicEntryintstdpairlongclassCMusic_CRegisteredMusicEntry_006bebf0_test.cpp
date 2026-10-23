// 006bebf0 N006bebf0::__adjust_heap<Element,CKeyPairCompareLess> (STLport; catalog: _Adjust_heap<std::pair<long,CMusicManager::CRegisteredMusicEntry>*,int,std::pair<long,class_CMusicManager::CRegisteredMusicEntry>,CKeyPairCompareLess<long,CMusicManager::_Adjust_heap<std::pair<long,CM)
// Flags: /O2 /Oy /W3 /Gr
// Leaf: __adjust_heap
// Layout: PDB19267-19276 CRegisteredMusicEntry20 has two WideStrings0/4, Gain8, Offset12, CacheToHD16; pair24 places it at4. Retail99b758/99b7f5 increments shared DWORD+12.
#include <new>
namespace N006bebf0 {
struct CWideStringData;
struct CWideString {CWideStringData* Data;CWideString();CWideString(const CWideString&);~CWideString();CWideString& operator=(const CWideString&);};
struct CRegisteredMusicEntry {CWideString SrcFileName,DstFileName;float Gain,Offset;bool CacheToHD;};
struct Element {int Key;CRegisteredMusicEntry Music;};
struct CKeyPairCompareLess {unsigned int State;__forceinline bool operator()(const Element& a,const Element& b)const{return a.Key<b.Key;}};
template<class T,class C>void __fastcall __push_heap(T*,int,int,T,C);
template<class T,class C>void __fastcall __adjust_heap(T* first,int hole,int length,T value,C comp){
 int top=hole;int second=2*hole+2;
 while(second<length){if(comp(first[second],first[second-1]))--second;first[hole]=first[second];hole=second;second=2*(second+1);}
 if(second==length){first[hole]=first[second-1];hole=second-1;}
 __push_heap(first,hole,top,value,comp);
}
template void __fastcall __adjust_heap<Element,CKeyPairCompareLess>(Element*,int,int,Element,CKeyPairCompareLess);
}

#include <cstdio>
#include <cstring>
#include <cwchar>
#include <cstdlib>
namespace N006bebf0 {
struct CWideStringData {wchar_t* Text;unsigned int Length,Capacity,Refs;};
static int liveBlocks,liveTexts,destroyedBlocks;
void Release(CWideStringData* p){if(p&&--p->Refs==0){std::free(p->Text);--liveTexts;delete p;--liveBlocks;++destroyedBlocks;}}
CWideString::CWideString():Data(0){}CWideString::CWideString(const CWideString& o):Data(o.Data){if(Data)++Data->Refs;}CWideString::~CWideString(){Release(Data);}
CWideString& CWideString::operator=(const CWideString& o){if(Data!=o.Data){CWideStringData* next=o.Data;if(next)++next->Refs;Release(Data);Data=next;}return *this;}
void Set(CWideString& s,const wchar_t* text){CWideString fresh;fresh.Data=new CWideStringData;fresh.Data->Length=(unsigned int)std::wcslen(text);fresh.Data->Capacity=fresh.Data->Length+1;fresh.Data->Text=(wchar_t*)std::malloc(fresh.Data->Capacity*sizeof(wchar_t));std::memcpy(fresh.Data->Text,text,fresh.Data->Capacity*sizeof(wchar_t));fresh.Data->Refs=1;++liveBlocks;++liveTexts;s=fresh;}
void Init(Element& e,int key,int id){e.Key=key;Set(e.Music.SrcFileName,L"source.wav");Set(e.Music.DstFileName,L"destination.wav");e.Music.Gain=(float)(id+1);e.Music.Offset=(float)(id+1)*0.5f;e.Music.CacheToHD=(id&1)!=0;}
bool Payload(const Element& e,int id){return e.Music.Gain==(float)(id+1)&&e.Music.Offset==(float)(id+1)*0.5f&&e.Music.CacheToHD==((id&1)!=0)&&std::wcscmp(e.Music.SrcFileName.Data->Text,L"source.wav")==0&&std::wcscmp(e.Music.DstFileName.Data->Text,L"destination.wav")==0;}
template<class T,class C>void __fastcall __push_heap(T* first,int hole,int top,T value,C comp){int parent=(hole-1)/2;while(hole>top&&comp(first[parent],value)){first[hole]=first[parent];hole=parent;parent=(hole-1)/2;}first[hole]=value;}
bool Cross255(){int before=destroyedBlocks;{CWideString root;Set(root,L"shared wide payload");CWideString* copies=(CWideString*)::operator new(300*sizeof(CWideString));for(int i=0;i<300;++i)new(copies+i)CWideString(root);if(root.Data->Refs!=301||root.Data->Text[1]!=L'h')return false;copies[0]=root;if(root.Data->Refs!=301)return false;for(int j=0;j<45;++j)copies[j].~CWideString();if(root.Data->Refs!=256)return false;copies[45].~CWideString();if(root.Data->Refs!=255)return false;for(int k=46;k<300;++k)copies[k].~CWideString();if(root.Data->Refs!=1)return false;::operator delete(copies);}return destroyedBlocks==before+1&&liveBlocks==0&&liveTexts==0;}
}
int main(){using namespace N006bebf0;
 if(sizeof(Element)!=24||sizeof(CRegisteredMusicEntry)!=20||sizeof(CWideStringData)!=16||!Cross255())return 1;
 {CWideString a,b;Set(a,L"first");Set(b,L"second");CWideStringData* shared=b.Data;int before=destroyedBlocks;a=b;if(a.Data!=shared||shared->Refs!=2||destroyedBlocks!=before+1)return 2;a=a;b=a;if(shared->Refs!=2)return 3;}
 if(liveBlocks||liveTexts)return 4;
 {Element a[7];for(int i=0;i<7;++i)Init(a[i],7-i,i);CKeyPairCompareLess comp;comp.State=0;Element x=a[0];x.Key=1;__adjust_heap(a,0,7,x,comp);
 if(a[0].Key!=6||!Payload(a[0],1)||a[1].Key!=4||!Payload(a[1],3)||a[3].Key!=1||!Payload(a[3],0)||a[3].Music.SrcFileName.Data!=x.Music.SrcFileName.Data||a[3].Music.DstFileName.Data!=x.Music.DstFileName.Data)return 5;
 Element y=a[0];y.Key=10;__push_heap(a,6,0,y,comp);if(a[0].Key!=10||!Payload(a[0],1)||a[0].Music.SrcFileName.Data!=y.Music.SrcFileName.Data)return 6;
 Element z=a[0];z.Key=20;__adjust_heap(a,0,1,z,comp);if(a[0].Key!=20||a[0].Music.DstFileName.Data!=z.Music.DstFileName.Data)return 7;
 Element b[2];Init(b[0],2,8);Init(b[1],1,9);Element v=b[0];v.Key=0;__adjust_heap(b,0,2,v,comp);if(b[0].Key!=1||!Payload(b[0],9)||b[1].Key!=0||!Payload(b[1],8))return 8;}
 if(liveBlocks||liveTexts)return 9;std::printf("006bebf0_TEST PASS\n");return 0;}