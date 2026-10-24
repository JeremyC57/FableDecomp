// Behaviour test for CInputTypeDataRecord::SetValueB4 (0x0066F9E0): the setter writes the
// +0xB4 dword and nothing else.
#include <stdio.h>
#include <stddef.h>

class CInputTypeDataRecord
{
public:
    void SetValueB4(unsigned long value);

    unsigned long m_header[0xB4 / 4];
    unsigned long m_valueB4;
    unsigned long m_after;
};

void CInputTypeDataRecord::SetValueB4(unsigned long value)
{
    m_valueB4 = value;
}

int main()
{
    CInputTypeDataRecord r;
    int i;
    for (i = 0; i < 0xB4 / 4; ++i) r.m_header[i] = 0x11111111u * (unsigned long)(i & 7);
    r.m_valueB4 = 0;
    r.m_after = 0xCAFEBABEu;
    if (offsetof(CInputTypeDataRecord, m_valueB4) != 0xB4) { printf("FAIL field-0xB4 layout\n"); return 1; }
    r.SetValueB4(0xDEADBEEFu);
    if (r.m_valueB4 != 0xDEADBEEFu || r.m_after != 0xCAFEBABEu) { printf("FAIL field-0xB4 setter\n"); return 1; }
    for (i = 0; i < 0xB4 / 4; ++i)
        if (r.m_header[i] != 0x11111111u * (unsigned long)(i & 7)) { printf("FAIL field-0xB4 setter\n"); return 1; }
    r.SetValueB4(7);
    if (r.m_valueB4 != 7) { printf("FAIL field-0xB4 setter\n"); return 1; }
    printf("PASS field-0xB4 setter 0x0066F9E0\n");
    return 0;
}
