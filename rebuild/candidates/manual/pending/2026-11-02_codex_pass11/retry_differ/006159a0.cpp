// CPersistTraits<CTCInventoryMap::CUIMiniMapMarker>::TransferIn(CPersistContext&,CStringParser&,CTCInventoryMap::CUIMiniMapMarker&) -- parse a persistent map marker and optional graphic/angle fields.
// Leaf: ?TransferIn@?$CPersistTraits@VCUIMiniMapMarker@CTCInventoryMap@@@@QAEXAAVCPersistContext@@AAVCStringParser@@AAVCUIMiniMapMarker@CTCInventoryMap@@@Z
// Grade: DIFFER(842v1127) -- retail expands far more of the CCharString/CBasicString equality and integral-trait TransferIn bodies (285 bytes) and homes the default marker's words in a different frame order; 10 shapes (all trait declaration orders, named default marker, equality branch order, header vs source equality body, one named symbol string) leave it, the trait order has no effect at all. Graded in Codex's header context (Codex CCharString.h second model + FABLE_RETAIL_MINIMAP_TRAITS per-feature macro = header debt)
#define FABLE_RETAIL_MINIMAP_TRAITS
#define FABLE_RETAIL_CSTRINGPARSER
#define FABLE_RETAIL_C2DVECTOR_UI
#include <string.h>
#include "engine/CTCInventoryMap.h"
#include "engine/CStringParser.h"
#include "engine/CPersistTraits.h"
#pragma intrinsic(strcmp)
template<class T> inline bool CBasicString<T>::operator==(const T* text)const {return Compare(PData,text)==0;}
inline bool CCharString::operator==(const char* text)const {
 if(!PStringData)return strcmp(text,"")==0;
 return PStringData->Data==text;
}
void CPersistTraits<CTCInventoryMap::CUIMiniMapMarker>::TransferIn(CPersistContext& persist_context,CStringParser& parser,CTCInventoryMap::CUIMiniMapMarker& val)
{
 CPersistTraits<float> float_traits;
 CPersistTraits<long> slong_traits;
 CPersistTraits<CCharString> string_traits;
 val=CTCInventoryMap::CUIMiniMapMarker();
 if(parser.ReadAsSymbol()=="(") {
  slong_traits.TransferIn(persist_context,parser,val.Region);
  if(parser.ReadAsSymbol()==",") {
   slong_traits.TransferIn(persist_context,parser,val.DefNameLength);
   if(parser.ReadAsSymbol()==",") {
    string_traits.TransferIn(persist_context,parser,val.UIDefName);
    if(parser.ReadAsSymbol()==",") {
     float_traits.TransferIn(persist_context,parser,val.Position.X);
     if(parser.ReadAsSymbol()==",") {
      float_traits.TransferIn(persist_context,parser,val.Position.Y);
      CCharString symbol=parser.ReadAsSymbol();
      if(symbol==",") {
       slong_traits.TransferIn(persist_context,parser,val.GraphicIndex);
       symbol=parser.ReadAsSymbol();
       if(symbol==",") {
        float_traits.TransferIn(persist_context,parser,val.Angle);
        if(parser.ReadAsSymbol()==")") {}
       }else if(symbol==")")val.Angle=0.0f;
      }else if(symbol==")") {val.GraphicIndex=0;val.Angle=0.0f;}
     }
    }
   }
  }
 }
}
