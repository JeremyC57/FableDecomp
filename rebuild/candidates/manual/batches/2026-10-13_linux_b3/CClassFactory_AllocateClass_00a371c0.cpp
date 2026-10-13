// CClassFactory::AllocateClass(CCharString*)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A371C0, oracle length 42 bytes.
// Grade: candidate -- graded under the Linux lane (Wine + DDK 13.10.4035 cl.exe), see batch README.
//
// Retail bytes:  mov eax,[esp+4] ; push esi ; push eax ; mov esi,ecx ; call CClassFactory::GetClassNumberFromName ; mov esi,[esi+4] ; lea eax,[eax+eax*4] ; shl eax,2 ; mov ecx,[eax+esi+0Ch] ; test ecx,ecx ; je Z ; call ecx ; pop esi ; ret 4 ; Z: xor eax,eax ; pop esi ; ret 4
// engine/CClassFactory.h (PDB): +0x04 vector<CClassTypeInfo> ClassTypeInfo (retail: `first` at
// +0x04). Each 20-byte CClassTypeInfo carries its allocator function at +0x0C. The class name is
// resolved to a type number (0x00A37060, __thiscall) and that entry's allocator, if registered,
// creates the instance; an unregistered type yields null.
//
// Lane feedback: STILL DIFFER(40v42). The element address now matches (`lea ; shl ; [eax+esi+0Ch]`) but retail
// keeps `pop esi` inside each exit after the indirect call where our build hoists it above the
// branch; no flag/pragma in the sweep changes that. Open: see the batch README.
template<class T> struct CRetailVector {
    T* first; T* last; T* end_of_storage;
    struct iterator { T* p; explicit iterator(T* q) : p(q) {} iterator operator+(long n) const { return iterator(p + n); } T& operator*() const { return *p; } };
    iterator begin() { return iterator(first); }
    unsigned long size() const { return (unsigned long)(last - first); }
    T& operator[](unsigned long n) { return *(begin() + n); }
};

struct CClass;
struct CCharString;

struct CClassTypeInfoEntry {
    unsigned char _pad_0x00[0x0C];
    CClass* (__cdecl* Allocate)();     // +0x0C allocator callback
    unsigned char _pad_0x10[4];        // 20-byte stride
};

// Retail CClassFactory view: the type-info vector (12 bytes) starts at +0x04.
struct CClassFactory_Retail {
    void*                               __vftable;      // +0x00
    CRetailVector<CClassTypeInfoEntry>  ClassTypeInfo;  // +0x04
    long GetClassNumberFromName(CCharString* name);     // 0x00A37060
    CClass* AllocateClass(CCharString* name);
};

CClass* CClassFactory_Retail::AllocateClass(CCharString* name)
{
    // Resolve the name to a type number and call that entry's allocator if one is registered.
    CClassTypeInfoEntry& entry = this->ClassTypeInfo[GetClassNumberFromName(name)];
    if (entry.Allocate)
        return entry.Allocate();
    return 0;
}
