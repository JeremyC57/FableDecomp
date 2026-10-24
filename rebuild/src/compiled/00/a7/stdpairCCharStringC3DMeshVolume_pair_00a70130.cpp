// std::pair<CCharString,C3DMeshVolume>::~pair() -- destroy a named mesh volume entry: release the volume's counted convex volume, destroy its name, then the key string
// Leaf: ??1?$pair@VCCharString
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A70130, length 72 bytes, `jmp` tail. 0x00A6F390 is called from the
// introsort partition 0x00A71910 (a sort temporary of a vector<pair<CCharString,C3DMeshVolume>>); 0x00A70130 is the same
// destructor emitted in another TU (no references). The manifest label names the right element type.
// Layout: Ego_r.pdb C3DMeshVolume (0xc) = CCharString Name +0, CCountedPointer<CContainmentVolume> ConvexVolume +4; the pair
// is 0x10 (key +0, volume +4).
// Callees: CCPPointerInfo::DeleteFunc through the block (Data in ecx), operator delete @ 0x00BFE9BC (the block),
// CCharString::~CCharString @ 0x0099EAE0 (x2, the key is the tail jmp).
// Shape: the implicit pair destructor emitted through a qualified explicit destructor call; the inlined ~C3DMeshVolume keeps
// `&second` in esi (`lea esi,[edi+4]` scheduled between the Info test and its je) and ~CCountedPointer re-reads Info.
void __cdecl operator delete(void* p);                       // 0x00BFE9BC

class CCharString {
public:
    void* PStringData;
    ~CCharString();                                          // 0x0099EAE0
};

struct CCPPointerInfo {                                    // Ego_r.pdb: RefCount +0, DeleteFunc +4, Data +8
    unsigned long RefCount;
    void (__fastcall* DeleteFunc)(void* data);               // called with Data in ecx, nothing on the stack
    void* Data;
};
template<class T> struct CCountedPointer {
    T*              Data;                                    // +0x00
    CCPPointerInfo* Info;                                    // +0x04
    void ReleaseInfo()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
    }
    ~CCountedPointer() { ReleaseInfo(); Data = 0; Info = 0; }
};

struct CContainmentVolume;
struct C3DMeshVolume {                                       // 0xc bytes
    CCharString                         Name;                // +0x00
    CCountedPointer<CContainmentVolume> ConvexVolume;        // +0x04
};

namespace std {
template<class T1, class T2> struct pair {
    T1 first;
    T2 second;
};
}

typedef std::pair<CCharString, C3DMeshVolume> TNamedMeshVolume;

void DestroyNamedMeshVolume(TNamedMeshVolume* entry)         // emits the implicit destructor; not part of the retail body
{
    entry->TNamedMeshVolume::~TNamedMeshVolume();
}