// CTCWillLightning::StartLoopingSound() -- when no looping sound is playing, restart the lightning spell's looping sound through the parent's sound component (interface 16)
// Leaf: ?StartLoopingSound@CTCWillLightning@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007A0400, 135 bytes. Catalog: "CTCPhysicalShieldSpell::CancelForCutscene" (propagated; this
//   body reads CTCWillLightning members and sits between CTCWillLightning::Deactivate 0x007A0310 and ~CTCWillLightning code).
// Identity (FINDINGS chunk 395: "landed 0x7A0400 'ResumeForCutscene' is CTCWillLightning::StartLoopingSound"): CTCWillLightning
//   has no ResumeForCutscene member in Ego_r.pdb but the public pair `?StartLoopingSound@CTCWillLightning@@QAEXXZ` /
//   `?StopLoopingSound@CTCWillLightning@@QAEXXZ`; this body is the mirror of the landed StopLoopingSound 0x007A0490 (same
//   LoopingSound handle at retail +0x98, same interface-16 sound component: start when no handle vs stop and forget it), and
//   both are called from the lightning update code at 0x007A1B20.
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020 (inlined GetInterface, interface 16 = sound),
//   CDefStringTable::GetString(long) @ 0x009D49B0 on g_DefStringTable 0x013CA828, the sound component's PlaySound @ 0x0066BD00,
//   CCharString::~CCharString @ 0x0099EAE0. g_InvalidSoundHandle @ 0x0129D25C.
// Layout: PDB CTCWillLightning (0xf0): m_lspDef +0x28 (CDefPointer<CSpecialAbilitiesLightningSpellDef>), LoopingSound PDB +0xa4,
//   retail +0x98 (the PDB list/intelligent-pointer members are 12 bytes larger than retail's). Def LoopingSound (CDefString id)
//   at retail +0x114 (PDB +0x158).
// Shape: the sound component is a named local (esi); the string is a named local in its own block so it shares the interface
//   key's frame word; the handle is stored before the string's destructor.
class CCharString { public:
    void* PStringData;                          // +0 the shared string block
    ~CCharString();                             // 0x0099EAE0
};
struct CDefStringTable {
    CCharString GetString(long id);             // 0x009D49B0
};
extern CDefStringTable g_DefStringTable;        // 0x013CA828
extern unsigned long g_InvalidSoundHandle;      // 0x0129D25C
class CTCBase;
struct CTCSound {                               // the thing's interface-16 sound component
    unsigned long PlaySound(const CCharString& name, bool positional, long a, long b, long c);   // 0x0066BD00 (catalog "CSoundInfo::PlaySound")
};
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);       // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
};
class CSpecialAbilitiesLightningSpellDef { public:
    unsigned char _pad00[0x114];
    long LoopingSound;                          // +0x114 CDefString id
};
class CTCWillLightning { public:
    void*   __vftable;                          // +0x00 (CTCBase)
    CThing* ParentThing;                        // +0x04
    unsigned char _pad08[0x20];
    const CSpecialAbilitiesLightningSpellDef* m_lspDef;   // +0x28 CDefPointer
    unsigned char _pad2c[0x6c];
    unsigned long LoopingSound;                 // +0x98
    void StartLoopingSound();
};

void CTCWillLightning::StartLoopingSound()
{
    if (LoopingSound == g_InvalidSoundHandle) {
        CTCSound* sound;
        if (ParentThing->GetInterface(&sound, 16)) {
            CCharString name = g_DefStringTable.GetString(m_lspDef->LoopingSound);
            LoopingSound = sound->PlaySound(name, false, 0, 0, -1);
        }
    }
}