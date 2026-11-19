// C3DVector::ToString() const -- format the vector as "(X, Y, Z)" with two decimals
// Retail 00A13DA0. Calls CCharString::FormatDouble 009A08B0, constructor 0099EBF0,
// concatenation 0099F570/0099F600 and destructor 0099EAE0. No shared C3DVector header exists.
// Verified literals: '(' at0122E028, ', ' at012397CC, ')' at0122E020.
#if !defined(_MSC_VER)
#define __fastcall
#endif
class CCharString { public: void* PStringData; CCharString(const char* text, long length=-1); ~CCharString(); };
// 0x009A08B0: Ego_r.pdb + FableWin.pdb ?GFFloatToCharString@@YI?AVCCharString@@NJ@Z (NF94, q11248; was the stand-in static CCharString::FormatDouble)
CCharString __fastcall GFFloatToCharString(double n, long precision);
CCharString __fastcall operator+(const CCharString& left,const CCharString& right);
CCharString __fastcall operator+(const CCharString& left,const char* right);
class C3DVector { public: float X,Y,Z; CCharString ToString() const; };
CCharString C3DVector::ToString() const { return CCharString("(") + GFFloatToCharString(X, 2) + ", " + GFFloatToCharString(Y, 2) + ", " + GFFloatToCharString(Z, 2) + ")"; }