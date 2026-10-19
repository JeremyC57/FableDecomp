// C3DVector::ToString() const -- format the vector as "(X, Y, Z)" with two decimals
// Retail 00A13DA0. Calls CCharString::FormatDouble 009A08B0, constructor 0099EBF0,
// concatenation 0099F570/0099F600 and destructor 0099EAE0. No shared C3DVector header exists.
// Verified literals: '(' at0122E028, ', ' at012397CC, ')' at0122E020.
#if !defined(_MSC_VER)
#define __fastcall
#endif
struct CCharString { void* PStringData; CCharString(const char* text, long length=-1); ~CCharString(); static CCharString __fastcall FormatDouble(long digits, double value); };
CCharString __fastcall operator+(const CCharString& left,const CCharString& right);
CCharString __fastcall operator+(const CCharString& left,const char* right);
struct C3DVector { float X,Y,Z; CCharString ToString() const; };
CCharString C3DVector::ToString() const { return CCharString("(") + CCharString::FormatDouble(2,X) + ", " + CCharString::FormatDouble(2,Y) + ", " + CCharString::FormatDouble(2,Z) + ")"; }