// NUISystem::CTable::Draw(CIEngine& engine, CEnginePrimitiveHandle* handle, long layer, long* count, CComponent* parent) -- a table drawn as a bastard child of its parent: take the parent's render position / relative render position (plus the table position), render zoom / relative render zoom (times the table zoom) and render colour (times the table colour), push them down to every child sprite (colour times the table's, position plus the table's, the relative position scaled by the UI scale, zoom times the table's), set each sprite's graphic to the one mapped to the parent's current state, then draw as a component
// Grade: DIFFER(1399v1399) -- frame slots only (20 masked lines): retail 0x00550EFE fstp [esp+74h] (parent channel) / 0x00550F53 fstp [esp+3Ch] (table channel), ours 3Ch / 74h; retail 0x00550F59 fstp [esp+38h] / 0x00550F63 fstp [esp+34h] (alpha / blue products), ours 34h / 38h; reads 0x00550F77 / 0x00550F7B / 0x00550F83 / 0x00550F92 / 0x00551080 / 0x00551084 follow. Chunk 3005: the outer named-symbol count (k = 1..16) moves only a member-product operand order (period 8), never these slots; member operator* (FableWin ??DCRGBFloatColour@@QBE?AV0@ABV0@@Z) and a converting ctor through operator= are byte- and slot-neutral
// Leaf: ?Draw@CTable@NUISystem@@UAEXAAVCIEngine@@PAVCEnginePrimitiveHandle@@JPAJPAVCComponent@2@@Z
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_CUIDEF
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CTABLE_DRAW
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CRGBFLOATCOLOUR
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CMANAGER
#define FABLE_RETAIL_CRGBFLOATCOLOUR_INITIALISE_CTOR
#include "engine/NUISystem_CTable.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CMorphingSprite.h"
#include "engine/CRGBFloatColour.h"
namespace NUISystem {
void CTable::Draw(CIEngine& engine, CEnginePrimitiveHandle* handle, long layer, long* count, CComponent* parent)
{
    if (IsBastardChild() && parent) {
        RenderPosition = parent->GetRenderPosition() + Position;
        RelativeRenderPosition = parent->GetRelativeRenderPosition() + Position;
        RenderZoom.X = parent->GetRenderZoom().X * Zoom.X;
        RenderZoom.Y = parent->GetRenderZoom().Y * Zoom.Y;
        RelativeRenderZoom.X = parent->GetRelativeRenderZoom().X * Zoom.X;
        RelativeRenderZoom.Y = parent->GetRelativeRenderZoom().Y * Zoom.Y;
        CRGBFloatColour colour = CRGBFloatColour(Colour) * CRGBFloatColour(parent->GetRenderColour());
        RenderColour = colour.GetRGBColour();
        CRGBFloatColour child_colour;
        for (unsigned long i = 0; i < Children.size(); ++i) {
            child_colour = CRGBFloatColour(Children[i]->GetColour()) * colour;
            CRGBColour render_colour = child_colour.GetRGBColour();
            Children[i]->SetRenderColour(render_colour);
            C2DVector position = RenderPosition + Children[i]->GetPosition();
            Children[i]->SetRenderPosition(position);
            C2DVector relative_position = Children[i]->GetPosition();
            relative_position.X *= CManager::GetInstance()->GetUIScale().X;
            relative_position.Y *= CManager::GetInstance()->GetUIScale().Y;
            C2DVector render_position = RelativeRenderPosition + relative_position;
            Children[i]->SetRelativeRenderPosition(render_position);
            C2DVector zoom;
            zoom.X = Children[i]->GetZoom().X;
            zoom.X *= RenderZoom.X;
            zoom.Y = Children[i]->GetZoom().Y;
            zoom.Y *= RenderZoom.Y;
            Children[i]->SetRenderZoom(zoom);
            C2DVector relative_zoom;
            relative_zoom.X = Children[i]->GetZoom().X;
            relative_zoom.X *= RelativeRenderZoom.X;
            relative_zoom.Y = Children[i]->GetZoom().Y;
            relative_zoom.Y *= RelativeRenderZoom.Y;
            Children[i]->SetRelativeRenderZoom(relative_zoom);
            CMorphingSprite* sprite = (CMorphingSprite*)Children[i].operator->();
            sprite->SetGraphicIndex(sprite->GetGRaphicIndices()[parent->GetCurrentStateIndex()]);
        }
    }
    CChangingStateComponent::Draw(engine, handle, layer, count, parent);
}
}
