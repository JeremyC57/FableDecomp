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
struct Foo { long Method(long arg); };
struct Sub { };

class CCreatureActionBase { public:
    char pad0[8];
    Sub  sub;
    char pad1[0x74 - 8 - sizeof(Sub)];
    Foo* f74;
};

long __fastcall GetAnimationTransitionInTime(CCreatureActionBase* action)
{
    Foo* f = action->f74;
    long r = ((__declspec(noinline) long)(((CBaseIntelligentPointer*)(&action->sub))->GetPItem()));
    return f->Method(r);
}