// Behaviour test for 00c0c910 CString::MakeUpper: a shared buffer is copied before writing, then the characters are upper-cased in place.
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

struct CStringData { long nRefs; long nDataLength; long nAllocLength; char* data() { return (char*)(this + 1); } };

class CString
{
public:
    char* m_pchData;
    CStringData* GetData() const { return ((CStringData*)m_pchData) - 1; }
    void CopyBeforeWrite()
    {
        if (GetData()->nRefs > 1)
        {
            CStringData* old = GetData();
            CStringData* fresh = (CStringData*)malloc(sizeof(CStringData) + old->nAllocLength + 1);
            fresh->nRefs = 1; fresh->nDataLength = old->nDataLength; fresh->nAllocLength = old->nAllocLength;
            memcpy(fresh->data(), m_pchData, old->nDataLength + 1);
            --old->nRefs;
            m_pchData = fresh->data();
        }
    }
    void MakeUpper()
    {
        CopyBeforeWrite();
        for (char* p = m_pchData; *p; ++p) *p = (char)toupper((unsigned char)*p);
    }
};

int main()
{
    CStringData* d = (CStringData*)malloc(sizeof(CStringData) + 16);
    d->nRefs = 2; d->nDataLength = 5; d->nAllocLength = 15;
    strcpy(d->data(), "bank1");
    CString a; a.m_pchData = d->data();
    CString b; b.m_pchData = d->data();
    a.MakeUpper();
    if (strcmp(a.m_pchData, "BANK1") != 0 || strcmp(b.m_pchData, "bank1") != 0 || d->nRefs != 1 || a.m_pchData == b.m_pchData)
    { printf("FAIL\n"); return 1; }
    b.MakeUpper();
    if (strcmp(b.m_pchData, "BANK1") != 0 || b.m_pchData != d->data()) { printf("FAIL\n"); return 1; }
    printf("OK_00c0c910\n");
    return 0;
}
