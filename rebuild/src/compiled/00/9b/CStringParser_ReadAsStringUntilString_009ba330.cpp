// CStringParser::ReadAsStringUntilString(const CCharString& delimiter) -- Return the text consumed by the delimiter scan.
// Leaf: ?ReadAsStringUntilString@CStringParser@@QAE?AVCCharString@@ABV2@@Z
// Identity: own delimiter helper009b9a20, original PDB return and reference signature.
#define CStringParser CStringParser_HeaderLayout
#include "engine/CStringParser.h"
#undef CStringParser
class CCharString {public:CCharStringData* PStringData;CCharString(const char*,long);CCharString(const CCharString&);~CCharString();};
// Layout: shared parser owns opaque CCursorPos12 at+28; original InQuotes+0, StringPos+4, Line+8.
class CCursorPos{public:bool InQuotes;unsigned long StringPos,Line;};
class CStringParser:public CStringParser_HeaderLayout{public:bool SkipUntilString(const CCharString&);CCharString ReadAsStringUntilString(const CCharString&);};
CCharString CStringParser::ReadAsStringUntilString(const CCharString& delimiter){const char* start=(const char*)String+reinterpret_cast<CCursorPos*>(CursorPos)->StringPos;if(SkipUntilString(delimiter))return CCharString(start,(const char*)String+reinterpret_cast<CCursorPos*>(CursorPos)->StringPos-start);return CCharString("",-1);}