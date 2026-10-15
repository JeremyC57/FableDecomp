// CThing::ValidateThing(long context) -- ask every live thing component to validate itself; false as soon as one refuses, true otherwise
// Retail: Fable.exe (TLC, VC7.1) @ 0x004C7470, 63 bytes. CThing vtable 0x01239ABC slot 18.
// Callees: CTCBase virtual slot 17 (+0x44) on each component not marked ToBeRemoved.
// Shape: Components (CVectorMap<ETCInterfaceType, CTCBase*>) is a vector of 8-byte {type, component} pairs at retail
// +0x44/+0x48 (PDB +0x48: the retail CThing is narrower); the loop re-reads the last pointer after every call
// and loads the argument once into ebx after the entry test. `mov al,1` / `xor al,al` = bool.
struct CTCBase;

struct SComponentPair {                              // pair<ETCInterfaceType, CTCBase*>
    long     first;
    CTCBase* second;
};

struct CTCBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16();
    virtual bool ValidateTC(long context);           // slot 17 (+0x44)
    void* ParentThing;                               // +0x04
    bool  ToBeRemoved;                               // +0x08
};

struct CThing {
    void*           __vftable;                       // +0x00
    unsigned char   _pad04[0x44 - 0x04];
    SComponentPair* m_componentsFirst;               // +0x44 Components
    SComponentPair* m_componentsLast;                // +0x48
    bool ValidateThing(long context);
};

bool CThing::ValidateThing(long context)
{
    for (SComponentPair* it = m_componentsFirst; it != m_componentsLast; ++it) {
        CTCBase* tc = it->second;
        if (!tc->ToBeRemoved && !tc->ValidateTC(context))
            return false;
    }
    return true;
}
