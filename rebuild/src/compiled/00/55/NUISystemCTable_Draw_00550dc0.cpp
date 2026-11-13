// NUISystem::CTable::Draw(CIEngine& engine, CEnginePrimitiveHandle* handle, long layer, long* count, CComponent* parent) -- a table drawn as a bastard child of its parent: take the parent's render position / relative render position (plus the table position), render zoom / relative render zoom (times the table zoom) and render colour (times the table colour), push them down to every child sprite (colour times the table's, position plus the table's, the relative position scaled by the UI scale, zoom times the table's), set each sprite's graphic to the one mapped to the parent's current state, then draw as a component
// Retail 0x00550DC0, 1399 bytes (`ret 14h` at 0x00551334). Shape: the colour block is written like the landed sibling CComponent::UpdateColour (0x0052F900): named `parent_colour` (converted first, after the GetRenderColour call), the table's own `colour`, and `render_colour = colour * parent_colour` (copy-initialised: built in place); the loop then REUSES `colour` for each child's product. That reuse makes the table-colour temporary and the loop colour one packer node (weight 10), so c2's frame packer puts the parent colour's R with render_colour ([esp+74h]) and the table colour's R with the loop colour ([esp+3Ch]), and the alpha / blue products land at [esp+38h] / [esp+34h] as retail (0x00550EFE / 0x00550F53 / 0x00550F59 / 0x00550F63). With a separate loop-only `child_colour` and unnamed temporaries every instruction is the same but those four frame slots swap (chunks 2006 / 3002 / 3005). Callees: virtual slots of CComponent (GetRenderPosition etc.), CManager::GetInstance / GetUIScale, CMorphingSprite::GetGRaphicIndices / SetGraphicIndex, the state map's operator[] (visible STLport body), CChangingStateComponent::Draw (base call), __ftol2 0x00BFEA70.
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
        CRGBFloatColour parent_colour(parent->GetRenderColour());
        CRGBFloatColour colour(Colour);
        CRGBFloatColour render_colour = colour * parent_colour;
        RenderColour = render_colour.GetRGBColour();
        for (unsigned long i = 0; i < Children.size(); ++i) {
            colour = CRGBFloatColour(Children[i]->GetColour()) * render_colour;
            CRGBColour child_render_colour = colour.GetRGBColour();
            Children[i]->SetRenderColour(child_render_colour);
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