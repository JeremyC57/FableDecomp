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
