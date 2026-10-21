#include <cstdio>
enum EInputType {};
enum EXboxControllerButton {};
class CInputTypeBase {
public:
    virtual ~CInputTypeBase();
    virtual EInputType GetInputType() const = 0;
    virtual bool operator==(const CInputTypeBase& other) const = 0;
};
class CInputTypeXboxPadButtonEvent : public CInputTypeBase {
public:
    EXboxControllerButton Button;             // +0x04
    virtual EInputType GetInputType() const;
    virtual bool operator==(const CInputTypeBase& other) const;
};
bool CInputTypeXboxPadButtonEvent::operator==(const CInputTypeBase& other) const
{
    if (GetInputType() == other.GetInputType())
        return Button == static_cast<const CInputTypeXboxPadButtonEvent&>(other).Button;
    return false;
}

CInputTypeBase::~CInputTypeBase() {}
EInputType CInputTypeXboxPadButtonEvent::GetInputType() const { return (EInputType)7; }
struct COther : CInputTypeBase {
    long Pad;
    EInputType GetInputType() const { return (EInputType)8; }
    bool operator==(const CInputTypeBase&) const { return false; }
};
int main()
{
    CInputTypeXboxPadButtonEvent a, b; COther o; o.Pad = 5;
    a.Button = (EXboxControllerButton)5; b.Button = (EXboxControllerButton)5;
    bool ok = (a == b) && (b == a);
    b.Button = (EXboxControllerButton)6;
    ok = ok && !(a == b);
    ok = ok && !(a == o);
    std::printf(ok ? "0066f210_TEST PASS\n" : "FAIL\n");
    return 0;
}