// 0051c2f0 CReplaceSerialise<CFileFormatMapHeader,COldFileFormatMapHeader8>::ConvertPreviousUnknownVersionUp (catalog: CRigidBody_CRigidBodyConfig_Constructor)
// Flags: /O2 /Oy /W3 
// Leaf: ConvertPreviousUnknownVersionUp
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8). Also: the local CCountedPointer members object->Data, control->Info are the header CCountedPointer's (same offsets); the local CCPPointerInfo members refs->RefCount, destroy->DeleteFunc, object->Data are the header CCPPointerInfo's (same offsets).
// Names: FableWin.pdb CReplaceSerialise<CFileFormatMapHeader,COldFileFormatMapHeader8>::ConvertPreviousUnknownVersionUp(void*) returning CCountedPointer<CFileFormatMapHeader>,
//        CFileFormatMapHeader::ConvertFromOld(const COldFileFormatMapHeader8*), CCountedPointer<T>::DeleteData(void*), CCPPointerInfo; sizes from Ego_r.pdb.
#include "fable_compiler.h"
#include <new>
// Layout: the serialised header opens with its version byte at +0x04.
class COldFileFormatMapHeader8{ public:char pad[4];unsigned char VersionByte;};
class COldFileFormatMapHeader7;
struct CFileLargeThemePalette{char storage[0x8400];CFileLargeThemePalette();};
// Layout: exact retail allocation 0x10822; LargeThemePalette at +0x16 and AtmosThemePalette at +0x841e each span 0x8400 bytes; prefix 0x16, VersionNumber/NoSoundThemeEntries 0x08, StaticMapChecksum 0x04. CCPPointerInfo size 0x0c: refs +0x00, deleter +0x04, data +0x08.
#pragma pack(push,1)
class CFileFormatMapHeader{ public:char prefix[0x16];CFileLargeThemePalette LargeThemePalette;char gap[0x08];CFileLargeThemePalette AtmosThemePalette;char StaticMapChecksum[0x04];FABLE_FORCEINLINE CFileFormatMapHeader(){}void ConvertFromOld(const COldFileFormatMapHeader8*);};
#pragma pack(pop)
#include "engine/CCountedPointer.h"
template<class TNew,class TOld> struct CReplaceSerialise{static CCountedPointer<TNew> __fastcall ConvertPreviousUnknownVersionUp(void*);};
template<> CCountedPointer<CFileFormatMapHeader> __fastcall CReplaceSerialise<CFileFormatMapHeader,COldFileFormatMapHeader8>::ConvertPreviousUnknownVersionUp(void*data){
 CCountedPointer<CFileFormatMapHeader> result(new CFileFormatMapHeader);CCountedPointer<COldFileFormatMapHeader8> held((COldFileFormatMapHeader8*)0);COldFileFormatMapHeader8*old=(COldFileFormatMapHeader8*)data;
 if(((COldFileFormatMapHeader8*)data)->VersionByte!=8){held=CReplaceSerialise<COldFileFormatMapHeader8,COldFileFormatMapHeader7>::ConvertPreviousUnknownVersionUp(data);old=held.Data;}
 result.Data->ConvertFromOld(old);return CCountedPointer<CFileFormatMapHeader>(result);
}
