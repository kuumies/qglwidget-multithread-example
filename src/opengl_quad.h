/* ---------------------------------------------------------------- *
   Author: Antti Jumpponen <kuumies@gmail.com>
   Desc:   Definition of kuu::opengl::Quad class.
 * ---------------------------------------------------------------- */

#pragma once

#include <memory>

class QMatrix4x4;

namespace kuu
{
namespace opengl
{

/* ---------------------------------------------------------------- *
   A quad mesh.
 * ---------------------------------------------------------------- */
class Quad
{
public:
    // Defines a shared pointer of quad.
    using Ptr = std::shared_ptr<Quad>;

    // Constructs the quad.
    Quad(float width = 1.0f, float height = 1.0f);

    // Updates the quad
    void update(float elapsed);

    // Renders the quad
    void render(const QMatrix4x4& view,
                const QMatrix4x4& projection);

private:
    struct Data;
    std::shared_ptr<Data> d;
};

} // namespace opengl
} // namespace kuu
