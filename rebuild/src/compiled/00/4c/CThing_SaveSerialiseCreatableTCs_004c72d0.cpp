// CThing::SaveSerialiseCreatableTCs(CCharString& out) -- forbids adding components, appends a `CreateTC "<name>";` line for every live component that reports itself creatable, then permits adding components again
// Leaf: SaveSerialiseCreatableTCs
// Retail: Fable.exe (TLC, VC7.1) @ 0x004C72D0, 159 bytes (+17 int3). `ret 4`.
// Callees: CTCBase virtual slot 19 (+0x4C, bool) and slot 22 (+0x58, CCharString by hidden pointer) on each component,
//          operator+(const char*, const CCharString&) 0x0099F690 and operator+(const CCharString&, const char*) 0x0099F600
//          (`__fastcall`: hidden result in ecx, the first operand in edx), CCharString::operator+=(const CCharString&)
//          0x0099F0A0, ~CCharString 0x0099EAE0 (x3). Strings: "CreateTC \"" @ 0x01239900, "\";\r\n" @ 0x0123990C.
// Shape: AddingTCsPermitted is bit 6 of the +0x93 flag byte (`and cl,0BFh` store before the loop, `or ..,40h` after);
//        Components {type, component} pairs at +0x44/+0x48 with the last pointer re-read after each pass; the component
//        pointer is re-read from the pair after the slot-19 call; the three string temporaries (name, left join, full
//        line) are destroyed in reverse order; the output reference is loaded into ebx after the empty-map test.
class CTCBase;
struct SComponentPair {                              // pair<ETCInterfaceType, CTCBase*>
    long     first;
    CTCBase* second;
};
class CCharString {
public:
    void* PStringData;
    ~CCharString();                                  // 0x0099EAE0
    CCharString& operator+=(const CCharString& other);   // 0x0099F0A0
};
CCharString __fastcall operator+(const char* left, const CCharString& right);   // 0x0099F690
CCharString __fastcall operator+(const CCharString& left, const char* right);   // 0x0099F600
class CTCBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18();
    virtual bool IsCreatable_slot19();               // slot 19 (+0x4c): serialised as a CreateTC line
    virtual void v20(); virtual void v21();
    virtual CCharString GetName_slot22();            // slot 22 (+0x58)
    void* ParentThing;                               // +0x04
    bool  ToBeRemoved;                               // +0x08
};
class CThing { public:
    void*           __vftable;                       // +0x00
    unsigned char   _pad04[0x44 - 0x04];
    SComponentPair* m_componentsFirst;               // +0x44 Components
    SComponentPair* m_componentsLast;                // +0x48
    unsigned char   _pad4c[0x93 - 0x4c];
    unsigned char   NotPausedDuringCutscenes : 1;    // +0x93 bit 0 (PDB +0xa3)
    unsigned char   DeathPersistent : 1;
    unsigned char   KillOnLevelUnload : 1;
    unsigned char   ActivatedScript : 1;
    unsigned char   LevelIsUnloading : 1;
    unsigned char   IsAffectedByBulletTimeOverride : 1;
    unsigned char   AddingTCsPermitted : 1;          // bit 6
    void SaveSerialiseCreatableTCs(CCharString& out);
};
void CThing::SaveSerialiseCreatableTCs(CCharString& out)
{
    AddingTCsPermitted = false;
    for (SComponentPair* it = m_componentsFirst; it != m_componentsLast; ++it) {
        if (it->second->IsCreatable_slot19()) {
            CTCBase* tc = it->second;
            if (!tc->ToBeRemoved)
                out += "CreateTC \"" + tc->GetName_slot22() + "\";\r\n";
        }
    }
    AddingTCsPermitted = true;
}