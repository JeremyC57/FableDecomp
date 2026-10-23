// 00926850 _Adjust_heap<CIntelligentPointer<const CThing>*>
// Flags: /O2 /Oy /Gr
// Leaf: _Adjust_heap
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
template<class T,class Compare> __declspec(noinline) void __fastcall _Push_heap(T*first,int hole,int top,T value,Compare comp){int parent=(hole-1)/2;while(hole>top&&comp(first[parent],value)){first[hole]=first[parent];hole=parent;parent=(hole-1)/2;}first[hole]=value;}
template<class T,class Compare> void __fastcall _Adjust_heap(T*first,int hole,int length,T value,Compare comp){int top=hole;int child=hole*2+2;while(child<length){if(comp(first[child],first[child-1]))--child;first[hole]=first[child];hole=child;child=2*(child+1);}if(child==length){first[hole]=first[child-1];hole=child-1;}_Push_heap(first,hole,top,value,comp);}
template void __fastcall _Adjust_heap(ThingPointer*,int,int,ThingPointer,ThingCompare);