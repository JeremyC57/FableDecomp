class CInputEvent { public: char pad0[0x20]; unsigned long f20; char pad1[0x4]; unsigned long f28; void SetAsNull(); };
void CInputEvent::SetAsNull(){ this->f28 = 0u; this->f20 = 0u; }