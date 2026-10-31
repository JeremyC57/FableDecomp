// Behaviour test for CThing::IsEntitySerialisable (0x004C7870): no driver -> false, else the driver's answer.
#include <cstdio>
class CTCDriverBase { public:
    virtual ~CTCDriverBase() {}
    virtual bool IsComponentSerialisable() const { return false; }
};
class CTCDCameraPoint : public CTCDriverBase { public:
    virtual bool IsComponentSerialisable() const { return true; }
};
class CThing { public:
    CTCDriverBase* PTCDriver;
    bool IsEntitySerialisable() const
    {
        bool result = false;
        CTCDriverBase* driver = PTCDriver;
        if (driver)
            result = driver->IsComponentSerialisable();
        return result;
    }
};
int main()
{
    CTCDriverBase base; CTCDCameraPoint cam;
    CThing t;
    t.PTCDriver = 0;
    if (t.IsEntitySerialisable()) { std::printf("004c7870_TEST FAIL\n"); return 1; }
    t.PTCDriver = &base;
    if (t.IsEntitySerialisable()) { std::printf("004c7870_TEST FAIL\n"); return 1; }
    t.PTCDriver = &cam;
    if (!t.IsEntitySerialisable()) { std::printf("004c7870_TEST FAIL\n"); return 1; }
    std::printf("004c7870_TEST PASS\n");
    return 0;
}