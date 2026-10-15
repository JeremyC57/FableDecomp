// CAnimationEntry::TransferBinaryOut(CDataOutputStream& stream) const -- binary save of an animation entry: bank index, the two def-string table positions, the component count, then each component's type id followed by the component's own binary save
// Retail: Fable.exe (TLC, VC7.1) @ 0x005DB630, 127 bytes (+1 int3), __fastcall, ret 4.
// Callees: CDataOutputStream::WriteLong 0x00993EB0, CDataOutputStream::WriteULong 0x00993EA0, and the
// component's virtual TransferBinaryOut (slot 7, `call [edx+1Ch]`; slot 6 is TransferBinaryIn, cf.
// CAnimComponentStrikeSpeedMultiplier's vtable 0x01255800).
// Layout: PDB CAnimationEntry (0x10, as the landed operator= 0x00471A09): BankIndex, CDefString AnimName,
// CDefString GroupName, CIVCountedPointer<CAnimComponentMap> Components (+0x0C). The component map is an
// intrusive counted pointee (vptr, count) holding a retail vector of 8-byte {EAnimComponent,
// CIVCountedPointer<CAnimComponentBase>} pairs at +8 (begin) / +0x0C (end).
// Shape: the count is a ternary over the null map (`(end-begin) >> 3`, sar), written as an unsigned
// long; the walk only runs for a non-zero count and re-reads the map pointer and its end after every
// virtual call. Both pair fields are read before the first call: the component pointer is a local
// (ebx) taken from the element ahead of the type-id write.
class CDataOutputStream {
public:
    void WriteLong(long value);                  // 0x00993EB0
    void WriteULong(unsigned long value);        // 0x00993EA0
};

class CAnimComponentBase {
public:
    virtual void v00();
    virtual void Destroy();                      // slot 1
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void TransferBinaryIn(void* stream); // slot 6
    virtual void TransferBinaryOut(CDataOutputStream& stream) const;   // slot 7
    long m_refCount;                             // +0x04
};

template<class T> struct CIVCountedPointer {     // intrusive counted pointer (4 bytes)
    T* m_pObject;
    T* operator->() const { return m_pObject; }
};

enum EAnimComponent { ANIM_COMPONENT_FLAGS = 0 };

struct CAnimComponentEntry {                     // pair<EAnimComponent, CIVCountedPointer<CAnimComponentBase> >
    EAnimComponent first;
    CIVCountedPointer<CAnimComponentBase> second;
};

struct CAnimComponentMap {
    void* m_pVtable;                             // +0x00
    long m_refCount;                             // +0x04
    CAnimComponentEntry* m_pFirst;               // +0x08
    CAnimComponentEntry* m_pLast;                // +0x0C
    CAnimComponentEntry* m_pEnd;                 // +0x10
    long Size() const { return m_pLast - m_pFirst; }
};

struct CDefString { long TablePos; };

struct CAnimationEntry {
    long BankIndex;                              // +0x00
    CDefString AnimName;                         // +0x04
    CDefString GroupName;                        // +0x08
    CIVCountedPointer<CAnimComponentMap> Components;   // +0x0c
    void TransferBinaryOut(CDataOutputStream& stream) const;
};

void CAnimationEntry::TransferBinaryOut(CDataOutputStream& stream) const
{
    stream.WriteLong(BankIndex);
    stream.WriteLong(AnimName.TablePos);
    stream.WriteLong(GroupName.TablePos);
    unsigned long count = Components.m_pObject ? Components->Size() : 0;
    stream.WriteULong(count);
    if (count) {
        for (CAnimComponentEntry* it = Components->m_pFirst; it != Components->m_pLast; ++it) {
            CAnimComponentBase* component = it->second.m_pObject;
            stream.WriteLong(it->first);
            component->TransferBinaryOut(stream);
        }
    }
}