// CGameScriptInterface::IsDeedWitnessed  (retail 0x0089c8a0, VC7.1 / MSVC 2003)
// Byte-exact reconstruction. The IsDeedWitnessed method itself is modeled as a free
// __fastcall (this in ecx, unused edx, one stack arg -> ret 4). The two engine helpers
// invoked with extra args (LowerBound, Final) are modeled as __fastcall MEMBER methods
// so VC7.1 passes 'this' in ecx and their extra parameters on the stack WITHOUT touching
// edx — matching retail, which emits no `xor edx,edx` at those call sites. HelperA /
// HelperB (single-arg-less __fastcall, this in ecx) are likewise member methods. All
// callee bodies are extern (relocation-masked).

struct GSI_Self
{
    unsigned char pad00[0x14];
    void*         field14;   // [this+0x14]
};

class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
typedef std::pair<ETCInterfaceType, CTCBase*> TCEntry;

// The iface object whose Final(arg0, 1) is the deed-witness test (__fastcall).
struct IDeedIface
{
    bool Final(void* arg0, int one);   // call 0xffe295f0
};

// The vector-map container at target+0x44. LowerBound is __fastcall (this in ecx,
// &key pushed on the stack, no edx). Its "end" pointer lives at +0x04.
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    void*     begin;                        // +0x00
    TCEntry*  end;                          // +0x04  (== target+0x48)
    TCEntry* LowerBound(const ETCInterfaceType& pKey);  // call 0xffb72780
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > TCMap;

struct GSI_Target;
class CThingPlayerCreature;
class CPlayer
{
public:
    CThingPlayerCreature* GetPControlledCreature() const;   // 0x00487DC0
};

// [self+0x14] object with a single argument-less __fastcall accessor.
struct GSI_HelperAObj
{
    CPlayer* HelperA();   // 0x00449970
};

struct GSI_Target
{
    unsigned char pad00[0x28];
    unsigned int  field28;   // +0x28  (tested & 0x20000)
    unsigned char pad2c[0x18];
    TCMap         map;       // +0x44  (container), map.end == +0x48
    unsigned char pad4c[0x45];
    unsigned char flags91;   // +0x91  (tested & 0x01)
};
class CThingPlayerCreature : public GSI_Target { };

// bool __fastcall CGameScriptInterface::IsDeedWitnessed(void* arg0)
// (this in ecx; single stack arg -> ret 4)
bool __fastcall CGameScriptInterface_IsDeedWitnessed(const void* thisPtr,
                                                     unsigned long /*edx*/,
                                                     void* arg0)
{
    const GSI_Self* self = reinterpret_cast<const GSI_Self*>(thisPtr);

    GSI_HelperAObj* a = reinterpret_cast<GSI_HelperAObj*>(self->field14);
    CPlayer*        b = a->HelperA();
    GSI_Target*     target = b->GetPControlledCreature();

    if (target != 0 &&
        (target->flags91 & 0x01) == 0 &&
        (target->field28 & 0x20000) != 0)
    {
        ETCInterfaceType key = (ETCInterfaceType)(0x51);
        TCMap* container = &target->map;

        TCEntry* node = container->LowerBound(key);
        TCEntry* end  = container->end;

        if (node != end)
        {
            if (node->first > 0x51)
                node = end;
        }
        else
        {
            node = end;
        }

        IDeedIface* iface = reinterpret_cast<IDeedIface*>(((void*)node->second));
        return iface->Final(arg0, 1);
    }

    return false;
}