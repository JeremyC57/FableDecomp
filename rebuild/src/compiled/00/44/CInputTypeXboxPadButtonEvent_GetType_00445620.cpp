// CInputTypeXboxPadButtonEvent::GetType
// Retail bytes: b8 02 00 00 00 c3  =>  mov eax,2 ; ret
// __fastcall (this in ecx), no stack args, returns constant enum value 2.
int __fastcall GetType(void *event)
{
    (void)event;
    return 2;
}