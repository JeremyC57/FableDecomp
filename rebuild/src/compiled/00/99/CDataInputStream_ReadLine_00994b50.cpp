// CDataInputStream::ReadLine() -- read until CR/LF/NUL; CR consumes following LF or seeks back one character
// Leaf: ?ReadLine@CDataInputStream@@QAE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00994B50, 289 bytes, thiscall, hidden result pointer (`ret 4`).
// Callees: CCharString() 0x0099E4B0, CCharString::operator+=(char) 0x0099F180, CCharString(const CCharString&) 0x0099EC30 (the
//   by-value return: VC7.1 has no NRVO, the result is copy-constructed from the local), ~CCharString 0x0099EAE0,
//   CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0 (the inline Read's slow path), the virtual SetPosition / GetPosition.
// Naming (NF129, 2026-11-20, q6185): Ego_r.pdb 0x00AFCC20 + FableWin.pdb 0x03081A20 ?ReadLine@CDataInputStream@@QAE?AVCCharString@@XZ,
//   the spelling the landed caller CBankFileManager::OpenIniFile 0x009A8170 binds (0x009A825C). The row's CStringReadResult adapter
//   (derived from CCharString for the copy constructor and operator+=(char)) and the renamed CDataInputStream layout include are replaced by
//   engine/CDataInputStream.h (FABLE_RETAIL_CDATAINPUTSTREAM_TEXT) and fable_string.h (FABLE_RETAIL_CCHARSTRING_APPEND_CHAR); each
//   character is the header's inline ReadType<char>() (FableWin ??$ReadType@D@CDataInputStream@@QAEDXZ, wrapped by ReadCBYTE).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_APPEND_CHAR
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CDATAINPUTSTREAM_TEXT
#include "fable_string.h"
#include "engine/CDataInputStream.h"

CCharString CDataInputStream::ReadLine()
{
    CCharString result;
    while (StreamPos != (unsigned long)StreamSize) {
        char c = ReadType<char>();
        if (c == '\r') {
            if (StreamPos != (unsigned long)StreamSize) {
                char next = ReadType<char>();
                if (next != '\n')
                    SetPosition(GetPosition() - 1);
            }
            break;
        }
        if (c == '\n' || c == 0)
            break;
        result += c;
    }
    return result;
}
