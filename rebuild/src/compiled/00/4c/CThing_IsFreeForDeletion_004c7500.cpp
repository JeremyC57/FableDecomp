// CThing::IsFreeForDeletion() -- a thing may be deleted only when every live thing component agrees (false as soon as one refuses)
// Retail: Fable.exe (TLC, VC7.1) @ 0x004C7500, 55 bytes. Callees: CTCBase virtual slot 18 (+0x48) on each
// component not marked ToBeRemoved.
// Shape: the 0x004C7470 ValidateThing loop without the argument: Components {type, component} pairs at retail
// +0x44/+0x48, last pointer re-read after each call, aligned loop head (`lea esp,[esp]`). bool result.
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
    virtual void v16(); virtual void v17();
    virtual bool IsFreeForDeletion();                // slot 18 (+0x48)
    void* ParentThing;                               // +0x04
    bool  ToBeRemoved;                               // +0x08
};

struct CThing {
    void*           __vftable;                       // +0x00
    unsigned char   _pad04[0x44 - 0x04];
    SComponentPair* m_componentsFirst;               // +0x44 Components
    SComponentPair* m_componentsLast;                // +0x48
    bool IsFreeForDeletion();
};

bool CThing::IsFreeForDeletion()
{
    for (SComponentPair* it = m_componentsFirst; it != m_componentsLast; ++it) {
        CTCBase* tc = it->second;
        if (!tc->ToBeRemoved && !tc->IsFreeForDeletion())
            return false;
    }
    return true;
}