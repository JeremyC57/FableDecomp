// CMeshBankManager::CMeshBankManager() -- default constructor.
// Leaf: ??0CMeshBankManager@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A0A110, length 57 bytes.
// Manifest label "CAIStateGroup_FlyingThingBase" is propagated; the vtables stored are 0x0129C98C (+0) and
// 0x0129C980 (+4), both CMeshBankManager (rtti-vtables.tsv): a class with two polymorphic bases.
// Callees: CBaseClassNonCopyable::CBaseClassNonCopyable @ 0x0099A2F0 (first base, out of line), malloc @ 0x00BFEA0E (the list head node).
// The second base's constructor is inline and only stores its vtable 0x0122DE50 at +4.
//
// Shape: base #1 ctor, base #2 vtable, then both derived vtables, then the member at +8: a VC6-era
// Dinkumware std::list whose constructor nulls the head, allocates a 0x14-byte sentinel node through
// the allocator (malloc) and links it to itself (`next = prev = node`); the size counter is not stored
// here (the list's element type is 12 bytes: 0x14 - two links).
extern "C" void* __cdecl malloc(unsigned int size);

class CBaseClassNonCopyable {
protected:
    virtual ~CBaseClassNonCopyable();   // 0x0099A300
    CBaseClassNonCopyable();                                    // 0x0099A2F0
};

struct CMeshBankListener {                      // second base, vtable 0x0122DE50
    virtual void v00();
    CMeshBankListener() { }
};

struct CRetailList0x14 {                        // std::list<T> with 12-byte T: {next, prev, value[12]} nodes
    struct SNode { SNode* next; SNode* prev; unsigned char value[12]; };
    SNode* head;                                // +0x00 sentinel
    long size;                                  // +0x04 (not written by this constructor)
    CRetailList0x14() : head(0)
    {
        SNode* node = (SNode*)malloc(0x14);     // built in a local: retail stores the head LAST
        node->next = node;
        node->prev = node;
        head = node;
    }
};

class CMeshBankManager : public CBaseClassNonCopyable, public CMeshBankListener { public:
    CRetailList0x14 m_banks;                    // +0x08
    CMeshBankManager();
};

CMeshBankManager::CMeshBankManager()
    : CBaseClassNonCopyable()
    , CMeshBankListener()
    , m_banks()
{
}