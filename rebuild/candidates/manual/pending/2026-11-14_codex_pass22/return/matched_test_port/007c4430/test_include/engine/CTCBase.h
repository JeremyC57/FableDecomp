#ifndef FIXTURE_CTCBASE_H
#define FIXTURE_CTCBASE_H
class CThing;class CThingSearchTools;extern CThingSearchTools SearchTools;
class CTCBase {public:CThing*ParentThing;CTCBase():ParentThing(0){}CThing&GetParentThing(){assert(ParentThing);return *ParentThing;}const CThingSearchTools&PeekThingSearchTools()const{return SearchTools;}};
#endif
