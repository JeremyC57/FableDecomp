// Leaf: ??YCCharString@@QAEABV0@ABV0@@Z
// NF373 (T3 nf305 port): the former extern "C" __fastcall stub CCharString_operator_plusequals(self, edx unused, rhs) is the
// member const CCharString& CCharString::operator+=(const CCharString&) (Ego_r.pdb / FableWin.pdb ??YCCharString@@QAEABV0@ABV0@@Z;
// callers already bind that spelling, e.g. 0x0099F690), so it reaches the protected MakeOwnCopyOfData (IAE, 0x0099EAF0).
#pragma pack(push, 1)
class CCharStringData
{ public:
    char* text;
    unsigned char pad[9];
    unsigned int refCount;

    void AppendData(const char* src, int len);
};
#pragma pack(pop)

extern char g_emptyCharStringBuf;

class CCharString
{ public:
    CCharStringData* data;

protected:
    void MakeOwnCopyOfData();
public:
    const CCharString& operator+=(const CCharString& rhs);
};

const CCharString& CCharString::operator+=(const CCharString& rhs)
{
    if (data != 0)
    {
        MakeOwnCopyOfData();

        CCharStringData* rhsData = rhs.data;
        char* rhsText = &g_emptyCharStringBuf;
        if (rhsData != 0)
        {
            rhsText = rhsData->text;
        }

        CCharStringData* selfData = data;

        int len = 0;
        if (rhsText[0] != 0)
        {
            do
            {
                ++len;
            } while (rhsText[len] != 0);
        }

        selfData->AppendData(rhsText, len);

        return *this;
    }

    if (&rhs != this)
    {
        CCharStringData* rhsData = rhs.data;
        if (rhsData != 0)
        {
            data = rhsData;
            ++rhsData->refCount;
        }
    }

    return *this;
}