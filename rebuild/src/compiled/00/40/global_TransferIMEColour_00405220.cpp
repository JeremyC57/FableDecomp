// TransferIMEColour -- transfer four IME colour channels with independently preserved defaults
// Leaf: ?TransferIMEColour@@YIKAAVCPersistContext@@ABVCCharString@@K@Z
// Retail: 00405220,290 bytes,fastcall.
// Flags: none
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CPERSISTCONTEXT
#define FABLE_RETAIL_CRGBCOLOUR_DWORD_COPY
#include "fable_string.h"
#include "engine/CRGBColour.h"
#include "engine/CPersistContext.h"
extern unsigned char IMERenderAlpha;
CRGBColour FABLE_FASTCALL IMEColour2RGB(unsigned long colour)
{
 unsigned char blue=(unsigned char)colour;
 colour>>=8;
 unsigned char green=(unsigned char)colour;
 colour>>=8;
 unsigned char red=(unsigned char)colour;
 return CRGBColour(red,green,blue,IMERenderAlpha);
}
unsigned long FABLE_FASTCALL RGB2IMEColour(CRGBColour& colour)
{
 return 0xff000000UL|((unsigned long)colour.R<<16)|((unsigned long)colour.G<<8)|colour.B;
}
unsigned long FABLE_FASTCALL TransferIMEColour(CPersistContext& context,const CCharString& token,unsigned long defaultColour)
{
 CRGBColour defCol,col;
 defCol=IMEColour2RGB(defaultColour);
 context.Transfer(token+"R",col.R,defCol.R);
 context.Transfer(token+"G",col.G,defCol.G);
 context.Transfer(token+"B",col.B,defCol.B);
 context.Transfer(token+"A",col.A,defCol.A);
 return RGB2IMEColour(col);
}