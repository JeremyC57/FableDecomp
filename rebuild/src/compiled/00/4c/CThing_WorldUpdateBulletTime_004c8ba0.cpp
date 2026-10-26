// CThing::WorldUpdateBulletTime() -- the bullet-time world update: if the thing carries objects (interface 0x46) update the carried objects, then give every component on the update list its bullet-time update
// Retail: Fable.exe (TLC, VC7.1) @ 0x004C8BA0, 91 bytes. CThing vtable 0x01239ABC slot 20.
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020 (the inlined GetInterface lookup),
// CTCCarrying::UpdateCarriedObjects @ 0x006A1D60, CTCBase virtual slot 10 (+0x28) on each live component of
// UpdateComponentsList.
// Shape: the landed GetInterface out-parameter template (0x007504E0 / 0x008386E0): interface 0x46 = bitset
// word 2 bit 6 (`test byte ptr [edi+28h],40h`), key in the `push ecx` slot, the map as a named pointer
// local. UpdateComponentsList is the 4-byte STLport list at retail +0x54 (its node pointer; the loop walks
// node->next until it is back at the node, the CTCBase* at node +8).
class CTCBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09();
    virtual void UpdateBulletTime();                 // slot 10 (+0x28)
    void* ParentThing;                               // +0x04
    bool  ToBeRemoved;                               // +0x08
};

class CTCCarrying { public:
    void UpdateCarriedObjects();                     // 0x006A1D60
};

struct CInterfaceMap {                               // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);            // 0x0040F020
};

struct SComponentListNode {
    SComponentListNode* m_next;                      // +0
    SComponentListNode* m_prev;                      // +4
    CTCBase*            m_data;                      // +8
};

class CThing { public:
    void*               __vftable;                   // +0x00
    unsigned char       _pad04[0x20 - 0x04];
    unsigned long       UsingComponents[9];          // +0x20 bitset<274>
    CInterfaceMap       Components;                  // +0x44
    unsigned char       _pad4c[0x54 - 0x4c];
    SComponentListNode* UpdateComponentsList;        // +0x54 STLport list node (PDB +0x5c)
    template<class T> bool GetInterface(T** ppOut, long interfaceType)
    {
        if (UsingComponents[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &Components;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
    void WorldUpdateBulletTime();
};

void CThing::WorldUpdateBulletTime()
{
    CTCCarrying* carrying;
    if (GetInterface(&carrying, 0x46))
        carrying->UpdateCarriedObjects();
    for (SComponentListNode* node = UpdateComponentsList->m_next; node != UpdateComponentsList; node = node->m_next) {
        CTCBase* tc = node->m_data;
        if (!tc->ToBeRemoved)
            tc->UpdateBulletTime();
    }
}