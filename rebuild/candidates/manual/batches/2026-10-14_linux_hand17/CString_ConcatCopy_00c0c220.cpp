// CString::ConcatCopy(int nSrc1Len, LPCTSTR lpszSrc1Data, int nSrc2Len, LPCTSTR lpszSrc2Data) -- the MFC
// CString concatenation worker: allocate a buffer for the combined length and copy both pieces into it
// (nothing happens for a combined length of 0). This is the MFC/ATL CString that the 0x00C0xxxx library
// module carries (CStringData {nRefs, nDataLength, nAllocLength} precedes the text; AllocBuffer @ 0x00C0BDA0
// is the MFC one with the ASSERT hook @ 0x0143BB2C), not CCharString -- manifest label corrected.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C0C220, length 79 bytes.
// Callees: CString::AllocBuffer @ 0x00C0BDA0. memcpy is the intrinsic (rep movsd ; rep movsb).
#include <string.h>

class CString {
public:
    char* m_pchData;                            // +0x00 (points past the CStringData header)
    void AllocBuffer(int nLen);                 // 0x00C0BDA0
    void ConcatCopy(int nSrc1Len, const char* lpszSrc1Data, int nSrc2Len, const char* lpszSrc2Data);
};

void CString::ConcatCopy(int nSrc1Len, const char* lpszSrc1Data, int nSrc2Len, const char* lpszSrc2Data)
{
    int nNewLen = nSrc1Len + nSrc2Len;
    if (nNewLen != 0) {
        AllocBuffer(nNewLen);
        memcpy(m_pchData, lpszSrc1Data, nSrc1Len * sizeof(char));
        memcpy(m_pchData + nSrc1Len, lpszSrc2Data, nSrc2Len * sizeof(char));
    }
}
