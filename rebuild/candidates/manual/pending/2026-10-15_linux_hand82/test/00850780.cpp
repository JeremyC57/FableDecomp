// Behaviour test for 00850780 GetPointType<T>: type id 0 stores this and returns true, any other id returns false and leaves *out alone.
#include <cstdio>

class CTCCameraPointDefinitionBase
{
public:
    CTCCameraPointDefinitionBase(long id) : m_id(id) {}
    virtual ~CTCCameraPointDefinitionBase() {}
    virtual long GetPointTypeID() const { return m_id; }

    template <class T> bool GetPointType(T** out)
    {
        if (GetPointTypeID() == T::POINT_TYPE_ID)
        {
            *out = static_cast<T*>(this);
            return true;
        }
        return false;
    }
    long m_id;
};

class CTCCameraPointFixedPoint : public CTCCameraPointDefinitionBase
{
public:
    enum { POINT_TYPE_ID = 0 };
    CTCCameraPointFixedPoint(long id) : CTCCameraPointDefinitionBase(id) {}
};

int main()
{
    CTCCameraPointFixedPoint hit(0), miss(0 + 1);
    CTCCameraPointFixedPoint* out = 0;
    if (!hit.GetPointType(&out) || out != &hit) { std::printf("00850780_TEST FAIL\n"); return 1; }
    out = 0;
    if (miss.GetPointType(&out) || out != 0) { std::printf("00850780_TEST FAIL\n"); return 1; }
    std::printf("00850780_TEST PASS\n");
    return 0;
}
