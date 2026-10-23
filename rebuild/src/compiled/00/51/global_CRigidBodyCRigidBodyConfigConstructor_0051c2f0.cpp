// 0051c2f0 CReplaceSerialise<CFileFormatMapHeader,COldFileFormatMapHeader8>::ConvertPreviousUnknownVersionUp (catalog: CRigidBody_CRigidBodyConfig_Constructor)
// Flags: /O2 /Oy /W3 
// Leaf: ConvertPreviousUnknownVersionUp
// Names: FableWin.pdb CReplaceSerialise<CFileFormatMapHeader,COldFileFormatMapHeader8>::ConvertPreviousUnknownVersionUp(void*) returning CCountedPointer<CFileFormatMapHeader>,
//        CFileFormatMapHeader::ConvertFromOld(const COldFileFormatMapHeader8*), CCountedPointer<T>::DeleteData(void*), CCPPointerInfo; sizes from Ego_r.pdb.
#include <new>
// Layout: the serialised header opens with its version byte at +0x04.
struct COldFileFormatMapHeader8{char pad[4];unsigned char VersionByte;};
struct COldFileFormatMapHeader7;
struct CFileLargeThemePalette{char storage[0x8400];CFileLargeThemePalette();};
// Layout: exact retail allocation 0x10822; LargeThemePalette at +0x16 and AtmosThemePalette at +0x841e each span 0x8400 bytes; prefix 0x16, VersionNumber/NoSoundThemeEntries 0x08, StaticMapChecksum 0x04. CCPPointerInfo size 0x0c: refs +0x00, deleter +0x04, data +0x08.
#pragma pack(push,1)
struct CFileFormatMapHeader{char prefix[0x16];CFileLargeThemePalette LargeThemePalette;char gap[0x08];CFileLargeThemePalette AtmosThemePalette;char StaticMapChecksum[0x04];__forceinline CFileFormatMapHeader(){}void ConvertFromOld(const COldFileFormatMapHeader8*);};
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
template<> CCountedPointer<CFileFormatMapHeader> __fastcall CReplaceSerialise<CFileFormatMapHeader,COldFileFormatMapHeader8>::ConvertPreviousUnknownVersionUp(void*data){
 CCountedPointer<CFileFormatMapHeader> result(new CFileFormatMapHeader);CCountedPointer<COldFileFormatMapHeader8> held((COldFileFormatMapHeader8*)0);COldFileFormatMapHeader8*old=(COldFileFormatMapHeader8*)data;
 if(((COldFileFormatMapHeader8*)data)->VersionByte!=8){held=CReplaceSerialise<COldFileFormatMapHeader8,COldFileFormatMapHeader7>::ConvertPreviousUnknownVersionUp(data);old=held.object;}
 result.object->ConvertFromOld(old);return CCountedPointer<CFileFormatMapHeader>(result);
}
