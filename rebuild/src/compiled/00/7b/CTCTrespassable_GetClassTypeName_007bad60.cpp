class CCharString
{
public:
    CCharString(const char* pText, long length);
};

class CTCTrespassable
{
public:
    CCharString GetClassTypeName();
};


CCharString CTCTrespassable::GetClassTypeName()
{
    return CCharString("CTCTrespassable", -1);
}