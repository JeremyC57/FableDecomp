// CShaderDataBank::CloseShaders() -- delete every shader entry in the Shaders array, then empty the array
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A62050, 128 bytes (padding included).
// Leaf: CloseShaders
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Label correction: the catalog row is the propagated "CMeshDataBank::FreeDeviceSpecificResources";
// the landed CShaderDataBank deleting destructor 0x00A62860 calls this body on its Shaders vector
// (+0x110), and the PDB's private `void CShaderDataBank::CloseShaders()` is the matching member.
// Shape: indexed walk `for (i = 0; i < size(); ++i)` (unsigned, size re-read each step) with the
// element through STLport `*(begin() + n)`; a non-null entry is deleted through its virtual deleting
// destructor (`push 1 ; call [edx]`). clear() is the SGI erase(begin(), end()): __copy_trivial of the
// (empty) tail [end, finish) with its `last == first` shortcut (`cmp eax,eax` after CSE) else memmove
// (0x00BFEAE6), then finish = the copy's end. Retail bank layout per the landed 0x00A62860.
extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int count);   // 0x00BFEAE6

class CShaderBankEntry {
public:
    virtual ~CShaderBankEntry();                    // slot 0 (deleting destructor)
};

#include "engine/CArray.h"

class CShaderDataBank {
public:
    virtual void v00();
    unsigned char             _body[0x10c];         // CBankFile part
    CArray<CShaderBankEntry*> Shaders;              // +0x110
private:
    void CloseShaders();
};

void CShaderDataBank::CloseShaders()
{
    for (unsigned long i = 0; i < Shaders.size(); ++i) {
        CShaderBankEntry* entry = Shaders[i];
        if (entry)
            delete entry;
    }
    Shaders.clear();
}