// CPersistTraits<C3DVector>::TransferOut(CPersistContext* context, CCharString* out, C3DVector* value) -- appends the vector as "C3DCoordF(x,y,z)" with six decimals per component
// Leaf: TransferOut
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E7F90, 182 bytes, `ret 0Ch`.
// Callees: 0x0099F100 CCharString::operator+=(const char*), 0x009A08B0 CCharString::FormatDouble(long digits, double)
// (static __fastcall: hidden result in ecx, digits in edx, the double on the stack), 0x0099F0A0
// CCharString::operator+=(const CCharString&), 0x0099EAE0 ~CCharString. Strings "C3DCoordF(" @ 0x0129C244,
// "," @ 0x0122E024, ")" @ 0x0122E020.
// The C3DVector twin of CPersistTraits<C2DVector>::TransferOut 0x009E7F00 (same TU, next function). The context is
// unused, so its argument slot is dead and VC builds each FormatDouble temporary there (`lea ecx,[esp+18h]` after the
// 8-byte double push); `out` stays in esi and `value` in edi across the calls.
class CPersistContext;

class CCharString { public:
    struct SBlock* m_pBlock;
    ~CCharString();                                              // 0x0099EAE0
    const CCharString& operator+=(const char* text);                   // 0x0099F100
    const CCharString& operator+=(const CCharString& other);           // 0x0099F0A0
};
// 0x009A08B0: Ego_r.pdb + FableWin.pdb ?GFFloatToCharString@@YI?AVCCharString@@NJ@Z (NF94, q11248; was the stand-in static CCharString::FormatDouble)
CCharString __fastcall GFFloatToCharString(double n, long precision);

#include "engine/C3DVector.h"

template <class T> struct CPersistTraits {
    T DefaultVal;
    void TransferOut(CPersistContext* context, CCharString* out, T* value);
};

template <>
void CPersistTraits<C3DVector>::TransferOut(CPersistContext* context, CCharString* out, C3DVector* value)
{
    *out += "C3DCoordF(";
    *out += GFFloatToCharString(value->X, 6);
    *out += ",";
    *out += GFFloatToCharString(value->Y, 6);
    *out += ",";
    *out += GFFloatToCharString(value->Z, 6);
    *out += ")";
}