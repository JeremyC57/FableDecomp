#include <cstdio>
#include <cstring>
// Behaviour test for CMeshDataBank::GetEnumerationType (0x00A23D80).
struct Data { const char* PData; unsigned int StringLength; };
class CCharString {
public:
    Data* PStringData;
    bool IsEmpty() const { return PStringData == 0 || PStringData->StringLength == 0; }
};
class CBankFile {
public:
    virtual ~CBankFile() {}
    virtual CCharString GetEnumerationType(unsigned int) const { CCharString s; s.PStringData = 0; return s; }
};
class CMeshDataBank : public CBankFile {
public:
    CCharString AnimationEnumType;
    virtual CCharString GetEnumerationType(unsigned int type) const
    {
        if (!AnimationEnumType.IsEmpty()) {
            if (type == 6 || type == 10 || type == 7 || type == 8 || type == 9)
                return AnimationEnumType;
        }
        return CBankFile::GetEnumerationType(type);
    }
};
int main()
{
    Data d = { "EAnim", 5 };
    CMeshDataBank b; b.AnimationEnumType.PStringData = &d;
    bool ok = true;
    for (unsigned int t = 0; t < 12; ++t) {
        bool anim = t >= 6 && t <= 10;
        ok = ok && ((b.GetEnumerationType(t).PStringData == &d) == anim);
    }
    Data e = { "", 0 }; b.AnimationEnumType.PStringData = &e;
    ok = ok && b.GetEnumerationType(7).PStringData == 0;
    b.AnimationEnumType.PStringData = 0;
    ok = ok && b.GetEnumerationType(8).PStringData == 0;
    if (!ok) { std::printf("00a23d80_TEST FAIL\n"); return 1; }
    std::printf("00a23d80_TEST PASS\n");
    return 0;
}