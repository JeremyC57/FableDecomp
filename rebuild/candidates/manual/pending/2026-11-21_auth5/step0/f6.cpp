// Standalone VC10 --stl text for Ego_d tc_geometric_shapes.obj (FableLib preset): Ego_d-order FractureShape (evidence only)
#include <vector>
#include <algorithm>
class C2DVector { public: float X, Y; C2DVector(); C2DVector(float x, float y); };
class C3DVector {
public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) { X = x; Y = y; Z = z; }
    C3DVector(const C3DVector& p) { X = p.X; Y = p.Y; Z = p.Z; }
    C3DVector& Normalise();
    const C3DVector operator-(const C3DVector& o) const;
};
class C2DLineF { public: C2DVector Point1, Point2; C2DLineF(const C3DVector& a, const C3DVector& b); };
namespace NShape {
enum ECirculationOrder { CIRCULATION_CLOCKWISE = 0, CIRCULATION_ANTICLOCKWISE = 1 };
class CIsPointFinalLinkInTriangleWithMatchingCirculation {
public:
    CIsPointFinalLinkInTriangleWithMatchingCirculation(const C3DVector& a, const C3DVector& b, ECirculationOrder circ);
    bool operator()(const C3DVector& point) const;
private:
    const C3DVector& PointA; const C3DVector& PointB; C3DVector UpDirection;
};
bool IsLineIntersectingWithOthersInList(const C2DLineF& line, const std::vector<C3DVector>& points_list);
bool IsTriangleWithPointsInside(const std::vector<C3DVector>& triangle, const std::vector<C3DVector>& p_list, ECirculationOrder circ);
void FractureShape(const std::vector<C3DVector>& test_shape, ECirculationOrder& circ, std::vector<std::vector<C3DVector> >& convex_shapes, std::vector<std::vector<C3DVector> >& concave_shapes)
#line 189
{
#line 193
    std::vector<C3DVector> duplicate_shape(test_shape);
#line 195
    std::vector<C3DVector>::iterator it_anchor = duplicate_shape.begin();
#line 197
    C3DVector a = *it_anchor++;
    std::vector<C3DVector>::iterator it_start = it_anchor + 1;
    C3DVector b = *it_anchor;
#line 201
    bool found_appropriate = false;
    bool quit = false;
    std::vector<C3DVector>::iterator it_result;
    std::vector<C3DVector>::iterator it_begin = duplicate_shape.begin();
    while (!quit)
    {
#line 207
        it_result = std::find_if(it_start, duplicate_shape.end(), CIsPointFinalLinkInTriangleWithMatchingCirculation(a, b, circ));
#line 210
        it_start++;
#line 212
        if (it_result != duplicate_shape.end())
        {
#line 214
            const C3DVector& result_point = *it_result;
#line 216
            C2DLineF line_a(a, result_point);
            C2DLineF line_b(b, result_point);
#line 220
            if (!IsLineIntersectingWithOthersInList(line_a, duplicate_shape) && !IsLineIntersectingWithOthersInList(line_b, duplicate_shape))
            {
                std::vector<C3DVector> triangle;
                triangle.push_back(a);
                triangle.push_back(b);
                triangle.push_back(*it_result);
                if (!IsTriangleWithPointsInside(triangle, duplicate_shape, circ))
                {
#line 229
                    found_appropriate = true;
                    quit = true;
                }
#line 232
            }
        }
        else
        {
#line 237
            quit = true;
        }
#line 239
    }
#line 241
    long distance = std::distance(it_begin, it_result);
#line 243
    if (found_appropriate)
    {
#line 245
        std::vector<C3DVector> triangle;
        triangle.push_back(a);
        triangle.push_back(b);
        triangle.push_back(*it_result);
#line 250
        convex_shapes.push_back(triangle);
#line 252
        std::vector<C3DVector>::iterator it_del_start;
        std::vector<C3DVector>::iterator it_del_end;
#line 255
        std::vector<C3DVector> splice_one(duplicate_shape);
        it_del_start = splice_one.begin(); ++it_del_start;
        it_del_end = splice_one.begin(); it_del_end += distance;
#line 259
        splice_one.erase(it_del_start, it_del_end);
#line 261
        long size_1 = splice_one.size();
        if (size_1 > 2)
        {
#line 264
            concave_shapes.push_back(splice_one);
        }
#line 267
        std::vector<C3DVector> splice_two(duplicate_shape);
        it_del_start = splice_two.begin(); it_del_start += distance + 1;
        it_del_end = splice_two.end();
#line 271
        splice_two.erase(it_del_start, it_del_end);
        splice_two.erase(splice_two.begin());
#line 274
        long size_2 = splice_two.size();
        if (size_2 > 2)
        {
#line 277
            concave_shapes.push_back(splice_two);
        }
#line 280
        return;
    }
#line 283
    else if (circ == CIRCULATION_CLOCKWISE)
    {
#line 286
        circ = CIRCULATION_ANTICLOCKWISE;
        concave_shapes.push_back(test_shape);
    }
#line 293
}
}
