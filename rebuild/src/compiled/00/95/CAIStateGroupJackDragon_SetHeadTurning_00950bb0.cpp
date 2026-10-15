// CAIStateGroup_JackDragon::SetHeadTurning(bool enable) -- when the creature's current mode is one of the two dragon modes (types 0x4e / 0x47), set its head-turning flag
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00950BB0, length 120 bytes (+ int3 padding).
// Callees: CVectorMap::LowerBound @ 0x0040F020 (inlined interface lookup, type 0x31 = CTCCreatureModeManager,
// bit 17 of the presence dword at +0x24), CTCCreatureModeManager::GetCurrentMode @ 0x0073A6E0 (landed name;
// manifest "CPixelShader::CountReferences" is propagated) x3, the mode's virtual slot 1 (GetModeType) x2.
//
// Shape: the out-parameter GetInterface template WITHOUT a test of its result (the landed 0x0095AA00 /
// 0x00930DC0 shape): on a miss the never-written manager local is read from a frame slot (`mov esi,
// [esp+0Ch]`, the slot VC shares with the `enable` argument), both arms join in esi. Then
// `if (type == 0x4e) store; else if (type == 0x47) store;` -- the two identical store tails are cross-jumped,
// which hoists `mov ecx,esi` above the first `je` (the `||` form leaves it after); GetCurrentMode() is re-called for every use;
// the byte stored at +0x7c of the current mode. The mode classes are not modelled by
// an engine header: descriptive local model (the flag name is descriptive).
class CTCBase;

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);      // 0x0040F020
};

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
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

struct CMode {
    virtual void v00();
    virtual long GetModeType();                 // slot 1
    unsigned char _pad[0x78];                   // +0x04..+0x7b
    bool m_headTurning;                         // +0x7c (dragon modes 0x47 / 0x4e)
};

struct CTCCreatureModeManager {                 // interface type 0x31
    CMode* GetCurrentMode();                    // 0x0073A6E0
};

struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad[0x0c];                   // +0x08..+0x13
};

struct CAIStateGroup_JackDragon : CAIStateGroupBase {
    void SetHeadTurning(bool enable);
};

void CAIStateGroup_JackDragon::SetHeadTurning(bool enable)
{
    CTCCreatureModeManager* modes;
    PBrain->PCreature->GetInterface(&modes, 0x31);
    if (modes->GetCurrentMode()->GetModeType() == 0x4e)
        modes->GetCurrentMode()->m_headTurning = enable;
    else if (modes->GetCurrentMode()->GetModeType() == 0x47)
        modes->GetCurrentMode()->m_headTurning = enable;
}