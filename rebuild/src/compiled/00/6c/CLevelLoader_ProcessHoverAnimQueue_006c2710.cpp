// CLevelLoader::ProcessHoverAnimQueue() -- when the loader's pending-object list is not empty: show the "Level loader update" progress text, run the front object's Load step with this loader as the suspendable process, show "Level loader update end" and pop the object off the list
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006C2710, length 144 bytes.
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0, DisplayProgress @ 0x009E9F40 (`__fastcall`:
//          string in ecx, the first bool in dl, the float and the second bool on the stack),
//          CCharString::~CCharString @ 0x0099EAE0, CLevelLoader::CObject::Load(CASuspendableProcess*) @ 0x006C2170,
//          the list's erase(iterator) @ 0x006C2BA0 (iterator by value, result by hidden pointer).
// Shape: the retail list (+0x14) is the single sentinel pointer of the landed constructor 0x006C26B0; the empty
// test is `head->next == head`. The two CCharString named locals (each in its own block, built before the pushes) and erase's discarded result share the one
// `push ecx` frame word; the by-value iterator has user constructors so it is built in the argument area
// (`push ecx ; mov eax,esp ; mov [eax],ecx`). The catalog name is kept; the body is the loader's update step.
class CCharString {
public:
    CCharString(const char* text, long length);    // 0x0099EBF0
    ~CCharString();                                // 0x0099EAE0
private:
    void* m_pData;
};
void __fastcall DisplayProgress(const CCharString& text, bool flag, float fraction, bool flag2);   // 0x009E9F40
struct CASuspendableProcess { virtual void v00(); };
struct SCountBlock;
struct CLevelLoaderObject {
    void Load(CASuspendableProcess* process);      // 0x006C2170
};
struct CCountedPointer_CObject {                   // {object, count block}
    CLevelLoaderObject* m_pObject;
    SCountBlock*        m_pCount;
    CLevelLoaderObject* operator->() const { return m_pObject; }
};
struct CObjectList {                               // retail list: one sentinel-node pointer
    struct node { node* next; node* prev; CCountedPointer_CObject value; };
    struct iterator {
        node* m_pNode;
        iterator(node* n) : m_pNode(n) { }
        iterator(const iterator& other) : m_pNode(other.m_pNode) { }
    };
    node* head;
    bool empty() const { return head->next == head; }
    iterator begin() { return iterator(head->next); }
    CCountedPointer_CObject& front() { return head->next->value; }
    iterator erase(iterator where);                // 0x006C2BA0
};
struct CLevelLoader : CASuspendableProcess {
    unsigned char _pad04[0x10];
    CObjectList   ObjectList;                      // +0x14
    void ProcessHoverAnimQueue();
};

void CLevelLoader::ProcessHoverAnimQueue()
{
    if (!ObjectList.empty())
    {
        {
            CCharString text("Level loader update", -1);
            DisplayProgress(text, false, -1.0f, false);
        }
        ObjectList.front()->Load(this);
        {
            CCharString text("Level loader update end", -1);
            DisplayProgress(text, false, -1.0f, false);
        }
        ObjectList.erase(ObjectList.begin());
    }
}