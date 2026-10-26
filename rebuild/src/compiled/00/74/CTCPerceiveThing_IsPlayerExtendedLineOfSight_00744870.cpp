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
struct CSub {
	unsigned char pad[0x10];
	unsigned char kind; // +0x10
};

class CTCPerceiveThing { public:
	unsigned char pad0[0xc];
	unsigned char sub[0x22];
	unsigned char flag; // +0x2e
};


int __fastcall CTCPerceiveThing_IsPlayerExtendedLineOfSight(CTCPerceiveThing* self)
{
	void* sub = (char*)self + 0xc;
	if (((CSub*)(((CBaseIntelligentPointer*)(sub))->GetPItem())) != 0) {
		if (((CSub*)(((CBaseIntelligentPointer*)(sub))->GetPItem()))->kind == 2) {
			if (self->flag != 0) {
				return 1;
			}
		}
	}
	return 0;
}