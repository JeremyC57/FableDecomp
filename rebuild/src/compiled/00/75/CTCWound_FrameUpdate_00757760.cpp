// CTCWound::FrameUpdate() -- update every wound against the parent's texture-decal component and erase the dead ones
// Leaf: FrameUpdate
// Retail: Fable.exe (TLC, VC7.1) @ 0x00757760, 132 bytes (+12 int3). __fastcall, `ret`. CTCWound vtable 0x012698B4 slot 9.
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x0040F020; CTCWound::CWound::Update(CTCTextureDecal*)
//          0x00757570; CTCWound::CWound::Kill(CTCTextureDecal*) 0x007573C0; free 0x00BFEA14 (the list node).
// Shape: interface 0xE9 (CTCTextureDecal, bitset word 7 bit 9 -> `test ch,2` on +0x3C) fetched into ebx; the wound list
//        is walked with end() re-read each pass; a dead wound is killed and erased through the inline STLport list erase
//        (next/prev relink, trivially destroyed CWound, node freed) which returns the next node.
// Layout: CTCWound (PDB, 0x1C with a 12-byte list): ParentThing +0x04, LastFrameChanged +0x0C, Wounds +0x10 (retail STLport
//         list = one sentinel pointer); CWound (0x44): Dead +0x40; retail CThing bitset +0x20, interface map +0x44.
class CTCBase;
struct CInterfaceMap {                          // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44: {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020 (another TU's copy is 0x004365B0)
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char Flags91;                      // +0x91 bit 0 Dead
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
extern "C" void __cdecl free(void* p);           // 0x00BFEA14
class CTCTextureDecal;
struct CWound {
    unsigned char _pad_0x00[0x40];               // Desc (CEntry), Duration, decal entry IDs, MorphIndex
    bool Dead;                                   // +0x40
    bool HadUpdate;                              // +0x41
    void Update(CTCTextureDecal* decal);         // 0x00757570
    void Kill(CTCTextureDecal* decal);           // 0x007573C0
};
template <class T> struct CRetailList {          // STLport list: the sentinel node pointer only
    struct Node { Node* Next; Node* Prev; T Value; };
    Node* Head;
    Node* begin() const { return Head->Next; }
    Node* end() const { return Head; }
    Node* erase(Node* pos)
    {
        Node* next = pos->Next;
        Node* prev = pos->Prev;
        prev->Next = next;
        next->Prev = prev;
        free(pos);
        return next;
    }
};
class CTCWound { public:
    virtual void v00();
    CThing* ParentThing;                         // +0x04
    unsigned char _pad_0x08[4];
    unsigned long LastFrameChanged;              // +0x0c
    CRetailList<CWound> Wounds;                  // +0x10
    void FrameUpdate();
};
void CTCWound::FrameUpdate()
{
    CTCTextureDecal* decal;
    if (ParentThing->GetInterface(&decal, 0xe9)) {
        for (CRetailList<CWound>::Node* it = Wounds.begin(); it != Wounds.end(); ) {
            it->Value.Update(decal);
            if (it->Value.Dead) {
                it->Value.Kill(decal);
                it = Wounds.erase(it);
            } else {
                it = it->Next;
            }
        }
    }
}