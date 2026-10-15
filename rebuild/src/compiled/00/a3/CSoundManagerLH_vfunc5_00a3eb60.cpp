// CSoundManagerLH::vfunc_5(CSoundObjectLH* sound) -- calls the sound's slot 2, then removes every list entry (+0x4C) that holds it, deleting the sound through its virtual destructor each time
// Leaf: vfunc_5
// Retail: Fable.exe @ 0x00A3EB60, 82 bytes (`ret 4`; vtable 0x0129D2C4 slot 5).
// Callees: the sound's virtual slots 2 (+0x08, no arguments) and 0 (the scalar deleting destructor,
//          `push 1 ; call [edx]`), free 0x00BFEA14 (the STLport list node).
// Label: the manifest's "CTCTextureDecal::DeleteDecal" is propagated; rtti-vtables.tsv puts this body in
// slot 5 of CSoundManagerLH (the landed vfunc_6/vfunc_10 0x00A3EC70/0x00A3EC90 read the same 4-byte
// STLport list at +0x4C).
// Shape: the erase loop of CTCTextureDecal::DeleteDecal (0x0070A430) over list<CSoundObjectLH*>;
//        `delete sound` has NO null test because `sound` was dereferenced first (VC7.1 drops the
//        test after the slot-2 call); the sound stays in ebx across the loop.
#include <stdlib.h>

struct CSoundObjectLH {
    virtual ~CSoundObjectLH();                  // slot 0 (scalar deleting destructor)
    virtual void v01();
    virtual void Stop();                        // slot 2 (+0x08)
};

struct SSoundNode {                             // STLport _List_node<CSoundObjectLH*>
    SSoundNode* Next;
    SSoundNode* Prev;
    CSoundObjectLH* Value;                      // +0x08
};

struct CSTLportSoundList {                      // 4-byte STLport list: the sentinel pointer only
    SSoundNode* Node;
    SSoundNode* Begin() { return Node->Next; }
    SSoundNode* End() { return Node; }
    SSoundNode* Erase(SSoundNode* pos)
    {
        SSoundNode* next = pos->Next;
        SSoundNode* prev = pos->Prev;
        prev->Next = next;
        next->Prev = prev;
        free(pos);
        return next;
    }
};

struct CSoundManagerLH {                        // vtable 0x0129D2C4
    virtual void v00();
    unsigned char _pad_0x04[0x48];
    CSTLportSoundList m_entries;                // +0x4c
    void vfunc_5(CSoundObjectLH* sound);
};

void CSoundManagerLH::vfunc_5(CSoundObjectLH* sound)
{
    sound->Stop();
    for (SSoundNode* it = m_entries.Begin(); it != m_entries.End(); ) {
        if (it->Value == sound) {
            delete sound;
            it = m_entries.Erase(it);
        } else {
            it = it->Next;
        }
    }
}