// CMessageEvent::GetPOriginator<CThing>() const -- the message's originator thing when the pointer still holds one; else null
// Grade: DIFFER(39v52) -- retail keeps the second GetPItem result in a frame word (`push ecx` frame, `mov [esp+8],eax`) and then tests a constant-false bool (`xor al,al ; jne`) before the tail jump; VC7.1 here folds the always-true CThing type test away entirely (tried: empty-mask test, `&ref != 0`, a `return true` helper on `const CThing* const&`, a pointer-cast test -- one probe of five variants, plus the 2026-10-14 forms). Next: lane gap: dead spill + constant test (a forced memory store or a different compiler build); park.
// Leaf: ??$GetPOriginator@VCThing@@@CMessageEvent@@QBEPAVCThing@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006AF4E0, 52 bytes, thiscall, `ret`.
// Identity (probable): Ego_r.pdb CMessageEvent::POriginator (+0x48) and the getter template
//   ??$GetPOriginator@VCThing@@@CMessageEvent@@QBEPAVCThing@@XZ; this +0x48 getter has no type-flag read (CThing needs
//   none), unlike the 0x1F-mask +0x48 twin 0x0090DF30 and the 0x0F-mask PTarget getter 0x006AF520. Its caller is
//   CAIStateGroup_MinionBehaviourCrowdBase code at 0x00957D28.
// Callees: CBaseIntelligentPointer::GetPItem() const 0x00A01B50 (three times; the last a tail jump).
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {
public:
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};
class CThing { public:
    unsigned char _pad[0x6C];
    unsigned long ThingTypeFlags;                        // +0x6C
};

class CMessageEvent {
public:
    unsigned char               _pad00[0x48];
    CIntelligentPointer<CThing> POriginator;             // +0x48
    CIntelligentPointer<CThing> PTarget;                 // +0x50
    template <class T> T* GetPOriginator() const;
};

template <class T> T* CMessageEvent::GetPOriginator() const
{
    T* result = 0;
    if (POriginator.GetPItem()) {
        CThing* thing = POriginator.GetPItem();
        if ((~thing->ThingTypeFlags & 0) == 0)          // CThing: the type mask is empty
            result = (T*)POriginator.GetPItem();
    }
    return result;
}
template CThing* CMessageEvent::GetPOriginator<CThing>() const;
