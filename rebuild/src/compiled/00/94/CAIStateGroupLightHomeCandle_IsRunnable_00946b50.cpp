// CAIStateGroup_LightHomeCandle::IsRunnable() -- refresh the candle to tend from the (alive) home building unless
// the building's material is flagged not-for-candles (bit 5 of the def's +0x3c), then runnable while that candle is
// alive and needs changing.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00946B50, length 252 bytes (+ int3 padding). Vtable slot 11 of
// CAIStateGroup_LightHomeCandle (0x0128BDBC).
// Callees: own virtual slots 14 (+0x38, GetBuilding @ 0x00946650) and 16 (+0x40, IsWorkCandle @ 0x0090002C),
// CThingBuilding::PeekPMaterial @ 0x0082D2D0 (CDefPointer by hidden pointer), the def's virtual slot 1 (destroy on the
// last release), FindAndValidateTask @ 0x00946AE0, CBaseIntelligentPointer ctor/SetPItem/GetPItem/dtor @ 0x00A01B10 /
// 0x00A01B90 / 0x00A01B50 / 0x00A01C10, IsCandleInNeedOfChanging @ 0x00946730.
// Shape: the CDefPointer temporary is the landed CAIStateGroup_Clean model (inline release: dec the count at +4,
// destroy at zero; the null test folds into the `if (mat.Object && ...)` test); the candle is reassigned through a
// CIntelligentPointer<CThing> temporary (vtable 0x01238C6C).
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
class CTCBase;
class CWorld;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with CVectorMap::LowerBound (this TU's copy) -- the inlined
// GetInterface<T>() shape of the landed AI bodies (hit path: lookup result; miss path: the never-written slot).
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020 / 0x004365B0 (per-TU copies)
};

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x60 - 0x4c];
    void* m_pSub60;                             // +0x60 (typed per use below)
    unsigned char _pad2[0x91 - 0x64];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
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

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // CIntelligentPointer<CThing>
public:
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }

public:
    T* GetPItem() const { return reinterpret_cast<T*>(CBaseIntelligentPointer::GetPItem()); }
public:
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem(reinterpret_cast<CBaseObject*>(item)); }
};

// IS_THING_ALIVE(ptr): the pointer is set and the thing's dead bit (+0x91 bit 0) is clear (inlined).
inline bool IS_THING_ALIVE(const CIntelligentPointer<CThing>& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

long __fastcall GFGetCurrentFrame();            // 0x0049D870
unsigned long __fastcall GFROR13(unsigned long value);   // 0x00497890

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();               // slot 0
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08();
    virtual void Cleanup();                     // slot 9  (+0x24)
    virtual void OnReplace();                   // slot 10 (+0x28)
    virtual bool IsRunnable();                  // slot 11 (+0x2c)
    virtual void Update();                      // slot 12 (+0x30)
    virtual void v13();
    virtual long GetTimeToInitiate();           // slot 14 (+0x38)
    virtual void v15(); virtual void v16(); virtual void v17();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
    unsigned long* GetPWorldSeed();             // 0x008FCFD0
    // the landed GetRandomObject random step: seed = ror13(seed*0x24A1 + 0x24DF), r = seed % range
    unsigned long Random(unsigned long range)
    {
        unsigned long* seed = GetPWorldSeed();
        unsigned long next = *seed * 0x24A1U + 0x24DFU;
        *seed = next;
        unsigned long rotated = GFROR13(next);
        *seed = rotated;
        return rotated % range;
    }
};

// vtable padding helpers: V4/V16 declare 4/16 placeholder virtual slots
#define V4(n) virtual void n##0(); virtual void n##1(); virtual void n##2(); virtual void n##3();
#define V16(n) V4(n##a) V4(n##b) V4(n##c) V4(n##d)
extern "C" double __cdecl sqrt(double);
#pragma intrinsic(sqrt)
struct CRefCounted {                            // reference count base at +4 of a def
    long Count;
    void DecRef() { --Count; }
    long GetRefCount() const { return Count; }
};
class CMaterialDef { public:
    virtual void v00();
    virtual void Destroy();                     // slot 1 (+0x04)
    CRefCounted Ref;                            // +0x04
    unsigned char _pad08[0x3c - 0x08];
    unsigned long Flags;                        // +0x3c (bit 5: no candle work)
    __forceinline void ReleaseTemporary() { Ref.DecRef(); if (Ref.GetRefCount() == 0) Destroy(); }
};
template <class T>
class CDefPointer { public:
    T* Object;
    __forceinline ~CDefPointer() { if (Object) Object->ReleaseTemporary(); }
    T* operator->() const { return Object; }
};
class CThingBuilding : public CThing { public:
    CDefPointer<CMaterialDef> PeekPMaterial();  // 0x0082D2D0
};

class CAIStateGroup_LightHomeCandle : public CAIStateGroupBase { public:
    CIntelligentPointer<CThing> PCandle;           // +0x14
    virtual bool IsRunnable();
    CThing* FindAndValidateTask(CThing* building);      // 0x00946AE0
    bool IsCandleInNeedOfChanging(CThing* candle);      // 0x00946730
};
struct CAIStateGroup_LightHomeCandleV {         // the class's own virtual slots past the base
    V4(a) V4(b) V4(c)
    virtual void v12(); virtual void v13();
    virtual CThingBuilding* GetBuilding();      // slot 14 (+0x38)
    virtual bool ShouldBeLit();                 // slot 15 (+0x3c)
    virtual bool IsWorkCandle();                // slot 16 (+0x40)
};

bool CAIStateGroup_LightHomeCandle::IsRunnable()
{
    CAIStateGroup_LightHomeCandleV* self = (CAIStateGroup_LightHomeCandleV*)this;
    CThingBuilding* building = self->GetBuilding();
    if (building && !(building->m_flags91 & 1)) {
        if (!self->IsWorkCandle()) {
            CDefPointer<CMaterialDef> material = building->PeekPMaterial();
            if (material.Object && (material->Flags & 0x20))
                return false;
        }
        PCandle = CIntelligentPointer<CThing>(FindAndValidateTask(building));
    }
    return IS_THING_ALIVE(PCandle) && IsCandleInNeedOfChanging(PCandle.GetPItem());
}