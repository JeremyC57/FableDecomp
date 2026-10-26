// std::fill(pair<unsigned long, CAnimationEntry>* first, pair<unsigned long, CAnimationEntry>* last, const pair<unsigned long, CAnimationEntry>& val) -- assign val to every keyed animation entry of [first, last)
// Leaf: fill
// Retail: Fable.exe @ 0x005E1820, 97 bytes, `ret 4`: __fastcall (first ecx -> ebx, last edx -> ebp, the value reference
// on the stack -> edi). Only call site 0x005E3402.
// Types (Ego_r.pdb): CAnimationEntry (16 bytes) = BankIndex +0, AnimName CDefString +4, GroupName CDefString +8,
//   Components CIVCountedPointer<CAnimComponentMap> +0xC; the pair is 20 bytes (first unsigned long +0, second +4).
// Callees: the component map's virtual destroy (vtable slot 1) when its reference count (+4) drops to zero.
// Shape: the pair's implicit copy assignment: the key, BankIndex, the two one-word CDefStrings (trivial copies), then the
// intrusive pointer's operator= (self test against the source, release through the local copy, store, count bump).
// VC7.1 keeps a second induction register at the Components member (`lea esi,[ebx+10h]`; the words before it are
// stored at [esi-0Ch] / [esi-8] / [esi-4]).
// Local base model, not engine/CIVCountedPointeeBase.h: the generated header flattens the vptr into a plain
// `void* __vftable`, which cannot make the slot-1 virtual destroy call the release runs.
class CIVCountedPointeeBase { public:
    virtual void v00();
    virtual void Destroy();                     // slot 1
    long m_count;                               // +0x04
};

template <class T>
class CIVCountedPointer { public:
    T* Object;                                  // +0x00
    CIVCountedPointer& operator=(const CIVCountedPointer& other)
    {
        if (Object != other.Object) {
            T* object = Object;
            if (object) {
                if (--object->m_count == 0)
                    object->Destroy();
                Object = 0;
            }
            Object = other.Object;
            if (Object)
                ++Object->m_count;
        }
        return *this;
    }
};

class CDefString { public:                             // one word (Ego_r.pdb: TablePos)
    long TablePos;                              // +0x00
};
class CAnimComponentMap : public CIVCountedPointeeBase { public: };
class CAnimationEntry { public:                        // 16 bytes
    long                                 BankIndex;     // +0x00
    CDefString                           AnimName;      // +0x04
    CDefString                           GroupName;     // +0x08
    CIVCountedPointer<CAnimComponentMap> Components;    // +0x0C
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
typedef pair<unsigned long, CAnimationEntry> CAnimationEntryPair;
template void __fastcall fill(CAnimationEntryPair*, CAnimationEntryPair*, const CAnimationEntryPair&);
}