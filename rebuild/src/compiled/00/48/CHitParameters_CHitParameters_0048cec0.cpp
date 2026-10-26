// CHitParameters::~CHitParameters() -- destroy the intelligent pointer at +0x38 (0x00A01C10), release the counted pointer at +0x30, destroy the intelligent pointer at +0x28, then the base destructor (0x00A0D3D0, a tail jump)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0048CEC0, length 51 bytes.
// The base label "~CAInputProcess" is propagated. Shape: no vtable store (the class is not polymorphic here), members last-to-first.
class CIVCountedPointeeBase { public:
    virtual void Destroy(int flags);               // slot 0
    virtual void Delete();                         // slot 1
    unsigned long IVRefCount;                      // +4
};
struct CIVCountedPointer {
    CIVCountedPointeeBase* Object;
    void Release()
    {
        CIVCountedPointeeBase* old = Object;
        if (old) {
            if (--old->IVRefCount == 0)
                old->Delete();
            Object = 0;
        }
    }
    ~CIVCountedPointer() { Release(); }
};
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
struct CHitParametersBase_00a0d3d0 {               // 0x28 bytes
    unsigned char _body[0x28];
    ~CHitParametersBase_00a0d3d0();                // 0x00A0D3D0
};
class CHitParameters : public CHitParametersBase_00a0d3d0 { public:
    CBaseIntelligentPointer m_pointer28;           // +0x28
    CIVCountedPointer       m_counted30;           // +0x30
    unsigned char           _pad34[4];
    CBaseIntelligentPointer m_pointer38;           // +0x38
    ~CHitParameters();
};

CHitParameters::~CHitParameters()
{
}