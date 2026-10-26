#include <cstdio>
enum EInputType {};
enum EMouseButtonControl {};
class C2DVector { public: float X; float Y; };
class CInputTypeBase {
public:
    virtual ~CInputTypeBase();
    virtual EInputType GetInputType() const = 0;
    virtual bool operator==(const CInputTypeBase& other) const = 0;
};
class CInputTypeMouseButtonEvent : public CInputTypeBase {
public:
    EMouseButtonControl Button;               // +0x04
    C2DVector ControlDirection;                          // +0x08
    virtual EInputType GetInputType() const;
    virtual bool operator==(const CInputTypeBase& other) const;
};
bool CInputTypeMouseButtonEvent::operator==(const CInputTypeBase& other) const
{
    if (GetInputType() == other.GetInputType())
        return Button == static_cast<const CInputTypeMouseButtonEvent&>(other).Button;
    return false;
}

CInputTypeBase::~CInputTypeBase() {}
EInputType CInputTypeMouseButtonEvent::GetInputType() const { return (EInputType)7; }
struct COther : CInputTypeBase {
    long Pad;
    EInputType GetInputType() const { return (EInputType)8; }
    bool operator==(const CInputTypeBase&) const { return false; }
};
int main()
{
    CInputTypeMouseButtonEvent a, b; COther o; o.Pad = 5;
    a.Button = (EMouseButtonControl)5; b.Button = (EMouseButtonControl)5;
    bool ok = (a == b) && (b == a);
    b.Button = (EMouseButtonControl)6;
    ok = ok && !(a == b);
    ok = ok && !(a == o);
    std::printf(ok ? "0066eea0_TEST PASS\n" : "FAIL\n");
    return 0;
}