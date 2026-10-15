// Behaviour test for 005ecfd0 GetPointType<T>: type id 6 stores this and returns true, any other id returns false and leaves *out alone.
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
    enum { POINT_TYPE_ID = 6 };
    CTCCameraPointFixedPoint(long id) : CTCCameraPointDefinitionBase(id) {}
};

int main()
{
    CTCCameraPointFixedPoint hit(6), miss(6 + 1);
    CTCCameraPointFixedPoint* out = 0;
    if (!hit.GetPointType(&out) || out != &hit) { std::printf("005ecfd0_TEST FAIL\n"); return 1; }
    out = 0;
    if (miss.GetPointType(&out) || out != 0) { std::printf("005ecfd0_TEST FAIL\n"); return 1; }
    std::printf("005ecfd0_TEST PASS\n");
    return 0;
}