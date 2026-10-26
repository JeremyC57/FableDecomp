#pragma optimize("s", on)
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
class CAIStateGroup_EnemyCreatureBase {
public:
    virtual ~CAIStateGroup_EnemyCreatureBase();   // 0x008FD769
};
void* __fastcall CAIStateGroup_WhisperRetreat_CompositeScalarDeletingDestructor(void* stateGroup, int, unsigned int flags)
{
    ((CBaseIntelligentPointer*)(static_cast<unsigned char*>(stateGroup) + 28))->CBaseIntelligentPointer::~CBaseIntelligentPointer();
    ((CAIStateGroup_EnemyCreatureBase*)(stateGroup))->CAIStateGroup_EnemyCreatureBase::~CAIStateGroup_EnemyCreatureBase();
    if (flags & 1)
        operator delete(stateGroup);
    return stateGroup;
}