#ifndef FABLE_ENGINE_CDEVICERESETCALLBACK_H
#define FABLE_ENGINE_CDEVICERESETCALLBACK_H
// Original PDB two pure virtual methods and no data, size4.
class CDeviceResetCallback {
public:
    virtual void OnPreDeviceReset()=0;
    virtual bool OnPostDeviceReset()=0;
};
#endif
