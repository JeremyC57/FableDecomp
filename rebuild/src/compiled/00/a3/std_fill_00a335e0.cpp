// std::fill(pair<long, C3DMeshVolume>* first, pair<long, C3DMeshVolume>* last, const pair<long, C3DMeshVolume>& val) -- assign val to every keyed mesh volume of [first, last)
// Leaf: fill
// Retail: Fable.exe @ 0x00A335E0, 144 bytes, `ret 4`: __fastcall (first ecx -> ebp, last edx -- spilled to a frame word --
// and the value reference on the stack).
// Types (Ego_r.pdb): C3DMeshVolume (12 bytes) = Name CCharString +0, ConvexVolume CCountedPointer<CContainmentVolume>
//   (Data +4, Info +8); the pair is 16 bytes (first long +0, second +4).
// Callees: CCharString::operator=(const CCharString&) @ 0x0099EFB0, operator delete @ 0x00BFE9BC (the count block).
// Shape: the pair's implicit copy assignment is memberwise: the key (the value reference re-read from its stack slot,
// `mov eax,[esp+1Ch]`), Name's operator= call (argument = &val.second, hoisted into ebx), then the inlined
// CCountedPointer operator=: both source words read into locals before the block compare (Data spilled to the frame),
// the old block released (count decrement, re-read, destroy hook with the object in ecx, delete), the new words stored
// and the new block's count bumped. The count-block address is a second induction register (`lea esi,[ebp+0Ch]`).
struct SCountBlock {
    long m_references;                          // +0x00
    void (__fastcall* m_pfnDestroy)(void*);     // +0x04
    void* m_pObject;                            // +0x08
};

template <class T>
class CCountedPointer { public:
    T* Data;                                    // +0x00
    SCountBlock* Info;                          // +0x04
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        SCountBlock* info = other.Info;         // both source words first (retail: edi / the frame word)
        T* data = other.Data;
        if (Info != info) {
            if (Info) {
                --Info->m_references;
                if (Info->m_references == 0) {
                    Info->m_pfnDestroy(Info->m_pObject);
                    delete Info;
                }
            }
            Data = data;
            Info = info;
            if (info)
                ++info->m_references;
        }
        return *this;
    }
};

class CCharString { public:
    void* m_pStringData;                        // +0x00
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

class CContainmentVolume;
class C3DMeshVolume { public:                          // 12 bytes
    CCharString                         Name;           // +0x00
    CCountedPointer<CContainmentVolume> ConvexVolume;   // +0x04
};

namespace std {
template <class T1, class T2>
struct pair {
    T1 first;
    T2 second;
};
template <class ForwardIter, class T>
void __fastcall fill(ForwardIter first, ForwardIter last, const T& val)
{
    for (; first != last; ++first)
        *first = val;
}
typedef pair<long, C3DMeshVolume> CMeshVolumeEntry;
template void __fastcall fill(CMeshVolumeEntry*, CMeshVolumeEntry*, const CMeshVolumeEntry&);
}