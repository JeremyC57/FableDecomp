// CTCUnholyPower::OnCreate() -- raise a physical barrier on the owner's position, register the owner with the barrier's driver and keep the barrier as PObstruction
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007949F0, length 193 bytes (+ int3 padding). __fastcall, `ret`.
//         CTCUnholyPower vtable 0x0126BFDC slot 4 (the catalog label "CTCDivineWrath::OnCreate" is propagated).
// Callees: CThing::GetPosition @ 0x004C73D0, the barrier factory (__fastcall start, end, radius) @ 0x007FD4B0,
//          CBaseIntelligentPointer ctor/GetPItem/SetPItem/dtor @ 0x00A01B10/0x00A01B50/0x00A01B90/0x00A01C10
//          (CIntelligentPointer<CThing> vtable 0x01238C6C), vector<CIntelligentPointer<CThing>>::push_back @ 0x004AD520.
// Shape: the position is a named reference (GetPosition runs before the radius load) and the physics component a named
//        local (else the radius load swaps ecx/edx); the driver is a named local taken before the temporary, read by the
//        inline cached-component accessor (interface 0xED = bitset word 7 bit 13, PTCDriver at retail +0x5C) that leaves
//        its result unset when the bit is clear; both intelligent-pointer temporaries share the accessor's frame word.
// Layout: retail CThing PTCDriver +0x5C, PhysicsTC +0x60 (PDB +0x6C/+0x70); retail CTCUnholyPower::PObstruction +0xC8
//         (PDB +0xD4: two 12-byte STLport containers and the 4-byte-short timer above it).
class CTCBase;
class CBaseObject;
class CThing;

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                // +0x04
    CBaseIntelligentPointer();                      // 0x00A01B10
    ~CBaseIntelligentPointer();                     // 0x00A01C10
    CThing* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);               // 0x00A01B90
};

template<class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:   // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        CThing* item = other.GetPItem();
        SetPItem((CBaseObject*)item);
        return *this;
    }
};

template <class T> struct SRetailAllocProxy {       // STLport _STLP_alloc_proxy: the end-of-storage pointer
    T* _M_data;
};
template <class T> struct CRetailVector {           // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    SRetailAllocProxy<T> _M_end_of_storage;
    void push_back(const T& value);                 // 0x004AD520
};

class C3DVector { public: float X, Y, Z; };

class CTCDriverBase { public:
    unsigned char _pad00[0x2c];
    CRetailVector< CIntelligentPointer<CThing> > Passengers;   // +0x2C (descriptive: the things riding/blocked by it)
};

class CTCPhysicsBase { public:
    unsigned char _pad00[0x34];
    float Radius;                                   // +0x34
};

class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                 // +0x20 std::bitset of interface types
    unsigned char _pad44[0x5c - 0x44];
    CTCDriverBase* PTCDriver;                       // +0x5C
    CTCPhysicsBase* PhysicsTC;                      // +0x60
    unsigned char _pad64[0x91 - 0x64];
    unsigned char m_flags91;                        // +0x91 bit 0: dead
    const C3DVector& GetPosition() const;           // 0x004C73D0
    CTCDriverBase* GetDriver() const
    {
        CTCDriverBase* driver;
        if (m_presentBits[0xed >> 5] & (1UL << (0xed & 31)))
            driver = PTCDriver;
        return driver;
    }
};

CThing* __fastcall CreatePhysicalBarrierThing(const C3DVector& start, const C3DVector& end, float radius);   // 0x007FD4B0

class CTCUnholyPower { public:
    virtual void v00();
    CThing* ParentThing;                            // +0x04
    unsigned char _pad08[0xc8 - 0x08];
    CIntelligentPointer<CThing> PObstruction;       // +0xC8
    void OnCreate();
};

void CTCUnholyPower::OnCreate()
{
    const C3DVector& position = ParentThing->GetPosition();
    CTCPhysicsBase* physics = ParentThing->PhysicsTC;
    CThing* barrier = CreatePhysicalBarrierThing(position, position, physics->Radius);
    if (barrier && !(barrier->m_flags91 & 1)) {
        CTCDriverBase* driver = barrier->GetDriver();
        driver->Passengers.push_back(CIntelligentPointer<CThing>(ParentThing));
        PObstruction = CIntelligentPointer<CThing>(barrier);
    }
}