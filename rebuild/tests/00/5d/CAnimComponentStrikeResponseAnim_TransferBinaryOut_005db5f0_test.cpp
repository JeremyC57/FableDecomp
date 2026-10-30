// Behaviour test for CAnimComponentStrikeResponseAnim::TransferBinaryOut (retail 0x005DB5F0): writes the name length
// (0 for a null string) and the string only when the length is non-zero.
#include <cstdio>
class CCharStringData { public: const char* PData; unsigned int Length; };
class CCharString { public:
    CCharStringData* PStringData;
    unsigned int GetLength() const { if (PStringData == 0) return 0; return PStringData->Length; }
};
class CDataOutputStream { public:
    unsigned int lengths[4]; int nLengths; int nStrings;
    void WriteULONG(unsigned int v) { lengths[nLengths++] = v; }
    void WriteString(const CCharString&) { ++nStrings; }
};
class CAnimComponentStrikeResponseAnim { public:
    CCharString ResponseAnimName;
    void TransferBinaryOut(CDataOutputStream& out) const
    {
        int length = (int)ResponseAnimName.GetLength();
        if (length < 0) length = 0;
        out.WriteULONG(length);
        if ((unsigned int)length > 0) out.WriteString(ResponseAnimName);
    }
};
int main()
{
    CDataOutputStream out = { {0,0,0,0}, 0, 0 };
    CAnimComponentStrikeResponseAnim a; a.ResponseAnimName.PStringData = 0;
    a.TransferBinaryOut(out);
    CCharStringData d = { "Strike", 6 }; a.ResponseAnimName.PStringData = &d;
    a.TransferBinaryOut(out);
    CCharStringData e = { "", 0 }; a.ResponseAnimName.PStringData = &e;
    a.TransferBinaryOut(out);
    if (out.nLengths != 3 || out.lengths[0] != 0 || out.lengths[1] != 6 || out.lengths[2] != 0 || out.nStrings != 1) {
        std::printf("005db5f0_TEST FAIL\n"); return 1;
    }
    std::printf("005db5f0_TEST PASS\n");
    return 0;
}