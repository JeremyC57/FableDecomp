// 009263f0 _Med3<CIntelligentPointer<const CThing>>
// Flags: /O2 /Oy /Gr
// Leaf: _Med3
// Layout: PDB32592 confirms virtual base destructor; retail stride 8, vfptr +0, link +4; CBaseIntelligentPointer callees A01B10/A01B50/A01B90/A01C10.
class CThing;
class CBaseObject;
struct PointerLink {CBaseObject* item;int refs;};
class CBaseIntelligentPointer {public:
 PointerLink* link;
 CBaseIntelligentPointer();virtual ~CBaseIntelligentPointer();
 CBaseObject* GetPItem()const;void SetPItem(CBaseObject*);
};
template<class T>class CIntelligentPointer:public CBaseIntelligentPointer {public:
 T* GetPItem()const{return (T*)CBaseIntelligentPointer::GetPItem();}
 void SetPItem(T*p){CBaseIntelligentPointer::SetPItem((CBaseObject*)p);}
 CIntelligentPointer(){}
 CIntelligentPointer(const CIntelligentPointer&p){SetPItem(p.GetPItem());}
 CIntelligentPointer& operator=(const CIntelligentPointer&p){SetPItem(p.GetPItem());return *this;}
};
typedef CIntelligentPointer<const CThing> ThingPointer;
typedef bool (__fastcall *ThingCompare)(ThingPointer,ThingPointer);
template<class T,class Compare> const T& __fastcall _Med3(const T&a,const T&b,const T&c,Compare comp){if(comp(a,b)){if(comp(b,c))return b;else if(comp(a,c))return c;else return a;}else if(comp(a,c))return a;else if(comp(b,c))return c;else return b;}
template const ThingPointer& __fastcall _Med3(const ThingPointer&,const ThingPointer&,const ThingPointer&,ThingCompare);