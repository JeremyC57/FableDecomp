// CAIStateGroup_SummonerCharge::InitStateGroup(CAIBrain&, CThingAICreature&) -- reset the charge timer and fetch the summoner sub-def
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0096F730, length 53 bytes (+ int3 padding).
// Callees: CDefClassBase::GetSubDef<CSummonerDef>(CDefPointer<const CSummonerDef>*) @ 0x008CBFF0
// (on the creature's def object, the out-pointer is this class's PDef at +0x1c), the def's
// virtual slot 1 (`call [eax+4]`, Delete) when the temporary's release drops the count to zero.
// Shape: NextFrameToCharge (+0x20, engine header CAIStateGroup_SummonerCharge.h) is set to -1
// first; then a CDefPointer temporary copies the creature's def pointer (PBrain->Creature->PDef
// at +0x70: null-checked AddRef `inc [esi+4]`), GetSubDef runs on the pointee with `&PDef`, and
// the temporary's destructor releases it (`dec [esi+4] ; jne`, virtual Delete at zero; the null
// store is elided before the return). The two reference arguments are unused (`ret 8`).
struct CAIBrain;
struct CThingAICreature;
struct CSummonerDef;

struct CDefPointeeBase {
    virtual void Destroy();                     // slot 0
    virtual void Delete();                      // slot 1: `call [eax+4]`
    long m_refCount;                            // +0x04
};

template <class T>
struct CDefPointer {
    CDefPointeeBase* Object;                    // +0x00
    __forceinline CDefPointer(CDefPointeeBase* object) : Object(object)
    {
        if (Object != 0)
            ++Object->m_refCount;
    }
    __forceinline ~CDefPointer()
    {
        if (Object != 0) {
            if (--Object->m_refCount == 0)
                Object->Delete();
            Object = 0;
        }
    }
    T* operator->() const { return static_cast<T*>(Object); }
};

struct CDefClassBase : CDefPointeeBase {
    bool GetSubDef(CDefPointer<const CSummonerDef>* out);    // 0x008CBFF0 GetSubDef<CSummonerDef>
};

struct CThingAICreatureModel {
    unsigned char _pad[0x70];
    CDefPointeeBase* PDef_Object;               // +0x70 CDefPointer<const CDefClassBase>::Object
};

struct CAIBrainModel {
    unsigned char _pad[0x18];
    CThingAICreatureModel* Creature;            // +0x18
};

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrainModel* PBrain;                      // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
};

struct CAIStateGroup_SummonerCharge : CAIStateGroupBase {
    unsigned char PTarget[8];                   // +0x14 CIntelligentPointer<CThing>
    CDefPointer<const CSummonerDef> PDef;       // +0x1c
    long NextFrameToCharge;                     // +0x20
    virtual void InitStateGroup(CAIBrain& brain, CThingAICreature& creature);
};

void CAIStateGroup_SummonerCharge::InitStateGroup(CAIBrain& brain, CThingAICreature& creature)
{
    NextFrameToCharge = -1;
    CDefPointer<CDefClassBase> def(PBrain->Creature->PDef_Object);
    def->GetSubDef(&PDef);
}