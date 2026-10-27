// CTCVillage::IsEatTime() const -- whether the time of day lies in one of the village def's eat windows
// Leaf: ?IsEatTime@CTCVillage@@QBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00658BB9, 81 bytes, thiscall, plain `ret`.
// Identity (proven): Ego_r.pdb ?IsEatTime@CTCVillage@@QBE_NXZ; it reads CVillageDef::EatStartTimes +0x84 and EatDuration
//   +0x90 (Ego_r.pdb +152 / +168: retail's vectors are 12 bytes, the Story..Gaming start-time vector / duration pairs
//   advance by 0x10); its callers are the serve-food / sit-for-meal / eat-at-home AI state groups (0x00923AA0, 0x00923B00, 0x009455B0, 0x009456A0, 0x0096D200). The manifest's "CTCVillage::IsCookTime" is propagated across byte-identical bodies.
// Callees: CTCVillage::IsTimeBetween(const vector<long>&, long) const 0x00658620.
// Shape (family M179, representative 0x00658B6B): `return IsTimeBetween(GetDef()->EatStartTimes, GetDef()->EatDuration);`
//   -- two by-value CDefPointer temporaries (the duration's first: arguments right to left), released in reverse; the
//   dereferenced one's release needs no null test. `optimize("s", on)`.
#pragma optimize("s", on)
class CDefPointeeBase {
public:
    virtual ~CDefPointeeBase();
    virtual void Delete() const;                          // slot 1 (+4)
    mutable unsigned long IVRefCount;                     // +0x04
};
template <class T> class CDefPointer {
public:
    T* Object;
    __forceinline CDefPointer(const CDefPointer& p) : Object(p.Object) { if (Object) ++Object->IVRefCount; }
    __forceinline ~CDefPointer() { if (Object && --Object->IVRefCount == 0) Object->Delete(); }
    T* operator->() const { return Object; }
};
namespace std {
template <class _Tp> class allocator { public: allocator() { } };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector { public: _Tp* _M_start; _Tp* _M_finish; _Tp* _M_end_of_storage; };
}
class CVillageDef : public CDefPointeeBase {
public:
    unsigned char     _pad08[0x84 - 0x08];
    std::vector<long> EatStartTimes;                    // +0x84 (PDB +152)
    long              EatDuration;                      // +0x90 (PDB +168)
};
class CTCVillage {
public:
    unsigned char _pad00[0x94];
    CDefPointer<const CVillageDef> PDef;                  // +0x94
    __forceinline CDefPointer<const CVillageDef> GetDef() const { return PDef; }
    bool IsTimeBetween(const std::vector<long>& startTimes, long duration) const;   // 0x00658620
    bool IsEatTime() const;
};
bool CTCVillage::IsEatTime() const
{
    return IsTimeBetween(GetDef()->EatStartTimes, GetDef()->EatDuration);
}