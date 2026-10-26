// CTCInventoryItem::IsAugmentation(long defIndex) -- static: cache the thing def of the index; true when that def carries a CAugmentationDef sub-def (the probe's def pointer is released again)
// Grade: DIFFER(73v64) -- the null-checked CDefPointer release reloads the def pointer and its count after `dec [eax+4]` (`mov ecx,[esp+4] ; mov eax,[ecx+4] ; test`) where retail keeps the pointer in ecx and branches on the dec flags (`dec [ecx+4] ; jne`), +9 bytes
// Leaf: ?IsAugmentation@CTCInventoryItem@@SI_NJ@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005D9870, 64 bytes, static `__fastcall` (defIndex in ecx), returns bool in al.
// Identity (proven): CTCInventoryItem::IsAugmentation; evidence: its GetSubDef callee 0x0042B248 fetches the 'CAugmentationDef' sub-def (PDB ??$GetSubDef@VCAugmentationDef@@@CDefClassBase@@...); callers CTCInventoryWeapons::RefreshAugmentationsLeftDescriptions 0x005C69AA and NUISystem::CManager 0x0041E6D3.
// Label: the manifest's "IsAugmentation" is propagated across byte-identical bodies (family H015: seven static predicates of
//   CTCInventoryItem that differ only in the GetSubDef instantiation they call; retail has no identical-code folding).
// Callees: CTCInventoryItem::CacheDef(long) 0x005D94B0 (private static `__fastcall`, defIndex passed through in ecx),
//   CDefClassBase::GetSubDef<CAugmentationDef>(CDefPointer<const CAugmentationDef>*) const @ 0x0042B248, the def's slot 1 on the last release;
//   global CTCInventoryItem::s_CachedThingDef 0x013B8C14 (its def pointer is the GetSubDef `this`).
// Shape: the probe CDefPointer is a zeroed local homed in the slot `push ecx` reserves; its destructor tests for null and
//   decrements through the pointer member. Tried: the early `return false` (merged false return in al), an
//   inline DecRef member of the def (long
//   `mov/dec/test/mov` form), a size TU (frame pointer), the local declared before the CacheDef test.
class CDefClassBase;
template <class T> class CDefPointer {
public:
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer();
    T* operator->() const { return Object; }
};
class CDefClassBase {                                // def object: vptr, reference count +4
public:
    virtual void v00();
    virtual void Release() const;                    // slot 1 (last reference gone)
    mutable long RefCount;                           // +0x04
    template <class T> bool GetSubDef(CDefPointer<const T>* out) const;
};
template <class T> CDefPointer<T>::~CDefPointer()
{
    if (Object != 0) {
        if (--Object->RefCount == 0)
            Object->Release();
    }
}
class CThingDef : public CDefClassBase { };
class CAugmentationDef : public CDefClassBase { };
class CTCInventoryItem {
public:
    static bool __fastcall IsAugmentation(long defIndex);
private:
    static bool __fastcall CacheDef(long defIndex);                 // 0x005D94B0
    static CDefPointer<const CThingDef> s_CachedThingDef;           // 0x013B8C14
};
bool __fastcall CTCInventoryItem::IsAugmentation(long defIndex)
{
    if (CacheDef(defIndex)) {
        CDefPointer<const CAugmentationDef> subDef;
        return s_CachedThingDef->GetSubDef(&subDef);
    }
    return false;
}
