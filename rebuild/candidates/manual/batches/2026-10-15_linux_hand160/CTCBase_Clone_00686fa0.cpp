// CTCBase::Clone() -- look this component's class name up in the registered-TC map, allocate a fresh component of that class for the same owner thing, copy this one into it and return it
// Leaf: ?Clone@CTCBase@@QAEPAV1@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00686FA0, length 107 bytes.
// Callees: GetClassTypeName (virtual slot 22, +0x58, CCharString by hidden pointer into the frame word), the registered-TC
// CVectorMap<CCharString, CRegisteredTCInfo>::LowerBound @ 0x006870C0 on the global map @ 0x013BA89C (end word @ 0x013BA8A0;
// catalog names the byte-twin <CCharString,C3DMeshVolume> instantiation), CBasicString<char>::operator< @ 0x00429950 (the inline
// CCharString compare's last step), the info's `__fastcall` Alloc(CThing&) pointer (+4 of the pair), CCharString::~CCharString
// @ 0x0099EAE0, the clone's virtual slot 12 (+0x30, copy from this).
// Shape: the owner is read into a local (ebp) before the name call; the name is a temporary passed by reference into an inline
// static AllocTC(name, owner) (VC keeps the returned pointer in esi) and dies at the end of that statement, before the copy call
// (made without a null test). The found test is `it != end && !(name < it->first)` with the inline CCharString operator<
// written `o.PString == PString` first (key loaded before the name), then the two null tests.
struct CBasicString {
    bool operator<(const CBasicString* other) const;    // 0x00429950 (CBasicString<char>::operator<)
};
struct CCharString {
    CBasicString* PString;
    ~CCharString();                             // 0x0099EAE0
    bool operator<(const CCharString& o) const
    {
        if (o.PString == PString)
            return false;
        if (!PString)
            return true;
        if (!o.PString)
            return false;
        return PString->operator<(o.PString);
    }
};
class CThing;
class CTCBase;
struct CRegisteredTCInfo {
    CTCBase* (__fastcall* Alloc)(CThing& owner);
};
struct CRegisteredTCPair {
    CCharString first;
    CRegisteredTCInfo second;
};
struct CRegisteredTCMap {                       // CVectorMap<CCharString, CTCBase::CRegisteredTCInfo>
    CRegisteredTCPair* m_first;                 // 0x013BA89C
    CRegisteredTCPair* m_last;                  // 0x013BA8A0
    CRegisteredTCPair* LowerBound(const CCharString& key);   // 0x006870C0
};
extern CRegisteredTCMap g_RegisteredTCs;        // 0x013BA89C

class CTCBase {
public:
    virtual ~CTCBase();
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06();
    virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void CopyFrom(const CTCBase& other);     // slot 12 (+0x30)
    virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18();
    virtual void v19(); virtual void v20(); virtual void v21();
    virtual CCharString GetClassTypeName() const;   // slot 22 (+0x58)
    CThing* ParentThing;                        // +0x04
    bool    ToBeRemoved;                        // +0x08
    bool    IsInGlobalUpdateTCs;                // +0x09
    CTCBase* Clone();
    static CTCBase* AllocTC(const CCharString& name, CThing& owner)
    {
        CRegisteredTCPair* it = g_RegisteredTCs.LowerBound(name);
        if (it != g_RegisteredTCs.m_last && !(name < it->first))
            return it->second.Alloc(owner);
        return 0;
    }
};

CTCBase* CTCBase::Clone()
{
    CThing* owner = ParentThing;
    CTCBase* clone = AllocTC(GetClassTypeName(), *owner);
    clone->CopyFrom(*this);
    return clone;
}
