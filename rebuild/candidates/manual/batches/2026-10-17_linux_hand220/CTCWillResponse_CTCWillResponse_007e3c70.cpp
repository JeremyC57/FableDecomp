// CTCWillResponse::CTCWillResponse(CThing*) -- initialize the response mask from the will definition.
// Retail: 0x007e3c70, 0xe0 bytes. CTCBase ctor 0x00686800, GetSubDef 0x007e47d0.
struct CThing;
struct CDefClassBase { bool GetSubDef(void*); };
struct CDefPointer { CDefClassBase* Object; CDefPointer():Object(0){} };
struct CThing { unsigned char pad[0x70]; CDefPointer PDef; unsigned char pad70_93[0x1f]; unsigned char pad93; };
struct CTCBase { virtual void v00(); CThing* ParentThing; unsigned char pad[4]; CTCBase(CThing*); };
struct CWillDef { unsigned char pad[0x25]; unsigned char Response0; unsigned char Response1; unsigned char Response2; unsigned char Response3; unsigned char pad2[1]; unsigned char Response5; unsigned char Response6; unsigned char Response7; };
struct CTCWillResponse : CTCBase {
  CDefPointer PDef;
  unsigned char ResponseMask;
  virtual void v00();
  CTCWillResponse(CThing*);
};
CTCWillResponse::CTCWillResponse(CThing* thing) : CTCBase(thing), PDef() {
  if (ParentThing->PDef.Object->GetSubDef(&PDef)) {
    CWillDef* d=(CWillDef*)PDef.Object;
    ResponseMask = (unsigned char)((ResponseMask & ~1) | (d->Response0 & 1));
    ResponseMask = (unsigned char)((ResponseMask & ~2) | ((d->Response1 << 1) & 2));
    ResponseMask = (unsigned char)((ResponseMask & ~4) | ((d->Response2 << 2) & 4));
    ResponseMask = (unsigned char)((ResponseMask & ~8) | ((d->Response3 << 3) & 8));
    ResponseMask = (unsigned char)((ResponseMask & ~0x10) | ((d->Response5 << 4) & 0x10));
    ResponseMask = (unsigned char)((ResponseMask & ~0x20) | ((d->Response6 << 5) & 0x20));
    ResponseMask = (unsigned char)((ResponseMask & ~0x40) | ((d->Response7 << 6) & 0x40));
    ParentThing->pad93 = (unsigned char)((ParentThing->pad93 & ~0x20) | ((d->pad2[0] << 5) & 0x20));
  } else ResponseMask = (unsigned char)((ResponseMask & ~0x08) | 0x77);
}
