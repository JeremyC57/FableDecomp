#pragma optimize("s", on)
// CTCVillage::IsHeroMurderer() -- true when the hero's deed log holds, for any of the village's recorded crime handles, a murder deed (type 9) with its flag at +0x23 set
// Leaf: IsHeroMurderer
// Naming wave 18: the interface-presence stand-in CBitset is std::bitset<274> (STLport 4.6); its test is ?test@?$bitset@$0BBC@@std@@QBE_NI@Z (name-only).
// Naming wave 18: the global stand-in g_pMainGameComponent (0x013B86A0) is the protected static CMainGameComponent::PMainGameComponent, read through the
//   inline static accessor Get() as retail code outside the class does (name-only: the bytes are unchanged).
// Retail: Fable.exe @ 0x0065A1FD, 146 bytes; size TU (unaligned entry, `push 51h ; pop edi`): `optimize("s",on)`.
// Callees: CWorld::PeekMyPlayer() 0x0049D9D0 (on CMainGameComponent::Get() 0x013B86A0 +0x24); CPlayer::GetPControlledCreature()
//          0x00487DC0; std::bitset<>::test(size_t) const 0x00410DE0 (interface 81); CVectorMap<ETCInterfaceType,CTCBase*>::
//          LowerBound 0x0040F020 (inlined GetTC); CTCHeroOpinionDeedLog::GetDeedFromHandle(long) 0x006C58F0; STLport
//          _Rb_global<bool>::_M_increment 0x004292C0 (__cdecl, manifest "CMemoryAllocatorVariableSize::GetNoAllocatedAreas"
//          is propagated).
// Layout: retail CTCVillage PlayersCrimes (set<long>, 12 bytes: header node pointer first) +0x64 (PDB +0xdc); set node value
//         at +0x10; COpinionDeed: type +0x00, flag byte +0x23.
// Shape: GetTC goes through an inline map member find(&key, key) that returns `last` on a miss (`cmp [eax],edi ; jg`, the
//        result in the map's esi; the open-coded select puts it in ecx, a `const long&` key is re-read); the GetTC key is a frame local ([ebp-4]); `this` spilled to [ebp-8]; the set walk keeps end() in edi and steps with
//        the static increment; the found arm's `mov al,1` jumps back to the shared epilogue.
class CTCBase;
class CThing;

namespace std {                                                       // STLport 4.6 <bitset>: _Base_bitset<9> words + bitset<274>
template <unsigned int _Nb> class bitset { public:
    // the row lays the 9 words (36 bytes) out itself: the stand-in was an empty class
    bool test(unsigned int __pos) const;                            // 0x00410DE0 ?test@?$bitset@$0BBC@@std@@QBE_NI@Z
};
}
struct CInterfaceMap {                          // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44: {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
    __forceinline SEntry* find(const long* key, long value)
    {
        SEntry* it = LowerBound(key);
        if (it == last || value < it->type)
            return last;
        return it;
    }
};
class CThing {
public:
    unsigned char _pad00[0x20];
    std::bitset<274>       m_present;                    // +0x20
    unsigned char _pad24[0x20];
    CInterfaceMap m_interfaces;                 // +0x44
    bool HasTC(long type) const { return m_present.test(type); }
    __forceinline CTCBase* GetTC(long interfaceType)
    {
        long type = interfaceType;
        return m_interfaces.find(&type, interfaceType)->component;
    }
};
class CThingPlayerCreature : public CThing { };

class CPlayer { public:
    CThingPlayerCreature* GetPControlledCreature() const;                // 0x00487DC0
};
class CWorld {
public:
    CPlayer const* PeekMyPlayer() const;   // 0x0049D9D0 (Ego_r.pdb ?PeekMyPlayer@CWorld@@QBEPBVCPlayer@@XZ)
private:           // 0x0049D9D0
};
class CMainGameComponent {
protected:
    static CMainGameComponent* PMainGameComponent;      // 0x013B86A0 ?PMainGameComponent@CMainGameComponent@@1PAV1@A (protected static; ESTABLISHED_SPELLINGS)
public:
    static CMainGameComponent* __fastcall Get() { return PMainGameComponent; }   // Ego_r / FableWin ?Get@CMainGameComponent@@SIPAV1@XZ (inline)
private: public:
    unsigned char _pad[0x24];
    CWorld*       World;                        // +0x24
};

class COpinionDeed { public:
    long          Type;                         // +0x00 (9 = murder)
    unsigned char _pad_0x04[0x1f];
    bool          Flag23;                       // +0x23
};
class CTCHeroOpinionDeedLog { public: COpinionDeed const* GetDeedFromHandle(long a0) const; private:   // 0x006C58F0
};

struct SRbNode {                                // STLport _Rb_tree_node<long>
    long     _M_color;
    SRbNode* _M_parent;
    SRbNode* _M_left;
    SRbNode* _M_right;
    long     _M_value_field;                    // +0x10
};
struct _Rb_global {
    static SRbNode* __cdecl _M_increment(SRbNode* node);   // 0x004292C0
};

class CTCVillage { public:
    unsigned char _pad_0x00[0x64];
    SRbNode*      PlayersCrimes;                // +0x64 set<long> header node
    bool IsHeroMurderer();
};

bool CTCVillage::IsHeroMurderer()
{
    CThing* hero = ((CPlayer*)((CWorld*)(CMainGameComponent::Get()->World))->PeekMyPlayer())->GetPControlledCreature();
    if (hero && hero->HasTC(81)) {
        CTCHeroOpinionDeedLog* log = (CTCHeroOpinionDeedLog*)hero->GetTC(81);
        SRbNode* end = PlayersCrimes;
        for (SRbNode* it = end->_M_left; it != end; it = _Rb_global::_M_increment(it)) {
            COpinionDeed* deed = ((COpinionDeed*)log->GetDeedFromHandle(it->_M_value_field));
            if (deed && deed->Type == 9 && deed->Flag23)
                return true;
        }
    }
    return false;
}