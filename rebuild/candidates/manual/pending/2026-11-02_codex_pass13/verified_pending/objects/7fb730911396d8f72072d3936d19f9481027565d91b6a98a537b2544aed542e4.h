#ifndef FABLE_ENGINE_CDEFSTRING_H
#define FABLE_ENGINE_CDEFSTRING_H
// Ego_r TPI CDefString class, size4, private TablePos+0; native parent-def
// constructors initialise this member at +25 to -1.
class CCharString;
#ifndef FABLE_RETAIL_CDEFSTRINGTABLE
#error engine/CDefString.h needs the retail CDefStringTable: define FABLE_RETAIL_CDEFSTRINGTABLE (and FABLE_RETAIL_CCHARSTRING) before including it
#endif
#include "fable_definition_table.h"
class CDefString {
public:
 CDefString():TablePos(-1){}
 CDefString(const CCharString&);
#ifdef FABLE_RETAIL_CDEFSTRING_EXTERNAL_CONVERSION
 // Retail opt-in (Codex pass 13): exact original const conversion, called at00415d70 from00ec5de0.
 operator CCharString() const;
#else
 operator CCharString()const{return GDefStringTable.GetString(TablePos);}
#endif
 CCharString GetString()const{return GDefStringTable.GetString(TablePos);}
private:
 long TablePos;
};
#endif
