// CAnimComponentFollowingOnAnimFilter::Initialise(CPersistContext& context, const CCharString& value) -- reset to the default, then take the quoted text of value (its first and last characters stripped) as the next animation filter; false when value is too short
// Leaf: ?Initialise@CAnimComponentFollowingOnAnimFilter@@UAE_NAAVCPersistContext@@ABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005DB530, 81 bytes, virtual thiscall `ret 8`.
// Identity (proven): slot 3 of CAnimComponentFollowingOnAnimFilter's vtable 0x012557D8 (rtti-vtables); FableWin.pdb
//   ?Initialise@CAnimComponentFollowingOnAnimFilter@@UAE_NAAVCPersistContext@@ABVCCharString@@@Z; byte twin of 0x005DB4C0 (CAnimComponentFollowingOnAnim::Initialise, NextAnimName).
// Callees: SetDefaultValue (own vtable slot 8, +0x20; an empty body here), CCharString::GetSubString(unsigned long,
//   unsigned long) const 0x0099EC70 (by value, hidden result in the `push ecx` slot), CDefString::SetString(const
//   CCharString&) 0x009D5060 (void, it does not return this, so it is not the constructor), ~CCharString 0x0099EAE0.
// Layout: Ego_r.pdb CAnimComponentFollowingOnAnimFilter::NextFilter (CDefString, one long TablePos) +8.
// Shape: the length is read into a signed local (`jle`); `NextFilter = CDefString(sub)` with the CDefString(const CCharString&) constructor inline (it calls SetString): the
//   temporary sits in the dead `value` argument slot and is copied out before the substring dies.
class CCharStringData { public:
    const char*   PData;                                // +0
    unsigned long Length;                               // +4
};
class CCharString { public:
    CCharStringData* PStringData;
    CCharString(const CCharString& other);
    ~CCharString();                                     // 0x0099EAE0
    unsigned long GetLength() const { return PStringData == 0 ? 0 : PStringData->Length; }
    CCharString GetSubString(unsigned long start, unsigned long end) const;   // 0x0099EC70
};
class CDefString { public:
    long TablePos;                                      // +0
    CDefString(const CCharString& s) { SetString(s); }
    void SetString(const CCharString& s);               // 0x009D5060
};
class CPersistContext;
class CAnimComponentBase { public:
    virtual ~CAnimComponentBase();
    virtual void _v001(); virtual void _v002();
    virtual bool Initialise(CPersistContext& context, const CCharString& value);   // slot 3
    virtual void _v004(); virtual void _v005(); virtual void _v006(); virtual void _v007();
    virtual void SetDefaultValue();                     // slot 8 (+0x20)
    long _base04;                                       // +4
};
class CAnimComponentFollowingOnAnimFilter : public CAnimComponentBase { public:
    CDefString NextFilter;                                   // +8
    bool Initialise(CPersistContext& context, const CCharString& value);
};

bool CAnimComponentFollowingOnAnimFilter::Initialise(CPersistContext& context, const CCharString& value)
{
    SetDefaultValue();
    long length = value.GetLength();
    if (length > 2) {
        NextFilter = CDefString(value.GetSubString(1, length - 1));
        return true;
    }
    return false;
}