// std::list<CActiveDialogueInfo>::CreateNode(const CActiveDialogueInfo&) -- allocates a 0x20-byte list node and copy-constructs the element into it (links left unset)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006E4CB0, length 96 bytes.
// Callees: malloc @ 0x00BFEA0E (0x20 bytes: two links + the 0x18-byte element), CBaseIntelligentPointer::CBaseIntelligentPointer
// @ 0x00A01B10, CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CBaseIntelligentPointer::SetPItem @ 0x00A01B90.
// Label: the manifest's "CCreatureAction_JackFloatUp::Clone" is propagated nonsense -- `this` (ecx) is never read, the one
// stack argument is the element, and the result is the raw node (its +8 is null-checked: the placement new of a class with
// a constructor). The element is CActiveDialogueInfo (Ego_r.pdb: CIntelligentPointer<CThing> ThingSpeaking +0, ulong
// SoundID +8, EDialogueLayer Layer +0xc, bool IsLayerHead +0x10, float OutputGain +0x14 = 0x18 bytes): the vtable stored
// into the node is CIntelligentPointer's (0x01238C6C) and the four trailing members are copied one register each (the
// implicit memberwise copy; OutputGain is modelled as a dword because retail moves it with `mov eax`). Callers:
// 0x006E33E0 / 0x006E4E35 (the dialogue manager's list inserts). The retail list is SGI-shaped here (create-node then
// link, unlike the Dinkumware _Buynode(next, prev) families): modelled as a member of the list so `this` is reserved.
extern "C" void* __cdecl malloc(unsigned int size);
inline void* __cdecl operator new(unsigned int, void* where) { return where; }

class CBaseObject;

class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};

class CThing;
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // vtable 0x01238C6C
public:
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

struct CActiveDialogueInfo {                // sizeof 0x18 (Ego_r.pdb layout)
    CIntelligentPointer<CThing> ThingSpeaking;      // +0x00 CIntelligentPointer<CThing>
    unsigned long       SoundID;            // +0x08
    long                Layer;              // +0x0c EDialogueLayer
    unsigned char       IsLayerHead;        // +0x10 (bool)
    long                OutputGain;         // +0x14 (float; copied as a dword)
};

struct CRetailList_CActiveDialogueInfo {
    struct node { node* next; node* prev; CActiveDialogueInfo value; };   // sizeof 0x20
    node* head;
    node* CreateNode(const CActiveDialogueInfo& value);
};

CRetailList_CActiveDialogueInfo::node* CRetailList_CActiveDialogueInfo::CreateNode(const CActiveDialogueInfo& value)
{
    node* p = (node*)malloc(sizeof(node));
    new (&p->value) CActiveDialogueInfo(value);
    return p;
}