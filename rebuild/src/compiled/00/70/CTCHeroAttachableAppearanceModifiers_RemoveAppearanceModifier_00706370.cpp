// CTCHeroAttachableAppearanceModifiers::RemoveAppearanceModifier(long defIndex) -- take every entry of a modifier def out of the worn list of its type, flag the change and stamp the frame
// Leaf: RemoveAppearanceModifier
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00706370, length 185 bytes. __fastcall, `ret 4`.
//         Ego_r.pdb: `?RemoveAppearanceModifier@CTCHeroAttachableAppearanceModifiers@@QAEXJ@Z`. Label: the catalog's
//         "AddAppearanceModifier" is its sibling (the body erases matching entries).
// Callees: CTCBase::GetDefinitionManager() const @ 0x006869D0, CDefinitionManager::GetDef<CThingObjectDef>(long, CDefPointer<const
//          T>*) const @ 0x0042B040, CDefClassBase::GetSubDef<CAppearanceModifierDef>(const T**) const @ 0x0057FF92,
//          vector<CModifierEntry>::erase(iterator) @ 0x00708600, CTCBase::PeekWorld() const @ 0x00686A80,
//          CWorld::GetWorldFrame (static) @ 0x0049D870, the defs' virtual slot 1.
// Shape: the thing def is a CDefPointer local (inline dtor, re-read from its slot after the erase calls), the appearance
//        def a raw pointer that GetSubDef fills through `const T**` (released by hand before the thing def); GetDef and
//        GetSubDef are same-TU noinline stand-in bodies that write only the out slot, so both defs stay in registers
//        (esi, ebx) between calls; one ebx zero clears both out slots after the pushes and stands for the never-filled
//        appearance def on the first miss path; the erase loop re-reads end() after its first test; Changed is the low
//        bit of +0x14.
// Layout: local model -- engine/CTCHeroAttachableAppearanceModifiers.h carries the 16-byte PDB containers; retail
//         FrameLastChanged +0x10, Changed +0x14 bit 0, AppearanceModifiers +0x30; retail CAppearanceModifierDef type +0x50.
template<class T> class CDefPointer {
public:
    T* Object;
    CDefPointer() : Object(0) {}
    ~CDefPointer()
    {
        if (Object && --Object->RefCount == 0)
            Object->Destroy();
    }
    T* operator->() const { return Object; }
};

class CDefClassBase {
public:
    virtual void v00();
    virtual void Destroy() const;                   // slot 1
    mutable long RefCount;                          // +0x04
    template<class T> bool GetSubDef(const T** out) const;   // 0x0057FF92 for CAppearanceModifierDef
};
void* Find_CDefClassBase_GetSubDef(const void* owner);   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
template <class T> __declspec(noinline) bool CDefClassBase::GetSubDef(const T** out) const
{
    void* def = Find_CDefClassBase_GetSubDef(this);
    if (!def)
        return false;
    *out = (const T*)def;
    return true;
}
#pragma auto_inline(on)
class CAppearanceModifierDef : public CDefClassBase {
public:
    unsigned char _pad08[0x50 - 0x08];
    long Type;                                      // +0x50 EHeroAttachableAppearanceModifierType
};
class CThingObjectDef : public CDefClassBase { };

class CDefinitionManager {
public:
    template<class T> bool GetDef(long index, CDefPointer<const T>* out) const;   // 0x0042B040 for CThingObjectDef
};
void* Lookup_CDefinitionManager_GetDef(const void* manager, long index);   // stand-in callee
#pragma auto_inline(off)
template <class T> __declspec(noinline) bool CDefinitionManager::GetDef(long index, CDefPointer<const T>* out) const
{
    void* def = Lookup_CDefinitionManager_GetDef(this, index);
    if (!def)
        return false;
    out->Object = (const T*)def;
    return true;
}
#pragma auto_inline(on)
class CGameDefinitionManager : public CDefinitionManager { };

class CWorld { public:
    static long __fastcall GetFrame();                    // 0x0049D870
};

struct CModifierEntry {                             // CTCHeroAttachableAppearanceModifiers::CModifierEntry (8 bytes)
    long DefIndex;                                  // +0x00 global def index
    long Data;                                      // +0x04
};

template <class T> struct CRetailVector {           // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* erase(T* position);                          // 0x00708600 for CModifierEntry
};

class CThing;
class CTCBase
{
public:
    virtual void v00();
protected:
    const CGameDefinitionManager& GetDefinitionManager() const;   // 0x006869D0
    const CWorld& PeekWorld() const;   // 0x00686A80
};

class CTCHeroAttachableAppearanceModifiers : public CTCBase {
public:
    CThing* ParentThing;                            // +0x04 (CTCBase)
    unsigned char _pad08[0x10 - 0x08];
    unsigned long FrameLastChanged;                 // +0x10
    unsigned char Changed : 1;                      // +0x14 bit 0
    unsigned char RecreatePrimitives : 1;
    unsigned char AppearanceModifiersLoaded : 1;
    unsigned char _pad15[0x30 - 0x15];
    CRetailVector< CRetailVector<CModifierEntry> > AppearanceModifiers;   // +0x30
    void RemoveAppearanceModifier(long defIndex);
};

void CTCHeroAttachableAppearanceModifiers::RemoveAppearanceModifier(long defIndex)
{
    CDefPointer<const CThingObjectDef> thingDef;
    const CAppearanceModifierDef* appearanceDef = 0;
    if (GetDefinitionManager().GetDef(defIndex, &thingDef)) {
        if (thingDef->GetSubDef(&appearanceDef)) {
            CRetailVector<CModifierEntry>& entries = AppearanceModifiers._M_start[appearanceDef->Type];
            for (CModifierEntry* it = entries._M_start; it != entries._M_finish; ) {
                if (it->DefIndex == defIndex)
                    it = entries.erase(it);
                else
                    ++it;
            }
            Changed = true;
        }
    }
    FrameLastChanged = PeekWorld().GetFrame();
    if (appearanceDef && --appearanceDef->RefCount == 0)
        appearanceDef->Destroy();
}