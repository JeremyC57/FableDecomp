// CTCDegradableBase::IsFullyDegraded() const -- true when the current degradation stage is the definition's last one
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008EC900, length 59 bytes.
// Callees: none direct; the definition's virtual slot 1 (`call [edx+4]`, the def-pointee self-destroy) when the
// temporary CDefPointer drops the last reference.
// Name and constness from ghidra_out/egor_signatures.tsv (`?IsFullyDegraded@CTCDegradableBase@@QBE_NXZ`); the
// definition is fetched through the inline `?GetDefinition@CTCDegradableBase@@IBE?AV?$CDefPointer@$$CBVCDegradableDef@@@@XZ`
// (a CDefPointer returned BY VALUE: add-ref on the copy, release after the compare).
// PDB CTCDegradableBase: PDefinition +0x10, PCurrentDegradableInfo +0x14. PDB CDegradableDef: Degradations
// (vector<CDegradableInfo>, 28-byte elements) +0x30 -- the retail 12-byte vector keeps its last pointer at +0x34.
// Shape: the null test on the current info returns false; the def copy add-refs with a null test
// (`inc dword ptr [eax+4]` = an inline AddRef helper), the def is then dereferenced unchecked (back() =
// last - 0x1c), the compare result is parked in bl (`sete bl`) across the release (load/dec/store
// of the count, then the virtual destroy), and two return tails follow.
class CDefPointeeBase { public:
    virtual void v00();
    virtual void DestroySelf();                             // slot 1 (+4)
    long RefCount;                                          // +0x04
    void AddRef() { ++RefCount; }
    void Release() { --RefCount; if (RefCount == 0) DestroySelf(); }
};

template<class T> class CDefPointer { public:
    T* Object;
    CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object != 0) Object->AddRef(); }
    ~CDefPointer() { if (Object != 0) Object->Release(); }
    T* operator->() const { return Object; }
};

class CDegradableInfo { public:                                    // 0x1c bytes
    float         HealthPercentage;                         // +0x00
    unsigned char EngineGraphic[0x10];                      // +0x04 CEngineGraphic
    long          SmashParticleEmitter;                     // +0x14
    bool          BlocksNavigation;                         // +0x18
};

template<class T> struct CRetailVector {
    T* m_pFirst;
    T* m_pLast;
    T* m_pEnd;
    T& back() const { return *(m_pLast - 1); }
};

class CDegradableDef : public CDefPointeeBase { public:
    unsigned char                 _pad[0x30 - 0x8];         // CDefBase / CThingComponentDef fields, GraphicType ...
    CRetailVector<CDegradableInfo> Degradations;            // +0x30
};

class CTCDegradableBase { public:
    virtual void v00();
    unsigned char                       _base[0x8];         // CTCBase: ParentThing +4, flags +8/+9
    long                                LastWorldFrameHit;  // +0x0c
    CDefPointer<CDegradableDef>         PDefinition;        // +0x10 CDefPointer<const CDegradableDef>
    CDegradableInfo*                    PCurrentDegradableInfo;   // +0x14
    void*                               PParentThingPhysical;     // +0x18 CThingPhysical*
    CDefPointer<CDegradableDef> GetDefinition() const { return PDefinition; }
    bool IsFullyDegraded() const;
};

bool CTCDegradableBase::IsFullyDegraded() const
{
    if (PCurrentDegradableInfo != 0) {
        if (PCurrentDegradableInfo == &GetDefinition()->Degradations.back())
            return true;
    }
    return false;
}