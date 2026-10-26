#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#endif
#include <cstdio>
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
    CIntelligentPointer() {}                        // test only: lets the test build a CTCUnholyPower
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

// ---- test stubs for the retail callees ----
CBaseIntelligentPointer::CBaseIntelligentPointer() : m_pItem(0) {}
CBaseIntelligentPointer::~CBaseIntelligentPointer() {}
CThing* CBaseIntelligentPointer::GetPItem() const { return m_pItem; }
void CBaseIntelligentPointer::SetPItem(CBaseObject* item) { m_pItem = (CThing*)item; }
void CBaseIntelligentPointer::v00() {}
template<class T> void CIntelligentPointer<T>::v00() {}
static CThing* g_pushed[4]; static int g_pushCount;
template<class T> void CRetailVector<T>::push_back(const T& value) { g_pushed[g_pushCount++] = value.GetPItem(); }
static C3DVector g_pos; static const C3DVector* g_start; static const C3DVector* g_end; static float g_radius;
static CThing* g_barrier;
const C3DVector& CThing::GetPosition() const { return g_pos; }
CThing* __fastcall CreatePhysicalBarrierThing(const C3DVector& start, const C3DVector& end, float radius)
{ g_start = &start; g_end = &end; g_radius = radius; return g_barrier; }
void CTCUnholyPower::v00() {}

static void Zero(void* p, unsigned n) { for (unsigned i = 0; i < n; ++i) ((unsigned char*)p)[i] = 0; }

int main()
{
    int ok = 1;
    CTCPhysicsBase physics; physics.Radius = 2.5f;
    CThing owner; Zero(&owner, sizeof(owner)); owner.PhysicsTC = &physics;
    CTCDriverBase driver;
    CThing barrier; Zero(&barrier, sizeof(barrier));
    barrier.m_presentBits[7] = 1u << 13; barrier.PTCDriver = &driver;
    CTCUnholyPower power; power.ParentThing = &owner; power.PObstruction.m_pItem = 0;
    g_barrier = &barrier; g_pushCount = 0;
    power.OnCreate();
    if (g_start != &g_pos || g_end != &g_pos || g_radius != 2.5f) ok = 0;
    if (g_pushCount != 1 || g_pushed[0] != &owner || power.PObstruction.GetPItem() != &barrier) ok = 0;
    // dead barrier: nothing registered
    barrier.m_flags91 = 1; power.PObstruction.m_pItem = 0; g_pushCount = 0;
    power.OnCreate();
    if (g_pushCount != 0 || power.PObstruction.GetPItem() != 0) ok = 0;
    // no barrier made
    g_barrier = 0;
    power.OnCreate();
    if (g_pushCount != 0 || power.PObstruction.GetPItem() != 0) ok = 0;
    if (!ok) { std::printf("007949f0_TEST FAIL\n"); return 1; }
    std::printf("007949f0_TEST PASS\n");
    return 0;
}