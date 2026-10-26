// NCreatureMode::CModeThunderLightningStorm::ResumeForCutscene() -- when no looping sound is playing, restart the storm's looping sound through the owner's sound component (interface 16)
// Leaf: ResumeForCutscene
// Retail: Fable.exe (TLC, VC7.1) @ 0x008DA990, 135 bytes. Catalog: "CTCPhysicalShieldSpell::CancelForCutscene" (propagated over the
//   same shape as 0x007A0400); this body sits between CModeDrained::FrameUpdate 0x008DA890 and
//   CModeThunderLightningStorm::CreateSkyBeam 0x008DAA80 and its offsets fit PDB CModeThunderLightningStorm, so the class is
//   CModeThunderLightningStorm; the name is inferred from the CTCPhysicalShieldSpell::ResumeForCutscene sibling (0x0079E520).
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020 (inlined GetInterface, interface 16 = sound),
//   CDefStringTable::GetString(long) @ 0x009D49B0 on g_DefStringTable 0x013CA828, the sound component's PlaySound @ 0x0066BD00,
//   CCharString::~CCharString @ 0x0099EAE0. g_InvalidSoundHandle @ 0x0129D25C.
// Layout: PDB CModeThunderLightningStorm (0xd4) with 12-byte retail vectors: Owner thing +0x04, PLightningStormDef PDB +0xac ->
//   retail +0xa4, LoopingSound PDB +0xd0 -> retail +0xc4. Def LoopingSound (CDefString id) at retail +0x64 (PDB +0x68).
// Shape: as 0x007A0400 (named sound component in esi, string in its own block sharing the key's frame word).
class CCharString { public:
    void* PStringData;                          // +0 the shared string block
    ~CCharString();                             // 0x0099EAE0
};
struct CDefStringTable {
public:
    CCharString GetString(long) const;   // 0x009D49B0
};
extern CDefStringTable g_DefStringTable;        // 0x013CA828
extern unsigned long g_InvalidSoundHandle;      // 0x0129D25C
class CTCBase;
class CTCSoundPlayer { public:                               // the thing's interface-16 sound component
public:
    unsigned long PlayAttachedCriteria(const CCharString&, bool, bool, bool, long);   // 0x0066BD00
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
class CSpecialAbilitiesThunderLightningStormDef { public:
    unsigned char _pad00[0x64];
    long LoopingSound;                          // +0x64 CDefString id
};
namespace NCreatureMode {
struct CModeThunderLightningStorm {
    void*   __vftable;                          // +0x00 (CModeBase)
    CThing* Owner;                              // +0x04
    unsigned char _pad08[0x9c];
    const CSpecialAbilitiesThunderLightningStormDef* PLightningStormDef;   // +0xa4 CDefPointer
    unsigned char _pada8[0x1c];
    unsigned long LoopingSound;                 // +0xc4
    void ResumeForCutscene();
};
}

void NCreatureMode::CModeThunderLightningStorm::ResumeForCutscene()
{
    if (LoopingSound == g_InvalidSoundHandle) {
        CTCSoundPlayer* sound;
        if (Owner->GetInterface(&sound, 16)) {
            CCharString name = g_DefStringTable.GetString(PLightningStormDef->LoopingSound);
            LoopingSound = sound->PlayAttachedCriteria(name, false, 0, 0, -1);
        }
    }
}