// CTCChest::GetOpenAnimationForCreature() const -- the chest def's OpenAnimationForCreature when it is set, else "OPEN_CHEST"
// Leaf: GetOpenAnimationForCreature
// Retail: Fable.exe 0x00750240, 130 bytes (`ret 4`, CCharString through the hidden pointer).
// Identity: the manifest label "CTCSinglePersonOccupiable::IsOfType" is a propagation (the body returns a string);
// called only by CTCActionUseChest::OnActionUse 0x007EE8F0 (on the thing's interface-0x3C component = CTCChest, as the
// landed CTCActivationReceptorChest::Activate 0x007FFC70) and CGameScriptInterface::OpenChest 0x00896390; it sits
// between CTCChest members (Open 0x00750540, Close 0x007506C0) and reads CChestDef::OpenAnimationForCreature.
// Callees: CDefClassBase::GetSubDef<CChestDef>(CDefPointer<const CChestDef>*) @ 0x0074F7D0 (manifest
// "GetSubDef<CAICreatureWillPowerIndicatorDef>", byte twin), CCharString::operator!=(const char*) @ 0x0099E960,
// CCharString(const CCharString&) @ 0x0099EC30, CCharString(const char*, long = -1) @ 0x0099EBF0, the def's virtual
// slot 1 (release at zero). Literals "" @ 0x0122D70E, "OPEN_CHEST" @ 0x0126955C.
// Layout: the parent thing's def +0x70; CChestDef (Ego_r.pdb 0x3C): OpenAnimationForCreature +0x30.
// Shape: the thing's def is read into a local before the out CDefPointer (null in the `push ecx` frame word) is
// declared; the object is copied out of it into a holder (esi) whose destructor releases it after either string is
// built straight into the return slot (null test, `dec [esi+4] ; jne`); releasing through the escaped out pointer
// itself re-reads it from the frame.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);             // 0x0099EC30
    bool operator!=(const char* text) const;           // 0x0099E960
};
class CDefPointeeBase { public:
    virtual void Destroy();                            // slot 0
    virtual void Delete();                             // slot 1 (+0x04)
    long m_refCount;                                   // +0x04
};
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
};
// The def taken out of the out pointer right after GetSubDef and released when the function returns (after the
// result string is built in the return slot).
template <class T> struct CDefHolder {
    T* Object;
    CDefHolder(T* object) : Object(object) { }
    ~CDefHolder()
    {
        if (Object && --Object->m_refCount == 0)
            Object->Delete();
    }
    T* operator->() const { return Object; }
};
class CChestDef : public CDefPointeeBase { public:
    unsigned char _pad08[0x30 - 0x08];
    CCharString OpenAnimationForCreature;              // +0x30
};
class CDefClassBase : public CDefPointeeBase { public:
    bool GetSubDef(CDefPointer<CChestDef>* out);       // 0x0074F7D0
};
class CThing { public:
    unsigned char  _pad00[0x70];
    CDefClassBase* PDef;                               // +0x70
};
class CTCChest { public:
    virtual void v00();
    CThing* ParentThing;                               // +0x04 (CTCBase)
    CCharString GetOpenAnimationForCreature() const;
};

CCharString CTCChest::GetOpenAnimationForCreature() const
{
    CDefClassBase* thingDef = ParentThing->PDef;
    CDefPointer<CChestDef> out;
    bool found = thingDef->GetSubDef(&out);
    CDefHolder<CChestDef> def(out.Object);
    if (found && def->OpenAnimationForCreature != "")
        return CCharString(def->OpenAnimationForCreature);
    return CCharString("OPEN_CHEST");
}