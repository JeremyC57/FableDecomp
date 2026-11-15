#pragma once
class CThing;class CCharString;
class CTCBase {public:CThing*ParentThing;CTCBase():ParentThing(0){}CTCBase(CThing&t):ParentThing(&t){}virtual~CTCBase(){};};
