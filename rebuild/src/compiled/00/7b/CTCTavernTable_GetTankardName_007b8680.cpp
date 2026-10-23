// CTCTavernTable::GetTankardName() -- the tankard object name for this table: the high-quality "OBJECT_TG_TANKARD_01" or the village "OBJECT_VILLAGE_TAVERN_TANKARD"
// Leaf: GetTankardName
// Retail: Fable.exe @ 0x007B8680, 144 bytes (`ret 4`, CCharString by hidden pointer).
// Callees: CDefClassBase::GetSubDef<CTavernTableDef>(CDefPointer*) 0x007B9700 on the owner's def class (CThing+0x70; the
//          manifest template label is propagated); CCharString::CCharString(const char*, long) 0x0099EBF0 (one per arm),
//          CCharString::CCharString(const CCharString&) 0x0099EC30 (into the return slot), CCharString::~CCharString
//          0x0099EAE0 (each arm's temporary, under the ebx construction flags); the def's slot 1 on release.
//          Strings 0x0126DB24 "OBJECT_TG_TANKARD_01", 0x0126DB04 "OBJECT_VILLAGE_TAVERN_TANKARD".
// Layout: Ego_r.pdb CTavernTableDef::HighQualityTankards +0x26 (holds in retail).
// Shape: `return cond ? CCharString(a) : CCharString(b);` -- each arm builds its own temporary (flag bits 1/2 in ebx),
//        the result is copy-constructed and the live temporary destroyed by flag; the def is read once into edi and
//        released by hand without a null test.
struct CCharString {
    void* m_data;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};

struct CDefPointee {
    virtual void v00();
    virtual void Destroy();                     // slot 1
    long m_count;                               // +0x04
};
struct CTavernTableDef : CDefPointee {
    unsigned char _pad_0x08[0x1d];
    bool AddTankards;                           // +0x25
    bool HighQualityTankards;                   // +0x26
};
struct CDefPointer {
    CTavernTableDef* Object;
    CDefPointer() : Object(0) {}
};
struct CDefReference {                          // holder of the taken def: releases it without re-reading the out slot
    CTavernTableDef* Object;
    CDefReference(CTavernTableDef* object) : Object(object) {}
    ~CDefReference()
    {
        if (--Object->m_count == 0)
            Object->Destroy();
    }
    CTavernTableDef* operator->() const { return Object; }
};

struct CDefClassBase {
    bool GetSubDef_007b9700(CDefPointer* out);           // 0x007B9700
};
void* FindDef_CDefClassBase_GetSubDef_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef_007b9700(CDefPointer* out)
{
    void* def = FindDef_CDefClassBase_GetSubDef_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)
struct CThing {
    unsigned char _pad_0x00[0x70];
    CDefClassBase* DefHolder;                   // +0x70
};

struct CTCTavernTable {
    virtual void v00();
    CThing* ParentThing;                        // +0x04
    __forceinline CTavernTableDef* TakeTableDef()
    {
        CDefPointer out;
        ParentThing->DefHolder->GetSubDef_007b9700(&out);
        return out.Object;
    }
    CCharString GetTankardName();
};

CCharString CTCTavernTable::GetTankardName()
{
    CDefReference def(TakeTableDef());
    return def->HighQualityTankards ? CCharString("OBJECT_TG_TANKARD_01") : CCharString("OBJECT_VILLAGE_TAVERN_TANKARD");
}