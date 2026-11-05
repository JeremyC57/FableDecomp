// CInputTypeMouseWheelMovementEvent::GetType
// Leaf: ?CInputTypeMouseWheelMovementEvent_GetType@@YIHPAX@Z
// Retail 0x00445730: mov eax,6 ; ret
// __fastcall (this in ecx) returning a constant type id.
int __fastcall CInputTypeMouseWheelMovementEvent_GetType(void *ecx)
{
    (void)ecx;
    return 6;
}