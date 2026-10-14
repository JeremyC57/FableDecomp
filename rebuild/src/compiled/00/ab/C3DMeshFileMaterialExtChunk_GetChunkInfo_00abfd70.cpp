// C3DMeshFileMaterialExtChunk::GetChunkInfo() const -- "Self Illumination: " + GFIntToCharString(SelfIllumination); the number string is built first (right operand), the literal temporary destroyed first
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00ABFD70, length 80 bytes.
// Callees: 0x0099F830 __fastcall GFIntToCharString, 0x0099EBF0 CCharString(const char*, long), 0x0099F570 __fastcall operator+(const CCharString&, const CCharString&), 0x0099EAE0 ~CCharString x2
//
#include "engine/C3DMeshFileMaterialExtChunk.h"   // PDB layout agrees with retail here (SelfIllumination +0x10)
struct CCharString {
    CCharString(const char* text, long length);         // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
    void* m_pStringData;
};
CCharString __fastcall operator+(const CCharString& left, const CCharString& right);   // 0x0099F570
CCharString __fastcall GFIntToCharString(long value);                                  // 0x0099F830
struct C3DMeshFileMaterialExtChunk_Methods : C3DMeshFileMaterialExtChunk {
    CCharString GetChunkInfo() const;
};
CCharString C3DMeshFileMaterialExtChunk_Methods::GetChunkInfo() const
{
    return CCharString("Self Illumination: ", -1) + GFIntToCharString(SelfIllumination);
}