// CThingPhysical::IsThingAbleToComeBetweenCameraAndObservedThing() -- the explicit CanComeBetweenCameraAndObservedThing setting, or the def's flag when it is -1 (unset)
// Retail: Fable.exe 0x006A5AB0, 60 bytes; the def release calls the def's vtable slot 1 (+4).
// Retail offsets (CThing 0x10 and CThingPhysical 0x18 below the PDB; the generated header keeps the PDB
// offsets, so the classes are modelled locally): PBaseDef +0x70 (PDB +0x80), the setting +0xb8 (PDB +0xd0).
// The def's flag is bit 2 of the byte at +0x42 of the CThingPhysicalDef.
// Shape: the def is copied into a by-value CDefPointer temporary (null-tested AddRef), read through
// `->` and released (no null test in the release) -- the flag rides in bl across the release; the
// explicit setting compares through a full register (`return value == 1;`, xor/sete/mov al,cl).
struct CThingPhysicalDef {
    virtual void v000();
    virtual void Destroy();                              // slot 1 (+4)
    long          m_refCount;                            // +0x04
    unsigned char _pad08[0x3a];
    unsigned char _flags42_0 : 2;
    unsigned char AbleToComeBetweenCameraAndObservedThing : 1;   // +0x42 bit 2
};

template <class T> struct CDefPointer {
    T* m_pDef;
    CDefPointer(const CDefPointer& other) : m_pDef(other.m_pDef) { if (m_pDef) ++m_pDef->m_refCount; }
    ~CDefPointer() { if (--m_pDef->m_refCount == 0) m_pDef->Destroy(); }
    T* operator->() const { return m_pDef; }
};

struct CThingPhysical {
    unsigned char                   _pad00[0x70];
    CDefPointer<CThingPhysicalDef>  PBaseDef;            // +0x70
    unsigned char                   _pad74[0x44];
    long                            CanComeBetweenCameraAndObservedThing;   // +0xb8
    CDefPointer<CThingPhysicalDef> GetDef() const { return PBaseDef; }
    bool IsThingAbleToComeBetweenCameraAndObservedThing();
};

bool CThingPhysical::IsThingAbleToComeBetweenCameraAndObservedThing()
{
    long setting = CanComeBetweenCameraAndObservedThing;
    if (setting == -1)
        return GetDef()->AbleToComeBetweenCameraAndObservedThing;
    return setting == 1;
}