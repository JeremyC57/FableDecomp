// CTCHitLocations::GetPHitLocationFromName(const CCharString& name) -- returns the first hit location whose definition's instantiation name equals `name`, else null
// Leaf: GetPHitLocationFromName
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00815820, length 70 bytes (`ret 4`).
// Callees: the hit-location def's virtual slot 10 (CParentDefClassBase::GetInstantiationName, the
//          CDefString returned through a hidden pointer), CDefString::operator==(const CCharString&)
//          const @ 0x004B7980 (looks the table string up and compares the text).
// Identity: the manifest label "CParticleEmitter::HasAllocatedDecals" is propagated. The callers
// (creature actions such as 0x008C862B, the Jack of Blades AI groups 0x00972509..) fetch the thing
// component of type 0x7f, call this with a CCharString literal and then CHitLocation::SetEnabled
// (0x00816390) on the result. PDB CTCHitLocations: HitLocations = vector<pair<CHitLocation, long> >
// at +0x0c (retail 12 bytes; 0x1c-byte elements = CHitLocation (0x18) + long), CHitLocation::PDef
// (CDefPointer<CHitLocationDef>) at +4 of each element. The name is descriptive.
// Shape: top-tested walk with `last` re-read after each pass; the name reference is loaded into
// ebx inside the non-empty path, the returned CDefString temporary is homed in that dead argument
// slot (`lea edx,[esp+10h]` for the hidden pointer, `lea ecx,[esp+14h]` as `this` of ==), a hit
// returns the element address, a miss `xor eax,eax`. The def name is a NAMED local: the call runs
// before the name is pushed and == re-addresses the slot (the bare temporary `GetInstantiationName()
// == name` pushes the name first and reuses the returned eax as `this`).

struct CCharString {
    void* m_pData;
};

struct CDefString {
    long TablePos;
    CDefString(const CDefString& other);                                  // user copy constructor: returned through a hidden pointer
    bool operator==(const CCharString& other) const;                      // 0x004B7980
};

struct CHitLocationDef {                                                  // CParentDefClassBase-derived def
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual CDefString GetInstantiationName() const;                      // slot 10
};

template <class T>
struct CDefPointer {
    T* m_pObject;
    T* operator->() const { return m_pObject; }
};

namespace NHitLocations {

struct CHitLocation {
    void*                        POwnerThing;                             // +0x00
    CDefPointer<CHitLocationDef> PDef;                                    // +0x04
    void*                        PArmourDef;                              // +0x08
    long                         HitResponseAction;                       // +0x0c
    long                         BlockResponseAction;                     // +0x10
    bool                         Enabled;                                 // +0x14
};

} // namespace NHitLocations

struct SHitLocationEntry {                                                // pair<CHitLocation, long> (0x1c bytes)
    NHitLocations::CHitLocation first;
    long                        second;
};

struct CHitLocationVector {                                               // retail std::vector (12 bytes)
    SHitLocationEntry* m_first;
    SHitLocationEntry* m_last;
    SHitLocationEntry* m_end;
};

struct CTCHitLocations {
    virtual void v00();
    unsigned char      _base[8];                                          // +0x04 thing-component base
    CHitLocationVector HitLocations;                                      // +0x0c
    NHitLocations::CHitLocation* GetPHitLocationFromName(const CCharString& name);
};

NHitLocations::CHitLocation* CTCHitLocations::GetPHitLocationFromName(const CCharString& name)
{
    for (SHitLocationEntry* it = HitLocations.m_first; it != HitLocations.m_last; ++it) {
        CDefString defName = it->first.PDef->GetInstantiationName();
        if (defName == name)
            return &it->first;
    }
    return 0;
}