// CReplaceSerialise::ConvertPreviousUnknownVersionUp -- upgrade a height-map cell to version5.
// Retail: 0x00826c90,288 bytes,static fastcall with hidden value return.
// Leaf: ?ConvertPreviousUnknownVersionUp@?$CReplaceSerialise@VCOldFileFormatHeightMapCell5@@VCOldFileFormatHeightMapCell4@@@@SI?AV?$CCountedPointer@VCOldFileFormatHeightMapCell5@@@@PAX@Z
#define FABLE_RETAIL_CFILEFORMATBRUSH
#define FABLE_RETAIL_CFILEFORMAT_HEIGHTMAP_V5
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
template<> CCountedPointer<COldFileFormatHeightMapCell4> FABLE_FASTCALL
CReplaceSerialise<COldFileFormatHeightMapCell4,COldFileFormatHeightMapCell3>::ConvertPreviousUnknownVersionUp(void*);
template CCountedPointer<COldFileFormatHeightMapCell5> FABLE_FASTCALL
CReplaceSerialise<COldFileFormatHeightMapCell5,COldFileFormatHeightMapCell4>::ConvertPreviousUnknownVersionUp(void*);