// std::swap(std::pair<unsigned long,C3DMeshVolume>& a, std::pair<unsigned long,C3DMeshVolume>& b) -- exchange two (unsigned long key, named mesh volume) pairs through a temporary, releasing the temporary's counted convex volume afterwards
// Leaf: ??$swap@U?$pair@KVC3DMeshVolume@@@std@@@std@@YIXAAU?$pair@KVC3DMeshVolume@@@0@0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A331B0, length 152 bytes (cut 160 with padding), plain `ret` (a in ecx, b in
//   edx -- the SGI algorithm TUs are /Gr). Callers: the introsort/partition helpers at 0x00A33860 (tail `jmp`) and 0x00A34296.
//   Byte twin in another TU: 0x00A70320 (different rel32s only).
// Identity: the row's `_Pop_heap_0<pair<long,C3DMeshVolume>*,...>` label names the element type but not its key; the function is
//   std::swap (no heap code, no comparator). Layout per the landed 0x00A6F390: Ego_r.pdb C3DMeshVolume (0xC) = CCharString
//   Name +0, CCountedPointer<CContainmentVolume> ConvexVolume +4 (Data, Info); the pair is 0x10 (key +0, volume +4). The
//   volume's assignment is the out-of-line 0x00A32960 (the landed 0x00A34420 / 0x00A71B40 call it as the record assignment).
// Callees: CCharString::CCharString(const CCharString&) @ 0x0099EC30, C3DMeshVolume::operator= @ 0x00A32960 (x2),
//   the info block's DeleteFunc (Data in ecx), operator delete @ 0x00BFE9BC, CCharString::~CCharString @ 0x0099EAE0.
// Shape: STLport `_Tp __tmp = __a; __a = __b; __b = __tmp;` with the pair's explicit copy constructor (key, string by call,
//   counted pointer inline: copy both words, add-ref) and implicit assignment (key inline, volume out of line) inlined, then
//   the temporary's implicit destructor: the counted pointer's release (info re-read), its zero stores, the name's destructor.
// Key: unsigned long -- this is the C3DMeshStats::MeshVolumes instantiation (Ego_r.pdb CVectorMap<unsigned long,
//   C3DMeshVolume, CKeyPairCompareLess<unsigned long,C3DMeshVolume> >, pair@K in the PDB), and its callers 0x00A33860 /
//   0x00A34260 compare the key unsigned (jae/jb). The signed-key pair<long,C3DMeshVolume> is the byte twin 0x00A70320.
void __cdecl operator delete(void* p);                  // 0x00BFE9BC

class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};

struct CCPPointerInfo {                                 // Ego_r.pdb: RefCount +0, DeleteFunc +4, Data +8
    unsigned long RefCount;
    void (__fastcall* DeleteFunc)(void* data);          // called with Data in ecx, nothing on the stack
    void* Data;
};

template <class T> struct CCountedPointer {
    T*              Data;                               // +0x00
    CCPPointerInfo* Info;                               // +0x04
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->RefCount;
    }
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

class C3DMeshVolume {                                   // Ego_r.pdb, 0xC bytes (a class: the PDB mangles V)
public:
    CCharString                         Name;           // +0x00
    CCountedPointer<CContainmentVolume> ConvexVolume;   // +0x04
    C3DMeshVolume& operator=(const C3DMeshVolume& other);   // 0x00A32960 (out of line)
};

namespace std {
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const pair& other) : first(other.first), second(other.second) { }
};

template <class T> inline void __fastcall swap(T& a, T& b)
{
    T tmp = a;
    a = b;
    b = tmp;
}

template void __fastcall swap<pair<unsigned long, C3DMeshVolume> >(pair<unsigned long, C3DMeshVolume>& a, pair<unsigned long, C3DMeshVolume>& b);
}