struct Base { virtual ~Base(){} virtual void Delete() const; mutable unsigned long rc; long Release() const { return --rc; } void AddRef() const { ++rc; } };
#ifdef FI
#define INL __forceinline
#else
#define INL inline
#endif
template<class T> class CDefPointer {
public:
    CDefPointer() : Object(0) {}
    INL ~CDefPointer() { Release(); }
    INL void Release() { const Base* o = Object; if (o) { if (o->Release() == 0) o->Delete(); Object = 0; } }
    const Base* Object;
};
class CTargetingDef;
void Get(CDefPointer<const CTargetingDef>&);
class CActionUseDef;
void Get(CDefPointer<const CActionUseDef>&);
class CReadableDef;
void Get(CDefPointer<const CReadableDef>&);
class CInventoryItemDef;
void Get(CDefPointer<const CInventoryItemDef>&);
class CVillageMemberDef;
void Get(CDefPointer<const CVillageMemberDef>&);
int g0() { CDefPointer<const CTargetingDef> a; Get(a); return 0; }
int g1() { CDefPointer<const CActionUseDef> a; Get(a); return 1; }
int g2() { CDefPointer<const CReadableDef> a; Get(a); return 2; }
int g3() { CDefPointer<const CInventoryItemDef> a; Get(a); return 3; }
int g4() { CDefPointer<const CVillageMemberDef> a; Get(a); return 4; }
