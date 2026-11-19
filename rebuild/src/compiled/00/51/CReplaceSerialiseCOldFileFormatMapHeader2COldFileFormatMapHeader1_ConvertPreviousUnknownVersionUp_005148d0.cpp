// CReplaceSerialise<COldFileFormatMapHeader2, COldFileFormatMapHeader1>::ConvertPreviousUnknownVersionUp(void* old_void) -- map-header version chain, step 1 -> 2: a fresh version-2 header, the data converted up to version 1 first unless it already is version 1, the new header returned
// Leaf: ?ConvertPreviousUnknownVersionUp@?$CReplaceSerialise@VCOldFileFormatMapHeader2@@VCOldFileFormatMapHeader1@@@@SI?AV?$CCountedPointer@VCOldFileFormatMapHeader2@@@@PAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005148D0, 209 bytes, static __fastcall (hidden result ecx, data edx), plain `ret`.
// Identity: Ego_r.pdb / FableWin.pdb ?ConvertPreviousUnknownVersionUp@?$CReplaceSerialise@VCOldFileFormatMapHeader2@@VCOldFileFormatMapHeader1@@@@
//   SI?AV?$CCountedPointer@VCOldFileFormatMapHeader2@@@@PAX@Z (world_map.obj; FableWin 0x01CC8530). The catalog label
//   "CEngineWorldMap_MoveConstructor" is propagated. Callees: operator new 0x00BFEA1A, CBaseSerialise<COldFileFormatMapHeader1,1>::
//   ConvertPreviousUnknownVersionUp 0x00512C70, CCountedPointer<COldFileFormatMapHeader2>::DeleteData 0x0050D1F0, operator delete 0x00BFE9BC.
// Shape: the shared CReplaceSerialise<T,Old>::ConvertPreviousUnknownVersionUp body of the landed 0x00826C90 (FableWin 0x01CC8530 order and
//   locals: new_thing, old_thing, p_old_thing, old_void), over the shared map-header records (new overlay opt-in
//   FABLE_RETAIL_CFILEFORMAT_OLDMAPHEADER12).
#define FABLE_RETAIL_CFILEFORMATBRUSH
#define FABLE_RETAIL_CFILEFORMAT_HEIGHTMAP_V5
#define FABLE_RETAIL_CFILEFORMAT_OLDMAPHEADER12
#include "engine/CFileFormatBrush.h"
template<class T,class Old>
CCountedPointer<T> FABLE_FASTCALL CReplaceSerialise<T,Old>::ConvertPreviousUnknownVersionUp(void* old_void)
{
 CCountedPointer<T> new_thing(new T);
 CCountedPointer<Old> old_thing;
 Old* p_old_thing=(Old*)old_void;
 if(p_old_thing->SerialiseVersionNumber!=Old::GetClassVersionNumber()){
  old_thing=Old::ConvertPreviousUnknownVersionUp(old_void);
  p_old_thing=old_thing.Get();
 }
 new_thing->ConvertFromOld(p_old_thing);
 return new_thing;
}
template CCountedPointer<COldFileFormatMapHeader2> FABLE_FASTCALL
CReplaceSerialise<COldFileFormatMapHeader2,COldFileFormatMapHeader1>::ConvertPreviousUnknownVersionUp(void*);