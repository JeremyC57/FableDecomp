// std::pair<long,C3DMeshVolume>::~pair() -- destroy an indexed mesh volume entry: release the volume's counted convex volume, then destroy its name
// Leaf: ??1?$pair@JUC3DMeshVolume
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A326D0, length 61 bytes, `jmp` tail. Callers: 0x00A350BA / 0x00A35114 in the
// vector insert helper 0x00A35010, both on a 0x10-byte stack temporary (`lea ecx,[esp+0x10]`, `add esp,0x10`) -- the
// element of a vector<pair<long,C3DMeshVolume>> (the neighbouring 0x00A70320 is its _Pop_heap). The manifest label
// (_Dest_val<allocator<pair<CCharString const,CCountedPointer<C3DSkeletalMorphTarget>>>>) is propagated and wrong.
// Layout: Ego_r.pdb C3DMeshVolume (0xc) = CCharString Name +0, CCountedPointer<CContainmentVolume> ConvexVolume +4; the pair
// is 0x10 (long key +0, volume +4).
// Callees: CCPPointerInfo::DeleteFunc through the block (Data in ecx), operator delete @ 0x00BFE9BC (the block),
// CCharString::~CCharString @ 0x0099EAE0 (tail).
// Shape: the long key needs no destruction, so `this` is never saved: Info is read straight off ecx, `&second` goes to esi
// and the volume's name is the tail jmp.
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

typedef std::pair<long, C3DMeshVolume> TIndexedMeshVolume;

void DestroyIndexedMeshVolume(TIndexedMeshVolume* entry)     // emits the implicit destructor; not part of the retail body
{
    entry->TIndexedMeshVolume::~TIndexedMeshVolume();
}