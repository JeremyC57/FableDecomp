#ifndef FABLE_ENGINE_CDEFSTRING_H
#define FABLE_ENGINE_CDEFSTRING_H
// Ego_r TPI CDefString class, size4, private TablePos+0; native parent-def
// constructors initialise this member at +25 to -1.
class CCharString;
#if defined(FABLE_RETAIL_SHOP) || defined(FABLE_RETAIL_FACTIONS)
#include "engine/CDefStringTable.h"
#endif
class CDefString {
public:
 CDefString():TablePos(-1){}
 CDefString(const CCharString&);
#if defined(FABLE_RETAIL_SHOP) || defined(FABLE_RETAIL_FACTIONS)
#ifdef FABLE_RETAIL_FACTIONS
 unsigned long GetCRC()const{return GDefStringTable.GetStringCRC(TablePos);}
#endif
 operator CCharString()const{return GDefStringTable.GetString(TablePos);}
 CCharString GetString()const{return GDefStringTable.GetString(TablePos);}
#endif
#if defined(FABLE_RETAIL_HIT_EFFECT) || defined(FABLE_RETAIL_UI_FACTORY)
 CCharString GetString() const;
#endif
private:
 long TablePos;
};
#endif
