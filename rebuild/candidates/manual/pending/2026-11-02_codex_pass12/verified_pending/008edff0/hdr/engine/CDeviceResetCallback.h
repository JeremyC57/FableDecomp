#ifdef FABLE_RETAIL_CTHINGMANAGER_BINARY
#ifndef FABLE_ENGINE_CDEVICERESETCALLBACK_H
#define FABLE_ENGINE_CDEVICERESETCALLBACK_H
// Original PDB two pure virtual methods and no data, size4.
class CDeviceResetCallback {
public:
    virtual void OnPreDeviceReset()=0;
    virtual bool OnPostDeviceReset()=0;
};
#endif
#else
#ifndef FABLE_ENGINE_CDEVICERESETCALLBACK_H
#define FABLE_ENGINE_CDEVICERESETCALLBACK_H
// Layout: original PDB classV, no fields, native secondary base at+4.
class CDeviceResetCallback {
public:
    virtual void OnPreDeviceReset();
    virtual bool OnPostDeviceReset();
};
#endif

#endif
