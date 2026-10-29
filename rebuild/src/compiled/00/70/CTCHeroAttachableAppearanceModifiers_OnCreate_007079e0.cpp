// CTCHeroAttachableAppearanceModifiers::OnCreate() -- applies the creature def's initial appearance modifiers (unless already loaded) plus the horns, then uploads the change
// Leaf: OnCreate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007079E0, 234 bytes; vtable 0x01264F0C slot 4 (catalogued "vfunc_4";
// slot 4 of the CTCBase vtables is OnCreate).
// Callees: CDefClassBase::GetSubDef<CCreatureDef>(CDefPointer<CCreatureDef>*) @ 0x00485815 (landed),
//          CTCHeroAttachableAppearanceModifiers::AddAppearanceModifier(long) @ 0x00706880 (per def entry, then the
//          horns), CCharString::CCharString(const char*, long) @ 0x0099EBF0, CTCBase::GetDefinitionManager() @ 0x006869D0,
//          CDefinitionManager::GetDefGlobalIndexFromName(const CCharString&) @ 0x009AD410, ~CCharString @ 0x0099EAE0,
//          UpdateHiddenModifiers @ 0x00706040, UpdateHiddenMaterials @ 0x00706510, UpdateGraphics @ 0x007070D0,
//          NGraphicAppearance::CTCGraphicAppearance::SetAsChanged(ulong) @ 0x004BF9E0, CTCBase::PeekWorld() const
//          @ 0x00686A80, CWorld::GetWorldFrame() @ 0x0049D870 (static).
// Shape: the sub-def comes through a plain out slot (zeroed, filled by GetSubDef) copied once into a holder whose
// destructor releases it (the def stays in edi for the loop and the null-tested release); the loop re-reads the
// vector bounds after every call. The horns go through a block-scoped CCharString in the `push ecx` frame word.
// `Changed = true` followed by the inlined UploadChangesToGraphics (the landed FrameUpdate 0x00707AD0 body) keeps
// the bit store/test pair; FrameLastChanged is stamped last.
// Layout: the landed FrameUpdate model (retail 12-byte containers; engine/CTCHeroAttachableAppearanceModifiers.h
// carries PDB offsets): bit-field byte +0x14 {Changed, RecreatePrimitives, AppearanceModifiersLoaded},
// FrameLastChanged +0x10; CCreatureDef::InitialAppearanceModifiers +0x54 (PDB +0x58).
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length);      // 0x0099EBF0
    ~CCharString();                                  // 0x0099EAE0
};

struct CDefBase {
    virtual void v00();
    virtual void Destroy();                          // slot 1
    long m_count;                                    // +0x04
    void Release() { if (--m_count == 0) Destroy(); }
};

struct CLongVector {                                 // retail std::vector<long> {first, last, end}
    long* _M_start;
    long* _M_finish;
    long* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    const long& operator[](unsigned long n) const { return _M_start[n]; }
};

class CCreatureDef : public CDefBase { public:
    unsigned char _pad_0x08[0x4C];
    CLongVector   InitialAppearanceModifiers;        // +0x54 (PDB +0x58)
};

template <class T> class CDefPointer { public:              // the out slot GetSubDef fills
    T* Object;
};

template <class T> struct CDefHolder {               // releases the def read out of the slot
    T* m_p;
    CDefHolder(T* p) : m_p(p) {}
    ~CDefHolder() { if (m_p) m_p->Release(); }
    T* operator->() const { return m_p; }
};

class CDefClassBase { public:
    bool GetSubDef(CDefPointer<CCreatureDef>* out);  // 0x00485815
};
void* FindDef_CDefClassBase_GetSubDef_1();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<CCreatureDef>* out)
{
    void* def = FindDef_CDefClassBase_GetSubDef_1();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)

class CDefinitionManager { public:
    long GetDefGlobalIndexFromName(const CCharString& name) const;   // 0x009AD410
};
class CGameDefinitionManager : public CDefinitionManager
{
};

namespace NGraphicAppearance {
class CTCGraphicAppearance { public:
    void SetAsChanged(unsigned long flags);          // 0x004BF9E0
};
}

class CThing { public:
    unsigned char _pad00[0x64];
    NGraphicAppearance::CTCGraphicAppearance* PTCGraphicAppearance;   // +0x64
    unsigned char _pad68[0x08];
    CDefClassBase* PDef;                             // +0x70 CDefPointer<const CThingDef>::Object
};

class CWorld { public:
    static long __fastcall GetFrame();                     // 0x0049D870
};

class CTCBase { public:
    virtual void v00();
    CThing* ParentThing;                             // +0x04
    bool    ToBeRemoved;                             // +0x08
    bool    IsInGlobalUpdateTCs;                     // +0x09
protected:
    const CGameDefinitionManager& GetDefinitionManager() const;   // 0x006869D0
    const CWorld& PeekWorld() const;   // 0x00686A80
};

class CTCHeroAttachableAppearanceModifiers : public CTCBase { public:
    void*         __vftable_0c;                      // +0x0c secondary vptr
    unsigned long FrameLastChanged;                  // +0x10
    bool Changed : 1;                                // +0x14 bit 0
    bool RecreatePrimitives : 1;                     // +0x14 bit 1
    bool AppearanceModifiersLoaded : 1;              // +0x14 bit 2

    void AddAppearanceModifier(long defIndex);       // 0x00706880
    void UpdateHiddenModifiers();                    // 0x00706040
    void UpdateHiddenMaterials();                    // 0x00706510
    void UpdateGraphics();                           // 0x007070D0
    void UploadChangesToGraphics()
    {
        if (Changed) {
            Changed = false;
            UpdateHiddenModifiers();
            UpdateHiddenMaterials();
            UpdateGraphics();
            ParentThing->PTCGraphicAppearance->SetAsChanged(15);
        }
    }
    void OnCreate();
};

void CTCHeroAttachableAppearanceModifiers::OnCreate()
{
    if (!AppearanceModifiersLoaded) {
        CDefClassBase* thingDef = ParentThing->PDef;
        CDefPointer<CCreatureDef> out;
        out.Object = 0;
        bool found = thingDef->GetSubDef(&out);
        CDefHolder<CCreatureDef> def(out.Object);
        if (found) {
            for (unsigned long i = 0; i < def->InitialAppearanceModifiers.size(); ++i)
                AddAppearanceModifier(def->InitialAppearanceModifiers[i]);
        }
    }
    {
        CCharString horns("OBJECT_HERO_HORNS", -1);
        AddAppearanceModifier(GetDefinitionManager().GetDefGlobalIndexFromName(horns));
    }
    Changed = true;
    UploadChangesToGraphics();
    FrameLastChanged = PeekWorld().GetFrame();
}