// Grade: DIFFER(190v187) -- register allocation: retail keeps defIndex in ebp, the zero / appearance def in ebx and loads the thing def into esi right after GetDef (reloading it from its slot after the erase loop); ours swaps ebx/ebp and reads the thing def from its slot at each use
// CTCHeroAttachableAppearanceModifiers::RemoveAppearanceModifier(long defIndex) -- take every entry of a modifier def out of the worn list of its type, flag the change and stamp the frame
// Leaf: RemoveAppearanceModifier
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00706370, length 185 bytes. __thiscall, `ret 4`.
//         Ego_r.pdb: `?RemoveAppearanceModifier@CTCHeroAttachableAppearanceModifiers@@QAEXJ@Z`. Label: the catalog's
//         "AddAppearanceModifier" is its sibling (the body erases matching entries).
// Callees: CTCBase::GetDefinitions() const @ 0x006869D0, CDefinitionManager::GetDef<CThingObjectDef>(long, CDefPointer<const
//          T>*) const @ 0x0042B040, CDefClassBase::GetSubDef<CAppearanceModifierDef>(CDefPointer<const T>*) const @ 0x0057FF92,
//          vector<CModifierEntry>::erase(iterator) @ 0x00708600, CTCBase::GetWorld() const @ 0x00686A80,
//          CWorld::GetWorldFrame (static) @ 0x0049D870, the defs' virtual slot 1.
// Shape: the thing def is a CDefPointer local (inline dtor, re-read from its slot after the erase calls), the appearance
//        def a plain out slot read once into a local released by hand before it; one ebx zero clears both out slots
//        after the pushes and stands for the never-filled appearance def on the first miss path; the erase loop re-reads
//        end() after its first test; Changed is the low bit of +0x14.
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

template<class T> class CDefOutPointer {           // a plain out slot (the def is released by hand)
public:
    T* Object;
    CDefOutPointer() : Object(0) {}
};

class CDefClassBase {
public:
    virtual void v00();
    virtual void Destroy() const;                   // slot 1
    mutable long RefCount;                          // +0x04
    template<class T> bool GetSubDef(CDefOutPointer<const T>* out) const;   // 0x0057FF92 for CAppearanceModifierDef
};
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
class CDefinitionHolder : public CDefinitionManager { };

struct CWorld {
    static long GetWorldFrame();                    // 0x0049D870
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
class CTCHeroAttachableAppearanceModifiers {
public:
    virtual void v00();
    CThing* ParentThing;                            // +0x04 (CTCBase)
    unsigned char _pad08[0x10 - 0x08];
    unsigned long FrameLastChanged;                 // +0x10
    unsigned char Changed : 1;                      // +0x14 bit 0
    unsigned char RecreatePrimitives : 1;
    unsigned char AppearanceModifiersLoaded : 1;
    unsigned char _pad15[0x30 - 0x15];
    CRetailVector< CRetailVector<CModifierEntry> > AppearanceModifiers;   // +0x30
    const CDefinitionHolder* GetDefinitions() const;   // 0x006869D0 (CTCBase)
    CWorld* GetWorld() const;                       // 0x00686A80 (CTCBase)
    void RemoveAppearanceModifier(long defIndex);
};

void CTCHeroAttachableAppearanceModifiers::RemoveAppearanceModifier(long defIndex)
{
    CDefPointer<const CThingObjectDef> thingDef;
    CDefOutPointer<const CAppearanceModifierDef> appearanceOut;
    const CAppearanceModifierDef* appearanceDef = appearanceOut.Object;
    if (GetDefinitions()->GetDef(defIndex, &thingDef)) {
        bool found = thingDef->GetSubDef(&appearanceOut);
        appearanceDef = appearanceOut.Object;
        if (found) {
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
    FrameLastChanged = GetWorld()->GetWorldFrame();
    if (appearanceDef && --appearanceDef->RefCount == 0)
        appearanceDef->Destroy();
}
