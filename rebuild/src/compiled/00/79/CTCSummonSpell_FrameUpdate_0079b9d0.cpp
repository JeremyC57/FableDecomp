// CTCSummonSpell::FrameUpdate() -- on the frame after the cast, create the summoned minion at the owner's position plus its summon offset; afterwards, while the minion lives, keep it on the hero's faction and fade it (and everything it carries) in over the fade frames; once the minion is gone remove this component (TC 0x91) from the owner
// Retail: Fable.exe @ 0x0079B9D0, 494 bytes (plain `ret`, no EH frame).
// Shape: the carried-things loop reads First then Last into named locals before the loop (retail's ecx/eax order, end kept in ebp); the minion test is the nested `if (GetPItem()) { if (!Dead) {...; return;} }` form with the RemoveTC tail last; the faction name is passed as `const char*` (built before the push).
// Callees: owner frame vslot 70 (+0x118), CThing::GetPosition 0x004C73D0, CTCSummonSpell::CreateMinion 0x0079ACC0, GetPItem 0x00A01B50,
//   LowerBound 0x0040F020, CCharString ctor/dtor, CTCEnemy::SetFaction 0x0076C810, __ftol2, CTCGraphicAppearance::SetAlpha 0x004BFAC0,
//   CTCCarrying::CollectCarrySlots 0x006A2220, free 0x00BFEA14, CThing::RemoveTC 0x004C9840
extern "C" void free(void* p);
class CTCBase;
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};
inline C3DVector operator+(const C3DVector& a, const C3DVector& b) { return C3DVector(a.X + b.X, a.Y + b.Y, a.Z + b.Z); }
class CCharString { public:
    void* m_data;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long& type);               // 0x0040F020
};
struct CFrameSource {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53();
    virtual void v54(); virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59();
    virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63(); virtual void v64(); virtual void v65();
    virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69();
    virtual void GetSummonOffset(C3DVector* out);       // vslot 70 (+0x118)
};
struct CTCGraphicAppearance { void SetAlpha(unsigned char alpha); };   // 0x004BFAC0
class CThing {
public:
    virtual void v00();
    unsigned char _pad04[0x1c];
    unsigned long m_presentBits[9];                     // +0x20
    CInterfaceMap m_interfaces;                         // +0x44
    unsigned char _pad4c[0x60 - 0x4c];
    CFrameSource* Frame;                                // +0x60
    CTCGraphicAppearance* Appearance;                   // +0x64
    unsigned char _pad68[0x91 - 0x68];
    unsigned char Dead : 1;                             // +0x91 bit0
    const C3DVector& GetPosition() const;               // 0x004C73D0
    void RemoveTC(long type);                           // 0x004C9840
    template<class T> __forceinline void GetInterface(long interfaceType, T** out)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long key = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(key);
            *out = (T*)(it == map->last || interfaceType < it->type ? map->last : it)->component;
        }
    }
};
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
struct CThingVector {
    CThing** First;
    CThing** Last;
    CThing** EndOfStorage;
    CThingVector() : First(0), Last(0), EndOfStorage(0) { }
    ~CThingVector() { if (First) free(First); }
};
class CTCEnemy { public: void SetFaction(const CCharString& faction); };          // 0x0076C810
class CTCCarrying { public: void CollectCarrySlots(CThingVector* things); };      // 0x006A2220
#pragma pack(push, 1)
class CTCSummonSpell { public:
    virtual void v00();
    CThing* ParentThing;                                // +0x04
    unsigned char _pad08[0x10 - 0x08];
    bool PendingCreate;                                 // +0x10
    CIntelligentPointer<CThing> Minion;                     // +0x11
    unsigned char _pad19[0x1c - 0x19];
    long FadeTotal;                                     // +0x1c
    long FadeFrames;                                    // +0x20
    unsigned char _pad24[0x28 - 0x24];
    long FrameCount;                                    // +0x28
    void CreateMinion(const C3DVector& pos, const C3DVector* facing);   // 0x0079ACC0
    void FrameUpdate();
};
#pragma pack(pop)

void CTCSummonSpell::FrameUpdate()
{
    CThing* thing = ParentThing;
    if (PendingCreate) {
        C3DVector offset;
        thing->Frame->GetSummonOffset(&offset);
        CreateMinion(offset + thing->GetPosition(), 0);
        PendingCreate = false;
        return;
    }
    ++FrameCount;
    if (Minion.GetPItem()) {
        if (!Minion.GetPItem()->Dead) {
            CTCEnemy* enemy;
            Minion.GetPItem()->GetInterface(0x49, &enemy);
            enemy->SetFaction("FACTION_HERO");
            if (FadeFrames > 0) {
                --FadeFrames;
                float alpha = (1.0f - (float)FadeFrames / FadeTotal) * 255.0f;
                CTCGraphicAppearance* appearance = Minion.GetPItem()->Appearance;
                if (appearance)
                    appearance->SetAlpha((unsigned char)alpha);
                CTCCarrying* carrying;
                CThing* minion = Minion.GetPItem();
                if (minion->m_presentBits[0x46 >> 5] & (1UL << (0x46 & 31))) {
                    minion->GetInterface(0x46, &carrying);
                    CThingVector carried;
                    carrying->CollectCarrySlots(&carried);
                    CThing** it = carried.First;
                    CThing** end = carried.Last;
                    for (; it != end; ++it) {
                        CTCGraphicAppearance* a = (*it)->Appearance;
                        if (a)
                            a->SetAlpha((unsigned char)alpha);
                    }
                }
            }
            return;
        }
    }
    thing->RemoveTC(0x91);
}