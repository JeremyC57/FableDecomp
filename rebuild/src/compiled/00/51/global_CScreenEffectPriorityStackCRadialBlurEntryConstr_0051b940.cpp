// 0051b940 CReplaceSerialise<COldFileFormatMapHeader8,COldFileFormatMapHeader7>::ConvertPreviousUnknownVersionUp (catalog: CScreenEffectPriorityStack_CRadialBlurEntry_Constructor)
// Flags: /O2 /Oy /W3 
// Leaf: ConvertPreviousUnknownVersionUp
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8). Also: the local CCountedPointer members object->Data, control->Info are the header CCountedPointer's (same offsets); the local CCPPointerInfo members refs->RefCount, destroy->DeleteFunc, object->Data are the header CCPPointerInfo's (same offsets).
// Names: FableWin.pdb CReplaceSerialise<COldFileFormatMapHeader8,COldFileFormatMapHeader7>::ConvertPreviousUnknownVersionUp(void*) returning CCountedPointer<COldFileFormatMapHeader8>,
//        COldFileFormatMapHeader8::ConvertFromOld(const COldFileFormatMapHeader7*), CCountedPointer<T>::DeleteData(void*), CCPPointerInfo; sizes from Ego_r.pdb.
#include <new>
// Layout: the serialised header opens with its version byte at +0x04.
class COldFileFormatMapHeader7{ public:char pad[4];unsigned char VersionByte;};
class COldFileFormatMapHeader6;
struct CFileLargeThemePalette{char storage[0x8400];CFileLargeThemePalette();};
// Layout: exact retail allocation 0x1081e; LargeThemePalette at +0x16 and AtmosThemePalette at +0x841e each span 0x8400 bytes; prefix 0x16, VersionNumber/NoSoundThemeEntries 0x08. CCPPointerInfo size 0x0c: refs +0x00, deleter +0x04, data +0x08.
#pragma pack(push,1)
class COldFileFormatMapHeader8{ public:char prefix[0x16];CFileLargeThemePalette LargeThemePalette;char gap[0x08];CFileLargeThemePalette AtmosThemePalette;__forceinline COldFileFormatMapHeader8(){}void ConvertFromOld(const COldFileFormatMapHeader7*);};
#pragma pack(pop)
#include "engine/CCountedPointer.h"
template<class TNew,class TOld> struct CReplaceSerialise{static CCountedPointer<TNew> __fastcall ConvertPreviousUnknownVersionUp(void*);};
template<> CCountedPointer<COldFileFormatMapHeader8> __fastcall CReplaceSerialise<COldFileFormatMapHeader8,COldFileFormatMapHeader7>::ConvertPreviousUnknownVersionUp(void*data){
 CCountedPointer<COldFileFormatMapHeader8> result(new COldFileFormatMapHeader8);CCountedPointer<COldFileFormatMapHeader7> held((COldFileFormatMapHeader7*)0);COldFileFormatMapHeader7*old=(COldFileFormatMapHeader7*)data;
 if(((COldFileFormatMapHeader7*)data)->VersionByte!=7){held=CReplaceSerialise<COldFileFormatMapHeader7,COldFileFormatMapHeader6>::ConvertPreviousUnknownVersionUp(data);old=held.Data;}
 result.Data->ConvertFromOld(old);return CCountedPointer<COldFileFormatMapHeader8>(result);
}
