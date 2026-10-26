// CTCCreatureGeneratorCreator::OnInitialActivate() -- resolve the saved generator UID into the generator pointer (then forget the UID and the generated flag); a generator that is still alive is killed and the pointer cleared
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007C9920, 204 bytes (plain `ret`; 8-byte frame = the intelligent-pointer temporary).
// Callees: CTCBase::GetThingManager() @ 0x00686AE0, CThingManager::GetThingWithUID(__int64) @ 0x0051ED80, CBaseIntelligentPointer
// ctor @ 0x00A01B10 / GetPItem @ 0x00A01B50 / SetPItem @ 0x00A01B90 / dtor @ 0x00A01C10 (CIntelligentPointer<CThing> vtable
// 0x01238C6C), CTCCreatureGeneratorCreator::IsGeneratorAlive() @ 0x007C98A0 (landed), CThing::Kill(bool) @ 0x004C9B80.
// Layout: as the landed IsGeneratorAlive (Ego_r.pdb): Generated +0x10, PGenerator +0x11 (unaligned: the intelligent-pointer
// classes are `pack(1)`), GeneratorUID +0x20 (ulong64; the class derives from a CTCBase struct so the vptr is not padded).
// Shape: `GeneratorUID > 0` on the unsigned 64-bit value (high word `ja`, low word `jbe`), both halves reused as the call's
// pushes; each assignment builds a CIntelligentPointer temporary (base ctor, vtable, SetPItem) copied in by the inline
// operator= (GetPItem() into a local first, so the member address is formed after it) and destroyed through the base dtor; Kill's argument is pushed before GetPItem.
class CThing { public:
    void Kill(bool immediately);                        // 0x004C9B80
};
class CThingManager { public:
    CThing* GetThingWithUID(__int64 uid);               // 0x0051ED80
};
#pragma pack(push, 1)
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
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {  // CIntelligentPointer<CThing>, vtable 0x01238C6C
public:
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { T* item = other.GetPItem(); SetPItem(item); return *this; }

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
#pragma pack(pop)
class CTCBase { public:
    virtual void v00();
    CThing* ParentThing;                                // +0x04
    bool    ToBeRemoved;                                // +0x08
    bool    IsInGlobalUpdateTCs;                        // +0x09
protected:
    CThingManager& GetThingManager();   // 0x00686AE0
};
struct CTCCreatureGeneratorCreator : CTCBase {
    long                FrameToRegenerate;              // +0x0c
    bool                Generated;                      // +0x10
    CIntelligentPointer<CThing> PGenerator;                     // +0x11
    unsigned __int64    GeneratorUID;                   // +0x20
    bool IsGeneratorAlive();                            // 0x007C98A0
    void OnInitialActivate();
};

void CTCCreatureGeneratorCreator::OnInitialActivate()
{
    if (GeneratorUID > 0) {
        PGenerator = CIntelligentPointer<CThing>(GetThingManager().GetThingWithUID(GeneratorUID));
        GeneratorUID = 0;
        Generated = false;
        if (IsGeneratorAlive()) {
            PGenerator.GetPItem()->Kill(false);
            PGenerator = CIntelligentPointer<CThing>(0);
        }
    }
}