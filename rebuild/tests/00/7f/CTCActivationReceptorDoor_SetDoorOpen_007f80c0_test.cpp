// Behaviour test for CTCActivationReceptorDoor::SetDoorOpen @ 0x007F80C0: the flag reaches the parent thing's door component (interface 0x26).
#include <cstdio>

class CTCBase { public: int unused; };

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type)
    {
        SEntry* it = first;
        while (it != last && it->type < *type) ++it;
        return it;
    }
};

class CThing { public:
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    unsigned char m_flags91;
    template<class T> bool GetInterface(T** ppOut, long interfaceType)
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

// A thing carrying components of types 0x10, TYPE and 0x90 (sorted), with TYPE's presence bit set.
struct CTestThing : CThing {
    CInterfaceMap::SEntry entries[4];
    void Init(long type, CTCBase* component, CTCBase* sentinel)
    {
        for (int i = 0; i < 9; ++i) m_presentBits[i] = 0;
        m_flags91 = 0;
        entries[0].type = 0x10; entries[0].component = 0;
        entries[1].type = type;  entries[1].component = component;
        entries[2].type = 0x90; entries[2].component = 0;
        entries[3].type = 0x7fffffff; entries[3].component = sentinel;   // the map's end slot
        m_interfaces.first = entries;
        m_interfaces.last = entries + 3;
        m_presentBits[type >> 5] |= 1UL << (type & 31);
    }
};

class CTCDoor : public CTCBase { public:
    bool open; int calls;
    void ScriptTriggerOpen(bool o) { open = o; ++calls; }
};

struct CTCActivationReceptorDoor {
    CThing* ParentThing;
    void SetDoorOpen(bool open)
    {
        CTCDoor* door;
        ParentThing->GetInterface(&door, 0x26);
        door->ScriptTriggerOpen(open);
    }
};

int main()
{
    CTCDoor door; door.open = false; door.calls = 0;
    CTCDoor other; other.open = false; other.calls = 0;
    CTestThing thing; thing.Init(0x26, &door, &other);
    CTCActivationReceptorDoor receptor; receptor.ParentThing = &thing;
    receptor.SetDoorOpen(true);
    bool ok = door.open && door.calls == 1;
    receptor.SetDoorOpen(false);
    if (door.open || door.calls != 2 || other.calls != 0) ok = false;
    if (!ok) { std::printf("007f80c0_TEST FAIL\n"); return 1; }
    std::printf("007f80c0_TEST PASS\n");
    return 0;
}