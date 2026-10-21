// behaviour test for 0x0066F510
#include <stdio.h>
class CInputTypeBase {
public:
    virtual ~CInputTypeBase() {}
    virtual long GetType() const = 0;                    // slot 1 (+4)
    virtual bool operator==(const CInputTypeBase& other) const = 0;   // slot 2 (+8)
};
class CInputTypeMouseWheelMovementUpEvent : public CInputTypeBase {
public:
    long Id;
    virtual long GetType() const { return Id; }
    virtual bool operator==(const CInputTypeBase& other) const;
};
bool CInputTypeMouseWheelMovementUpEvent::operator==(const CInputTypeBase& other) const
{
    return GetType() == other.GetType();
}

int main()
{
    CInputTypeMouseWheelMovementUpEvent a, b, c; a.Id = 3; b.Id = 3; c.Id = 4;
    const CInputTypeBase& ra = a;
    int ok = (ra == b) && !(ra == c) && (c == c);
    printf(ok ? "0066f510_TEST PASS\n" : "0066f510_TEST FAIL\n");
    return ok ? 0 : 1;
}