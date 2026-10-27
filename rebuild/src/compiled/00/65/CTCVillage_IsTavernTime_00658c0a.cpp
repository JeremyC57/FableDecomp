// CTCVillage::IsTavernTime() const -- whether the time of day lies in one of the village def's tavern windows
// Leaf: ?IsTavernTime@CTCVillage@@QBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00658C0A, 81 bytes, thiscall, plain `ret`.
// Identity (proven): Ego_r.pdb ?IsTavernTime@CTCVillage@@QBE_NXZ; it reads CVillageDef::TavernStartTimes +0x94 and TavernDuration
//   +0xA0 (Ego_r.pdb +172 / +188: retail's vectors are 12 bytes, the Story..Gaming start-time vector / duration pairs
//   advance by 0x10); its caller is CTCVillage::IsTimeToGoToTavern 0x0065AA6F. The manifest's "CTCVillage::IsCookTime" is propagated across byte-identical bodies.
// Callees: CTCVillage::IsTimeBetween(const vector<long>&, long) const 0x00658620.
// Shape (family M179, representative 0x00658B6B): `return IsTimeBetween(GetDef()->TavernStartTimes, GetDef()->TavernDuration);`
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
    unsigned char     _pad08[0x94 - 0x08];
    std::vector<long> TavernStartTimes;                    // +0x94 (PDB +172)
    long              TavernDuration;                      // +0xA0 (PDB +188)
};
class CTCVillage {
public:
    unsigned char _pad00[0x94];
    CDefPointer<const CVillageDef> PDef;                  // +0x94
    __forceinline CDefPointer<const CVillageDef> GetDef() const { return PDef; }
    bool IsTimeBetween(const std::vector<long>& startTimes, long duration) const;   // 0x00658620
    bool IsTavernTime() const;
};
bool CTCVillage::IsTavernTime() const
{
    return IsTimeBetween(GetDef()->TavernStartTimes, GetDef()->TavernDuration);
}