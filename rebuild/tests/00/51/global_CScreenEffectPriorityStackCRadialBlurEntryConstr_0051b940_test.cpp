// 0051b940 CReplaceSerialise<COldFileFormatMapHeader8,COldFileFormatMapHeader7>::ConvertPreviousUnknownVersionUp (catalog: CScreenEffectPriorityStack_CRadialBlurEntry_Constructor)
// Flags: /O2 /Oy /W3 
// Leaf: ConvertPreviousUnknownVersionUp
// Names: FableWin.pdb CReplaceSerialise<COldFileFormatMapHeader8,COldFileFormatMapHeader7>::ConvertPreviousUnknownVersionUp(void*) returning CCountedPointer<COldFileFormatMapHeader8>,
//        COldFileFormatMapHeader8::ConvertFromOld(const COldFileFormatMapHeader7*), CCountedPointer<T>::DeleteData(void*), CCPPointerInfo; sizes from Ego_r.pdb.
#include <new>
// Layout: the serialised header opens with its version byte at +0x04.
struct COldFileFormatMapHeader7{char pad[4];unsigned char VersionByte;};
struct COldFileFormatMapHeader6;
struct CFileLargeThemePalette{char storage[0x8400];CFileLargeThemePalette();};
// Layout: exact retail allocation 0x1081e; LargeThemePalette at +0x16 and AtmosThemePalette at +0x841e each span 0x8400 bytes; prefix 0x16, VersionNumber/NoSoundThemeEntries 0x08. CCPPointerInfo size 0x0c: refs +0x00, deleter +0x04, data +0x08.
#pragma pack(push,1)
struct COldFileFormatMapHeader8{char prefix[0x16];CFileLargeThemePalette LargeThemePalette;char gap[0x08];CFileLargeThemePalette AtmosThemePalette;__forceinline COldFileFormatMapHeader8(){}void ConvertFromOld(const COldFileFormatMapHeader7*);};
#pragma pack(pop)
struct CCPPointerInfo{unsigned refs;void(__fastcall*destroy)(void*);void*object;__forceinline CCPPointerInfo(void*p,void(__fastcall*d)(void*)):refs(1),destroy(d),object(p){}};
template<class T> struct CCountedPointer{T*object;CCPPointerInfo*control;
 static void __fastcall DeleteData(void*);
 __forceinline CCountedPointer(T*p):object(p),control(p?new CCPPointerInfo(p,DeleteData):0){}
 __forceinline CCountedPointer(const CCountedPointer&x):object(x.object),control(x.control){if(control)++control->refs;}
 __forceinline CCountedPointer&operator=(const CCountedPointer&x){CCPPointerInfo*c=x.control;T*o=x.object;if(control!=c){if(control&&--control->refs==0){control->destroy(control->object);operator delete(control);}object=o;control=c;if(control)++control->refs;}return *this;}
 __forceinline ~CCountedPointer(){if(control&&--control->refs==0){control->destroy(control->object);operator delete(control);}}
};
template<class TNew,class TOld> struct CReplaceSerialise{static CCountedPointer<TNew> __fastcall ConvertPreviousUnknownVersionUp(void*);};
template<> CCountedPointer<COldFileFormatMapHeader8> __fastcall CReplaceSerialise<COldFileFormatMapHeader8,COldFileFormatMapHeader7>::ConvertPreviousUnknownVersionUp(void*data){
 CCountedPointer<COldFileFormatMapHeader8> result(new COldFileFormatMapHeader8);CCountedPointer<COldFileFormatMapHeader7> held((COldFileFormatMapHeader7*)0);COldFileFormatMapHeader7*old=(COldFileFormatMapHeader7*)data;
 if(((COldFileFormatMapHeader7*)data)->VersionByte!=7){held=CReplaceSerialise<COldFileFormatMapHeader7,COldFileFormatMapHeader6>::ConvertPreviousUnknownVersionUp(data);old=held.object;}
 result.object->ConvertFromOld(old);return CCountedPointer<COldFileFormatMapHeader8>(result);
}

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stddef.h>
struct COldFileFormatMapHeader6{int unused;};
static int blocks,applies,deleted,earlierCalls,refsAtApply;static bool returnEmpty;static const COldFileFormatMapHeader7*seen;static COldFileFormatMapHeader7*liveObject;static CCPPointerInfo*liveControl;
CFileLargeThemePalette::CFileLargeThemePalette(){++blocks;memset(storage,0,sizeof(storage));}
void COldFileFormatMapHeader8::ConvertFromOld(const COldFileFormatMapHeader7*p){seen=p;++applies;if(p==liveObject&&liveObject){assert(liveControl&&liveControl->refs==1);refsAtApply=liveControl->refs;}prefix[0]=37;}
template<class T> void __fastcall CCountedPointer<T>::DeleteData(void*p){++deleted;if(p==liveObject){assert(liveControl->refs==0);liveObject=0;liveControl=0;}operator delete(p);}
template<> CCountedPointer<COldFileFormatMapHeader7> __fastcall CReplaceSerialise<COldFileFormatMapHeader7,COldFileFormatMapHeader6>::ConvertPreviousUnknownVersionUp(void*){++earlierCalls;if(returnEmpty)return CCountedPointer<COldFileFormatMapHeader7>((COldFileFormatMapHeader7*)0);CCountedPointer<COldFileFormatMapHeader7> out(new COldFileFormatMapHeader7);memset(out.object,0,sizeof(COldFileFormatMapHeader7));liveObject=out.object;liveControl=out.control;return CCountedPointer<COldFileFormatMapHeader7>(out);}
typedef CCountedPointer<COldFileFormatMapHeader8> Ptr;
int main(){COldFileFormatMapHeader7 in;memset(&in,0,sizeof(in));in.VersionByte=7;
 {Ptr p=CReplaceSerialise<COldFileFormatMapHeader8,COldFileFormatMapHeader7>::ConvertPreviousUnknownVersionUp(&in);assert(p.object&&p.control->refs==1&&seen==&in&&blocks==2&&earlierCalls==0&&p.object->prefix[0]==37);assert(sizeof(CFileLargeThemePalette)==0x8400&&sizeof(COldFileFormatMapHeader8)==0x1081e&&offsetof(COldFileFormatMapHeader8,LargeThemePalette)==0x16&&offsetof(COldFileFormatMapHeader8,AtmosThemePalette)==0x841e);for(unsigned j=0;j<0x8400;++j)assert(p.object->LargeThemePalette.storage[j]==0&&p.object->AtmosThemePalette.storage[j]==0);{Ptr copied(p);assert(copied.object==p.object&&p.control->refs==2);copied=copied;assert(p.control->refs==2);}assert(p.control->refs==1);}assert(deleted==1);
 in.VersionByte=0;{Ptr p=CReplaceSerialise<COldFileFormatMapHeader8,COldFileFormatMapHeader7>::ConvertPreviousUnknownVersionUp(&in);assert(p.object&&p.control->refs==1&&applies==2&&blocks==4&&earlierCalls==1&&refsAtApply==1&&!liveObject&&!liveControl&&deleted==2);}assert(deleted==3);
 returnEmpty=true;{Ptr p=CReplaceSerialise<COldFileFormatMapHeader8,COldFileFormatMapHeader7>::ConvertPreviousUnknownVersionUp(&in);assert(p.object&&p.control->refs==1&&seen==0&&applies==3&&earlierCalls==2&&blocks==6&&deleted==3);}assert(deleted==4);
 puts("0051b940_TEST PASS");}