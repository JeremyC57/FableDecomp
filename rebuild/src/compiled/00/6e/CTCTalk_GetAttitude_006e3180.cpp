// CTCTalk::GetAttitude() -- conversation attitude: 10 while the parent's interface-49 component is in mode type 0x56, else the stored Attitude
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006E3180, length 86 bytes.
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x004365B0 (the thing's interface map at +0x44), the
// component's current-mode getter @ 0x0073A6E0 (`[[this+0Ch]]+8`; manifest "CPixelShader::CountReferences" is a
// propagated name), then the mode's virtual slot 1 (its type id).
// Ego_r.pdb CTCTalk: ParentThing +0x04 (CTCBase), Attitude (EConversationAttitude) +0x18.
// Shape: CThing::GetInterface(T** out, type) is the inline template of the landed 0x007504E0 / 0x007B2CF0 family:
// presence bit `test [thing+24h],20000h` (word 1, bit 17 = interface 49), key 31h in the `push ecx` frame slot,
// `LowerBound(&key)`, the `it == last || type < it->type ? last : it` select and `*out = it->component`. The 10 is
// materialised before the `je` (`mov eax,0Ah`) and the fall-through reloads Attitude.
class CTCBase;

struct CMode {
    virtual void v00();
    virtual long GetModeType();              // slot 1
};
struct CModeComponent {                      // the interface-49 component
    CMode* GetCurrentMode();                 // 0x0073A6E0
};

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                       // CVectorMap<ETCInterfaceType, CTCBase*>
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const ETCInterfaceType& type) const;    // 0x004365B0
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];          // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;              // +0x44
    template<class T> bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
};

typedef long EConversationAttitude;

class CTCTalk { public:
    virtual void v00();
    CThing*       ParentThing;               // +0x04
    unsigned char _pad_0x08[0x10];           // flags, SpeechSample .. SampleDurationFrames
    EConversationAttitude Attitude;          // +0x18
    EConversationAttitude GetAttitude();
};

EConversationAttitude CTCTalk::GetAttitude()
{
    CModeComponent* modes;
    if (ParentThing->GetInterface(&modes, 0x31)) {
        if (modes->GetCurrentMode()->GetModeType() == 0x56)
            return 10;
    }
    return Attitude;
}