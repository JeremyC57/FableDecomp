// CWideString::GetDataSize() -- the character count (Finish - Start, halved for wchar_t) times
// sizeof(wchar_t): the byte size of the text. An unassigned string reports 0.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0099B240, length 21 bytes. No callees.
// Manifest label (CCharString::GetLength) corrected: the halved pointer difference is the wide class and the
// trailing `add eax,eax` doubles it (the landed CWideString::GetLength @ 0x0099B220 is the undoubled body).
// CWideString model: a 4-byte handle on a shared 16-byte block whose first three dwords are the retail
// (STLport-era) basic_string<wchar_t> {Start, Finish, EndOfStorage} and the fourth is NoRefs.
// (The Ego_r PDB shows a 28-byte VC7 basic_string there; retail is the 12-byte form.)
#include <stddef.h>
struct CWideStringData {
    wchar_t* Start;                             // +0x00
    wchar_t* Finish;                            // +0x04
    wchar_t* EndOfStorage;                      // +0x08
    long     NoRefs;                            // +0x0c
};
class CWideString {
public:
    CWideStringData* PStringData;               // +0x00
    long GetLength() const { if (PStringData == 0) return 0; return PStringData->Finish - PStringData->Start; }
    long GetDataSize() const;
};

long CWideString::GetDataSize() const
{
    return GetLength() * sizeof(wchar_t);
}