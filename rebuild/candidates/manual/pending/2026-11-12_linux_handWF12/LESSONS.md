## 0x009FF220

- (wf12, 2026-11-12) VC7.1 never auto-inlines a function whose body expands ~CGraphicFrameData (member CCountedPointer dtor), and `delete p` inside an inlined callee stays a ??_G call; retail's fully expanded CGraphicFrame release needs FABLE_FORCEINLINE op=/dtor with dtor + ::operator delete spelled out (0x009FF220).
- (wf12, 2026-11-12) An inline accessor returning a reference (PeekGraphicHeader) lets c2 reuse a member pointer across a call in the same statement; the direct member (GraphicInfo->Header) re-reads it like retail (0x009FF220).

## 0x0084D910

- (wf12) A vector sum that VC7.1 hoists above a virtual call when its scale/height are named locals stays after the call when the whole new position is one expression (0x0084D910).

