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

namespace
{
const char* const kActionNameLiteral =
    reinterpret_cast<const char*>(0x0123cb08);
}

CCharString CTCTrespassable::GetClassTypeName()
{
    return CCharString(kActionNameLiteral, -1);
}