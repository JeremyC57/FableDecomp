// std::__copy(Input first, Input last, Output result, const random_access_iterator_tag& tag, int* distance) -- assign intelligent pointers forwards.
// Leaf: ??$__copy@PAV?$CIntelligentPointer@$$CBVCThing@@@@PAV1@H@std@@YIPAV?$CIntelligentPointer@$$CBVCThing@@@@PAV1@00ABUrandom_access_iterator_tag@0@PAH@Z
// Identity: proven; vector<CIntelligentPointer<const CThing>>::erase0x738D70 and combat block-list callers; hand951G026own-copy address0x737D50; stride8; real GetPItem0xA01B50 and SetPItem0xA01B90.
class CThing; class CThingCreatureBase; class CThingObject;
class CBaseObject; class CBaseObjectPointer;
class CBaseIntelligentPointer { public: virtual ~CBaseIntelligentPointer(); CBaseObjectPointer* PData; CBaseObject* GetPItem() const; void SetPItem(CBaseObject*); };
template<class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
 T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
 void SetPItem(T* p) { CBaseIntelligentPointer::SetPItem((CBaseObject*)p); }
 CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};
namespace std {
struct input_iterator_tag {}; struct forward_iterator_tag: input_iterator_tag {}; struct bidirectional_iterator_tag: forward_iterator_tag {}; struct random_access_iterator_tag: bidirectional_iterator_tag {};
template<class Input,class Output,class Distance> Output __fastcall __copy(Input first,Input last,Output result,const random_access_iterator_tag&,Distance*) { for(Distance n=last-first;n>0;--n) { *result=*first; ++first; ++result; } return result; }
}
typedef CIntelligentPointer<const CThing> Element;
template Element* __fastcall std::__copy<Element*,Element*,int>(Element*,Element*,Element*,const std::random_access_iterator_tag&,int*);