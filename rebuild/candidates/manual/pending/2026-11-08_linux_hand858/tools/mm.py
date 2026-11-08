# mm.py <variant> <out>: build a DrawMiniMap variant from src/00648820.cpp by replacing the border block
import sys,os
S=os.environ['S']; A=S+'/w22/agent858'
v=sys.argv[1]; out=sys.argv[2]
s=open(A+'/src/00648820.cpp').read()
a=s.index("        const C2DVector& left_pos"); b=s.index("        info.Engine->SetPersistentPrimitive(PrimitiveHandles[7]")
SPR6='''        CEnginePrimitive2DSprite left_border(0, %s, 0x44, def.RegionChangeBorderLeftGraphic, 0, 2, 0, 0.0f, 0.0f, alpha, 0xff, 0xff, 0xff, 2);
        info.Engine->SetPersistentPrimitive(PrimitiveHandles[6], left_border, 0xc0);
'''
SPR7='''        CEnginePrimitive2DSprite right_border(0, %s, 0x44, def.RegionChangeBorderRightGraphic, 0, 2, 0, 0.0f, 0.0f, alpha, 0xff, 0xff, 0xff, 2);
'''
H='''        float height = GPDisplayManager->GetRenderTargetDimensions().Height * (1.0f / 480.0f) * (float)Bank->GetGraphicHeight(def.RegionChangeBorderLeftGraphic);
        %s.TLY = (GPDisplayManager->GetRenderTargetDimensions().Height - height) * 0.5f;
        %s.BRY = %s.TLY + height;
'''
if v=='A':  # init from GFToScreenScaled, 4-float ctor through a pos reference
    new='''        const C2DVector& left_pos = def.RegionChangeBorderLeftOffset;
        C2DBoxF left = GFToScreenScaled(C2DBoxF(left_pos.X, left_pos.Y, left_pos.X + (float)Bank->GetGraphicWidth(def.RegionChangeBorderLeftGraphic),
                                                left_pos.Y + (float)Bank->GetGraphicWidth(def.RegionChangeBorderRightGraphic)));
'''+H%('left','left','left')+SPR6%'left'+'''
        const C2DVector& right_pos = def.RegionChangeBorderRightOffset;
        C2DBoxF right = GFToScreenScaled(C2DBoxF(right_pos.X, right_pos.Y, right_pos.X + (float)Bank->GetGraphicWidth(def.RegionChangeBorderRightGraphic),
                                                 right_pos.Y + (float)Bank->GetGraphicHeight(def.RegionChangeBorderRightGraphic)));
        right.TLY = left.TLY;
        right.BRY = left.BRY;
'''+SPR7%'right'
elif v=='C':  # corner ctor with operator+
    new='''        const C2DVector& left_pos = def.RegionChangeBorderLeftOffset;
        C2DBoxF left = GFToScreenScaled(C2DBoxF(left_pos.X, left_pos.Y, left_pos.X + (float)Bank->GetGraphicWidth(def.RegionChangeBorderLeftGraphic),
                                                left_pos.Y + (float)Bank->GetGraphicWidth(def.RegionChangeBorderRightGraphic)));
'''+H%('left','left','left')+SPR6%'left'+'''
        const C2DVector& right_pos = def.RegionChangeBorderRightOffset;
        C2DBoxF right = GFToScreenScaled(C2DBoxF(right_pos.X, right_pos.Y, right_pos.X + (float)Bank->GetGraphicWidth(def.RegionChangeBorderRightGraphic),
                                                 right_pos.Y + (float)Bank->GetGraphicHeight(def.RegionChangeBorderRightGraphic)));
        right.TLY = left.TLY;
        right.BRY = left.BRY;
'''+SPR7%'right'
    cc=1
else:
    new=open(v).read()
s=s[:a]+new+s[b:]
if v=='C':
    s=s.replace("    C2DBoxF(const C2DVector& top_left, const C2DVector& size);          // 0x0041A7E0",
      "    C2DBoxF(const C2DVector& top_left, const C2DVector& size);          // 0x0041A7E0\n    C2DBoxF(const C2DBoxF& o) : TLX(o.TLX), TLY(o.TLY), BRX(o.BRX), BRY(o.BRY) {}")
open(out,'w').write(s)
