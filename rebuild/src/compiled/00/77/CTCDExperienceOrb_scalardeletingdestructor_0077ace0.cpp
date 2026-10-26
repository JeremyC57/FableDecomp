// CTCDExperienceOrb::`scalar deleting destructor'(unsigned int) -- own vtable, release the experience def (twice: the body's
// reset and the member destructor), the CTCBase destructor, then the flag-tested operator delete
// Leaf: ??_GCTCDExperienceOrb@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0077ACE0, length 93 bytes. CTCDExperienceOrb vtable 0x0126ABFC.
// Callees: the def's virtual slot 1 (destroy at a zero count), CTCBase::~CTCBase @ 0x00686830, operator delete @ 0x00BFE9BC.
// Shape: the destructor body assigns `PDef = 0` (the CDefPointer release with its inner zero store plus the assignment's
// unconditional zero store), then the PDef member destructor runs the same release again on the re-read member. The
// release, the assignment, the member destructor and the virtual destructor are `__forceinline` (else the deleting
// destructor collapses to `call ??1 ; delete`); a `new` factory pulls the vtable and the ??_G into the TU.
class CThing;

class CDefPointeeBase { public:
    virtual void v00();
    virtual void Destroy();                 // slot 1
    long RefCount;                          // +0x04
};

struct CDefPointer {                        // CDefPointer<CExperienceDef const>
    CDefPointeeBase* Object;
    CDefPointer() : Object(0) { }
    __forceinline void Release()
    {
        CDefPointeeBase* object = Object;
        if (object) {
            if (--object->RefCount == 0)
                object->Destroy();
            Object = 0;
        }
    }
    __forceinline CDefPointer& operator=(int)
    {
        Release();
        Object = 0;
        return *this;
    }
    __forceinline ~CDefPointer() { Release(); }
};

class CTCBase { public:
    CTCBase(CThing* parent);                // 0x00686800
    virtual ~CTCBase();                     // 0x00686830
    CThing* ParentThing;                    // +0x04
    bool ToBeRemoved;                       // +0x08
    bool IsInGlobalUpdateTCs;               // +0x09
};

class CTCDExperienceOrb : public CTCBase { public:
    CDefPointer PDef;                       // +0x0c
    long ExperiencePoints;                  // +0x10
    long CombinationImmunityTimer;          // +0x14
    long LifeTimer;                         // +0x18
    bool DecayOverTime;                     // +0x1c
    bool PickedUp;                          // +0x1d
    CTCDExperienceOrb(CThing* parent) : CTCBase(parent) { }
    __forceinline virtual ~CTCDExperienceOrb() { PDef = 0; }
};

CTCDExperienceOrb* CreateExperienceOrb(CThing* parent)
{
    return new CTCDExperienceOrb(parent);
}