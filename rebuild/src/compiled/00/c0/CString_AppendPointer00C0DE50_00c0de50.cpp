// CString::AppendPointer_00C0DE50(value) -- format the scalar in a local buffer and append it to the middleware string.
// Leaf: ?AppendPointer_00C0DE50@CString@@QAEXPBX@Z
// Flags: /GS
// Identity: descriptive; own sprintf import01440128 format012B21DC pointer; actual64-byte /GS stack buffer, cookie0139C8A8 and checker00BFE9F9; own Append(constchar*)00C0DD40 proven by SafeStrlen00C0BCC0/ConcatInPlace00C0C270 and already-landed CString::Append(double)00C0DD60. Original libpk/MFC-style CString identity descriptive from established source family, absent PDB/RTTI. Target method name address-suffixed; signed32-bit argument projection for the two decimal bodies does not claim distinct int/long source overload identity.
// Layout: original libpk CString four-byte string-data pointer (no shared header). No member fields/sizeof used by target. Existing CEngine::ClipBox labels are propagation errors. Native C++ char[64] and real CRT sprintf produce the compiler-generated security cookie.
#ifdef _MSC_VER
extern "C" __declspec(dllimport) int __cdecl sprintf(char*,const char*,...);
#else
#include <stdio.h>
#endif
class CString{public:char* m_pchData;void Append(const char*);void AppendPointer_00C0DE50(const void*);};void CString::AppendPointer_00C0DE50(const void* value){char buffer[64];sprintf(buffer,"%p",value);Append(buffer);}