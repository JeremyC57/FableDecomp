// CAttachingThingsDialog::SetAttachingThingTypeName(long type, CCharString name) -- records the display name for an
// attaching-thing type in the dialog's name map (map<CCharString, long>: the name is the key, the type the value).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006EAB60, length 71 bytes.
// Callees: CCharString::CCharString(const CCharString&) 0x0099EC30 (the pair's key), CCharStringMap::insert
//          0x006F2940 (pair<iterator,bool> through the hidden result pointer), CCharString::~CCharString 0x0099EAE0
//          (the pair's key, then the by-value `name` parameter -- destroyed by the callee).
// Frame: 0x10 bytes = the value pair {CCharString, long} at +4/+8 and the 8-byte insert result at +0xc. The map is
// the member at +4 (`lea ecx,[esi+4]`). The by-value CCharString argument is addressed in its own slot (`lea
// eax,[esp+1Ch]`), the signature of a 4-byte class with a user copy constructor passed by value.
class CCharString { public:
    void* PStringData;
    CCharString(const CCharString& o);                   // 0x0099EC30
    ~CCharString();                                      // 0x0099EAE0
};

struct SNamePair {
    CCharString first;
    long second;
    SNamePair(const CCharString& key, long value) : first(key), second(value) { }
};

struct SInsertResult {
    void* where;                                         // iterator (node pointer)
    bool inserted;
    SInsertResult() { }
};

struct CCharStringMap {
    void* head;                                          // retail std::map: the head-node pointer
    SInsertResult insert(const SNamePair& value);        // 0x006F2940
};

struct CAttachingThingsDialog {
    void* __vftable;
    CCharStringMap TypeNames;                            // +4
    void SetAttachingThingTypeName(long type, CCharString name);
};

void CAttachingThingsDialog::SetAttachingThingTypeName(long type, CCharString name)
{
    TypeNames.insert(SNamePair(name, type));
}