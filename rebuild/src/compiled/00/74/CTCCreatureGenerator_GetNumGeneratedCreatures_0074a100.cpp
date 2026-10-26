// CTCCreatureGenerator::GetNumGeneratedCreatures() const -- total number of creature entries over all of the generator's creature-family defs
// Leaf: GetNumGeneratedCreatures
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0074A100, length 137 bytes (+ the out-of-range tail). Catalog label
// "CTCObjectAugmentations::GetDamageMultiplier" is propagated: the body sits among the CTCCreatureGenerator methods (0x00749D70..
// 0x0074DA30) and its caller is CGameScriptInterface::CreatureGeneratorGetNumGeneratedCreatures @ 0x0089EB80 (the call at
// 0x0089EBDF on the thing's interface-0x3D component, result returned).
// Callees: CTCBase::GetDefinitionManager @ 0x006869D0, CDefinitionManager::GetDef<family def>(long, CDefPointer*) @ 0x0074D220
// (catalog <CThingPhysicalSwitchDef> is a byte twin), the def's slot-1 release, puts @ 0x00BFEAF2 + abort @ 0x00BFEAEC (STLport
// vector::at range check, literal "vector" @ 0x01268AE4).
// Layout: PDB CTCCreatureGenerator CreatureFamilies +0x0C (CArray<CDefIndex>, 4-byte elements; begin/end +0x0C/+0x10); the family
// def's creature vector begin/end at +0x3C/+0x40 (4-byte elements).
// Shape: `for (unsigned i = 0; i < size(); ++i)` with `at(i)`; a zeroed CDefPointer local per pass (its zero store lands between
// the range-check compare and its branch) whose inline dtor copies Object into a local, null-tests
// and releases it (`dec [ecx+4] ; jne`; testing the member itself re-reads it after the decrement).
// The family index is a named local (`long family = CreatureFamilies.at(i);`): read straight into the GetDef call the
// loop body's scratch registers rotate (size edx / vtable eax / loop test ecx where retail has eax / edx / eax).
#include <stdio.h>
#include <stdlib.h>
template<class T> struct CRetailVector {        // STLport vector<T> (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() const { return _M_start; }
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    const T& at(unsigned long n) const
    {
        if (n >= size()) {
            puts("vector");
            abort();
        }
        return *(begin() + n);
    }
};
class CDefClassBase { public:
    virtual void Destroy();
    virtual void Release();                     // slot 1
    long RefCount;                              // +0x04
};
struct CCreatureFamilyDef : CDefClassBase {
    unsigned char _pad08[0x3c - 0x08];
    CRetailVector<long> Creatures;              // +0x3c
};
template<class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer()
    {
        T* object = Object;
        if (object && --object->RefCount == 0)
            object->Release();
    }
    T* operator->() const { return Object; }
};
class CDefinitionManager { public:
    bool GetDef(long globalIndex, CDefPointer<CCreatureFamilyDef>* out) const;   // 0x0074D220
};
void* Find_CDefinitionManager_GetDef(const void* owner);   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefinitionManager::GetDef(long globalIndex, CDefPointer<CCreatureFamilyDef>* out) const
{
    void* def = Find_CDefinitionManager_GetDef(this);
    if (!def)
        return false;
    out->Object = (CCreatureFamilyDef*)def;
    return true;
}
#pragma auto_inline(on)
class CTCCreatureGenerator { public:
    virtual void v00();
    void* ParentThing;                          // +0x04 (CTCBase)
    unsigned char _base08[4];
    CRetailVector<long> CreatureFamilies;       // +0x0c CArray<CDefIndex>
    const CDefinitionManager& GetDefinitionManager() const;   // 0x006869D0 (CTCBase)
    long GetNumGeneratedCreatures() const;
};

long CTCCreatureGenerator::GetNumGeneratedCreatures() const
{
    long total = 0;
    for (unsigned long i = 0; i < CreatureFamilies.size(); ++i) {
        CDefPointer<CCreatureFamilyDef> def;
        long family = CreatureFamilies.at(i);
        GetDefinitionManager().GetDef(family, &def);
        total += def->Creatures.size();
    }
    return total;
}