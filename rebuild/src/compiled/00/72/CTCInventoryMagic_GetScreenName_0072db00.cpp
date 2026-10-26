// CTCInventoryMagic::GetScreenName() const -- the magic screen's title: the inventory def's magic screen-name tag looked up in the game-text data bank
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0072DB00, length 111 bytes (+ int3), `ret 4` (hidden CWideString result).
// Vtable 0x012668D8 slot 0: the NHeroInformationScreens::CBase secondary base at +0x130 (ctor 0x0072DB90 stores it at
// [esi+130h]), so `this` is the subobject and the full object's PDef (+0x68) is read at [ecx-0C8h].
// Ego_r.pdb: ?GetScreenName@CTCInventoryMagic@@UBE?AVCWideString@@XZ.
// Callees: NGameText::CDataBank::GetPGroupEntry(unsigned long, bool) const @ 0x009C9070, the group entry's GetText @ 0x009C7A40,
// the def's slot 1 / the entry's slot 1 on release. Globals: the engine singleton 0x013B86A0 (+0x14 = game-text data bank).
// Shape: a copied CDefPointer local (add-ref with a null test, release without one: `dec [esi+4] ; jne`); the text bank is a
// named local read after the copy (retail loads it before the tag); the counted group entry is a temporary (GetText runs on
// the returned pointer, `mov ecx,[eax]`) whose dtor has a null test, the inline ReleaseTemporary and a pointer clear.
// Leaf: GetScreenName
// Layout: retail CTCInventoryBase PDef +0x68 (Ego_r.pdb +0x70 less the 4 bytes of each of the two 16-byte debug vectors
// before it); the screen-name tag at CInventoryDef +0x1EC (retail offset; PDB name not resolved).
class CWideString {
public:
    void* PStringData;
};

struct CRefCounted {
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CRefVirtual {
    virtual void v00();
    virtual void Destroy();                     // slot 1
};
namespace NGameText {
class CGroupEntry : public CRefVirtual, public CRefCounted { public:
    CWideString GetText() const;                // 0x009C7A40
    void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
}
template <class T> class CIVCountedPointer { public:
    T* Data;
    ~CIVCountedPointer() { if (Data) { Data->ReleaseTemporary(); Data = 0; } }
    T* operator->() const { return Data; }
};
namespace NGameText {
class CDataBank {
public:
    CIVCountedPointer<CGroupEntry> GetPGroupEntry(unsigned long tag, bool b) const;   // 0x009C9070
};
}
class CEngine { public:
    unsigned char _pad00[0x14];
    NGameText::CDataBank* PGameTextDataBank;    // +0x14
};
extern CEngine* g_pEngine;                      // 0x013B86A0

class CDefPointeeBase { public:
    virtual void v00();
    virtual void Destroy();                     // slot 1
    long m_refCount;                            // +0x04
};
class CInventoryDef : public CDefPointeeBase { public:
    unsigned char _pad08[0x1ec - 0x08];
    long MagicScreenNameTag;                    // +0x1ec
};
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) ++((CDefPointeeBase*)Object)->m_refCount; }
    ~CDefPointer() { if (--((CDefPointeeBase*)Object)->m_refCount == 0) ((CDefPointeeBase*)Object)->Destroy(); }
    T* operator->() const { return Object; }
};

struct CTCInventoryBase_Magic {                 // retail 0x130 bytes
    virtual void v00();
    unsigned char _pad04[0x68 - 0x04];
    CDefPointer<CInventoryDef> PDef;            // +0x68
    unsigned char _pad6c[0x130 - 0x6c];
};
namespace NHeroInformationScreens {
class CBase { public:                                  // vtable 0x0123A3E0
    virtual CWideString GetScreenName() const;  // slot 0
    virtual void v01();
    virtual void v02();
};
}
class CTCInventoryMagic : public CTCInventoryBase_Magic, public NHeroInformationScreens::CBase {
public:
    virtual CWideString GetScreenName() const;
};

CWideString CTCInventoryMagic::GetScreenName() const
{
    CDefPointer<CInventoryDef> def = PDef;
    NGameText::CDataBank* textBank = g_pEngine->PGameTextDataBank;
    return textBank->GetPGroupEntry(def->MagicScreenNameTag, true)->GetText();
}